#ifndef CLIENT_H
#define CLIENT_H

#include <QMainWindow>
#include <QScopedPointer>
#include <QSslSocket>
#include <QSslConfiguration>
#include <QStandardItemModel>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QStatusBar>

namespace Ui {
class Client;
}

class Client : public QMainWindow
{
    Q_OBJECT

public:
    explicit Client(const QString &host, quint16 port, QWidget *parent = nullptr);
    ~Client() override;
    void connectToServer();
    void disconnectFromServer();
    void readFromServer();

private:
    QScopedPointer<Ui::Client> m_ui;
    QString m_host;
    quint16 m_port;
    QSslSocket *m_socket;

signals:
    void encrypted();
    void disconnected();
    void sslErrorOccured(const QList<QSslError> &error);
    void dataReceived(const QByteArray &data);

private slots:
    void onEncrypted();
    void onDisconnected();
    void onSslErrors(const QList< QSslError > &errors);
    void onReadyRead();
};

#endif // CLIENT_H
