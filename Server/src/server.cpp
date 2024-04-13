// SPDX-FileCopyrightText: 2024 Константин <email>
// SPDX-License-Identifier: MIT

#include "server.h"

Server::Server()
{
    console_logger = new Logger(new LogOutputConsole());
    db_manager = new DatabaseManager("QPSQL", "localhost", 5432, "laboratory", "admin", "root", console_logger);

    if(!this->openCertificate())
        return;
    if(!this->startServer())
        return;

}

Server::~Server() = default;

bool Server::startServer()
{
    if(this->listen(QHostAddress::Any, 2321)){
        qDebug().noquote() << QString("\033[1;34m::\033[0m Server has been started \033[1msuccess\033[0m. Running on \033[1m%1:2321\033[0m. (Press \033[1mCTRL+C\033[0m to quit).").arg(getIP());
        return true;
    }
    qDebug().noquote() << QString("\033[1;34m::\033[0m Server has been started \033[1mfailed\033[0m. %1. (Press \033[1mCTRL+C\033[0m to quit).").arg(this->errorString());

    return false;

}

QString Server::getIP()
{
    const QHostAddress &localhost = QHostAddress(QHostAddress::LocalHost);
    for(const QHostAddress &address: QNetworkInterface::allAddresses()){
        if(address.protocol() == QAbstractSocket::IPv4Protocol && address != localhost){
            return address.toString();
        }
    }
    return "Error";
}

bool Server::openCertificate()
{
    QFile cert_file("./../ssl/ca.crt");
    QFile key_file("./../ssl/ca.key");

    if(!cert_file.open(QIODevice::ReadOnly) || !key_file.open(QIODevice::ReadOnly)){;
        console_logger->log(QString("\033[1mFailed to open certificate or key\033[0m"), Logger::Level::ERROR);
        return false;
    }

    QSslCertificate certificate(&cert_file, QSsl::Pem);
    QSslKey key(&key_file, QSsl::Rsa, QSsl::Pem);

    cert_file.close();
    key_file.close();

    if(certificate.isNull() || key.isNull()){
        console_logger->log(QString("\033[1mCertificate of key loading failed\033[0m"), Logger::Level::ERROR);
        return false;
    }
    local_certificate = certificate;
    private_key = key;

    return true;
}

void Server::incomingConnection(qintptr socket_descriptor)
{
    QSslSocket *socket = new QSslSocket(this);

    if(!socket->setSocketDescriptor(socket_descriptor)){
        console_logger->log(QString("Failed to install socket descriptor"), Logger::Level::ERROR);
        socket->deleteLater();
        return;
    }
    connect(socket, &QSslSocket::encrypted, this, &Server::onEncrypted);
    connect(socket, &QSslSocket::sslErrors, this, &Server::onSslErrors);
    connect(socket, &QSslSocket::disconnected, this, &Server::onClientDisconnected);
    connect(socket, &QSslSocket::readyRead, this, &Server::onReadyRead);

    clients.insert(socket_descriptor, socket);
    console_logger->log(QString("Client %1:%2 connected. His descriptor is %3").arg(socket->peerAddress().toString()).arg(socket->peerPort()).arg(socket_descriptor), Logger::Level::INFO);

    socket->setLocalCertificate(local_certificate);
    socket->setPrivateKey(private_key);

    socket->startServerEncryption();

    QByteArray data = socket->readAll();
    sendToClient(QString("get_employees_with_fullname"));
}

void Server::onEncrypted()
{
    QObject* obj = sender();
    if(!obj){
        console_logger->log(QString("Server::onEncrypted() called but sender() is nullptr."), Logger::Level::ERROR);
        return;
    }

    QSslSocket *socket = qobject_cast<QSslSocket *>(obj);
    if(!socket){
        console_logger->log(QString("Server::onEncrypted() called but the sender() is not a QSslSocket."), Logger::Level::INFO);
        return;
    }

    if(!socket->isEncrypted()){
        console_logger->log(QString("Server::onEncrypted() called but the socket is not encrypted."), Logger::Level::ERROR);
        return;
    }

    QString peer_address = socket->peerAddress().toString();
    quint16 peer_port = socket->peerPort();
    console_logger->log(QString("Connection is encrypted with %1:%2").arg(peer_address).arg(peer_port), Logger::Level::INFO);
}

void Server::onSslErrors(const QList<QSslError> &errors)
{
    for(const QSslError &error: errors){
        if(error.error() == QSslError::SelfSignedCertificate){
            continue;
        }
        console_logger->log(QString("Ssl failure: \033[1m%1\033[0m").arg(error.errorString()), Logger::Level::ERROR);
    }
}

void Server::onClientDisconnected()
{
    QObject* obj = sender();
    if(!obj){
        console_logger->log(QString("Server::onClientDisconnected() called but sender() is nullptr"), Logger::Level::ERROR);
        return;
    }

    QSslSocket *socket = qobject_cast<QSslSocket *>(obj);
    if(!socket){
        console_logger->log(QString("Server::onClientDisconnected() called but sender() is not a QSslSocket"), Logger::Level::ERROR);
        return;
    }

    qintptr socket_descriptor = socket->socketDescriptor();
    if(!clients.contains(socket_descriptor)){
        console_logger->log(QString("Server::onClientDisconnected() called but socket descriptor is not found in clients map"), Logger::Level::ERROR);
        return;
    }

    clients.remove(socket_descriptor);
    console_logger->log(QString("Client %1:%2 disconnected. His descriptor has been %3").arg(socket->peerAddress().toString()).arg(socket->peerPort()).arg(socket->socketDescriptor()), Logger::Level::INFO);
    socket->deleteLater();
}

void Server::onReadyRead()
{
    QObject* obj = sender();
    if(!obj){
        console_logger->log(QString("Server::onReadyRead() called but sender() is nullptr"), Logger::Level::ERROR);
        return;
    }

    QSslSocket *socket = qobject_cast<QSslSocket *>(obj);
    if(!socket){
        console_logger->log(QString("Server::onReadyRead() called but sender() is not a QSslSocket"), Logger::Level::ERROR);
        return;
    }
    QByteArray data = socket->readAll();
    processClientData(socket, data);
    sendToClient(QString("get_employees_with_fullname"));
}

void Server::sendToClient(const QString &function_name)
{
    QByteArray block = db_manager->executeSqlFunction(function_name);

    if(block.isEmpty()){
        console_logger->log(QString("No data to send client"), Logger::Level::ERROR);
        return;
    }

    for(QSslSocket *client: clients) {
        if(client->state() != QAbstractSocket::ConnectedState) {
            QString client_info = client ? QString("Client %1:%2 is not connected")
                                            .arg(client->peerAddress().toString(), client->peerPort()) : QString("Client is nullptr");
            console_logger->log(client_info, Logger::Level::WARNING);
            return;
        }
        client->write(QByteArray::number(block.size()) + '\n');
        client->write(block);
        client->flush();
    }
}

void Server::processClientData(QSslSocket* socket, const QByteArray& data)
{
    db_manager->open();

    QDataStream stream(data);
    stream.setVersion(QDataStream::Qt_5_15);

    QString procedure_data;
    QVariantList parameters;

    stream >> procedure_data;
    stream >> parameters;

    if(!db_manager->executeStoredProcedure(procedure_data, parameters)){
        QByteArray response;
        QDataStream response_stream(&response, QIODevice::WriteOnly);
        response_stream.setVersion(QDataStream::Qt_5_15);
        response_stream << false;
        socket->write(response);
        return;
    }

    QByteArray response;
    QDataStream response_stream(&response, QIODevice::WriteOnly);
    response_stream.setVersion(QDataStream::Qt_5_15);
    response_stream << true;
    socket->write(response);
    db_manager->close();
}
