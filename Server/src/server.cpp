// SPDX-FileCopyrightText: 2024 Константин <email>
// SPDX-License-Identifier: MIT

#include "server.h"

Server::Server()
{
    server = new QTcpServer(this);
    connect(server, &QTcpServer::newConnection, this, &Server::IncomingConnection);

    if(!server->listen(QHostAddress::Any, 2321)){
        qDebug().noquote() << QString("\033[1;34m::\033[0m Server has been started \033[1mfailed\033[0m. %1.\n(Press \033[1mCTRL+C\033[0m to quit).").arg(this->errorString());
        return;
    }

    qDebug().noquote() << QString("\033[1;34m::\033[0m Server has been started \033[1msuccess\033[0m. Running on \033[1m%1:2321\033[0m.\n(Press \033[1mCTRL+C\033[0m to quit).").arg(GetIP());
}

Server::~Server() = default;

QByteArray Server::HandleRequest(const QByteArray& requestData)
{
    return "Response from server";
}


QString Server::GetIP()
{
    const QHostAddress &localhost = QHostAddress(QHostAddress::LocalHost);
    for(const QHostAddress &address: QNetworkInterface::allAddresses()){
        if(address.protocol() == QAbstractSocket::IPv4Protocol && address != localhost){
            return address.toString();
        }
    }
    return "1";
}


void Server::IncomingConnection()
{
    while(server->hasPendingConnections()){
        QTcpSocket* socket = server->nextPendingConnection();
        connect(socket, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(stateChanged(QAbstractSocket::SocketState)));
        connect(socket, &QTcpSocket::readyRead, this, &Server::SlotReadyRead);
        connect(socket, &QTcpSocket::disconnected, this, &Server::Disconnected);

    }
}

void Server::Disconnected(){
    QTcpSocket* socket = qobject_cast<QTcpSocket*>(sender());
    if(socket){
        socket->deleteLater();
    }
}

void Server::SlotReadyRead()
{
    QTcpSocket* socket = static_cast<QTcpSocket*>(sender());
    if(!socket){
        return;
    }

    QByteArray request_data = socket->readAll();

    qDebug() <<;

    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_5_9);
    if(in.status() == QDataStream::Ok){
        qDebug() << "Read...";
        QString str;
        in >> str;
        qDebug() << str;
    } else {
        qDebug() << "DataStream is failed";
    }
}

void Server::SendToClient(QString str)
{
    data.clear();
    QDataStream out(&data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_9);
    out << str;
    socket->write(data);
}

