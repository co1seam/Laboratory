// SPDX-FileCopyrightText: 2024 Константин <email>
// SPDX-License-Identifier: MIT

#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QDataStream>
#include <QHttpServer>
#include <QNetworkInterface>
#include <QVector>
#include <QDebug>
/**
 * @todo write docs
 */
class Server : public QTcpServer
{
    Q_OBJECT


public:
    /**
     * Default constructor
     */
    Server();
    ~Server();
private:
    QTcpServer* server;
    QTcpSocket* broadcastSocket;

    void SendToClient(QString str);
    QString GetIP();

public slots:
    void IncomingConnection();
    QByteArray HandleRequest(const QByteArray &requestData);
    void SlotReadyRead();
    void Disconnected();
};

#endif // SERVER_H
