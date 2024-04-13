// SPDX-FileCopyrightText: 2024 Константин <email>
// SPDX-License-Identifier: MIT

#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QSslSocket>
#include <QNetworkInterface>
#include <QStringLiteral>
#include <QSslCertificate>
#include <QSslKey>
#include <QSslServer>
#include <QFile>

#include "logger.h"
#include "databasemanager.h"

/**
 * @todo write docs
 */
class Server : public QSslServer
{
    Q_OBJECT

public:
    /**
     * Default constructor
     */
    Server();

    /**
     * Destructor
     */
    ~Server();

    void Disconnected();

private:
    QSslCertificate local_certificate;
    QSslKey private_key;
    QSslSocket socket;
    QTcpServer *server;
    QMap<qintptr, QSslSocket*> clients;
    DatabaseManager* db_manager;
    Logger* console_logger;

    /**
     * Server startup.
     * If the server is not tapped on a specific port returns false and displayed an error.
     */
    bool startServer();

    /**
     * Displayed an external IP.
     */
    QString getIP();

    bool openCertificate();

    /**
     * Processes data received from the client.
     * Sends the client true/false depending on the result
     */
    void processClientData(QSslSocket *socket, const QByteArray& data);

    void sendToClient(const QString &function_name);

protected:
    void incomingConnection(qintptr socket_descriptor);

private slots:
    void onEncrypted();
    void onSslErrors(const QList<QSslError> &errors);
    void onClientDisconnected();
    void onReadyRead();

};

#endif // SERVER_H
