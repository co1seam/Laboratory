#include "client.h"
#include "ui_client.h"

Client::Client(const QString &host, quint16 port, QWidget *parent) :
    QMainWindow(parent),
    m_ui(new Ui::Client),
    m_host(host),
    m_port(port)
{
    m_ui->setupUi(this);
    m_socket = new QSslSocket(this);

    QSslConfiguration ssl_conf = m_socket->sslConfiguration();
    ssl_conf.setPeerVerifyMode(QSslSocket::VerifyNone);
    m_socket->setSslConfiguration(ssl_conf);

    connect(m_socket, &QSslSocket::encrypted, this, &Client::onEncrypted);
    connect(m_socket, &QSslSocket::disconnected, this, &Client::onDisconnected);
    connect(m_socket, QOverload<const QList<QSslError>&>::of(&QSslSocket::sslErrors),
                this, &Client::onSslErrors);
    connect(m_socket, &QSslSocket::readyRead, this, &Client::onReadyRead);
}

Client::~Client() = default;

void Client::connectToServer()
{
    m_socket->connectToHostEncrypted(m_host, m_port);
    if(!m_socket->waitForEncrypted()){
        m_ui->statusBar->showMessage(QString("Не удалось установить соединение"));
    }
}

void Client::disconnectFromServer()
{
    m_socket->disconnectFromHost();
}



void Client::onEncrypted()
{
    m_ui->statusBar->showMessage(QString("Безопасное подключение установлено"));
    emit encrypted();
}

void Client::onDisconnected()
{
    m_ui->statusBar->showMessage((QString("Отсоединение от сервера")));
    emit disconnected();
}

void Client::onSslErrors(const QList<QSslError> &errors)
{
    for(const QSslError &error: errors){
        qDebug() << (QString("Ошибка: %1").arg(error.errorString()));
    }

    emit sslErrorOccured(errors);
}

void Client::readFromServer()
{
    QStandardItemModel *model = new QStandardItemModel(this);
    while(m_socket->bytesAvailable() > 0){
        QByteArray data_block_size = m_socket->readLine();
        bool ok;
        int block_size = data_block_size.toInt(&ok);
        if(!ok){
            return;
        }

        QByteArray block;
        while(block.size() < block_size){
            block.append(m_socket->read(block_size - block.size()));
        }

        QDataStream in(&block, QIODevice::ReadOnly);
        in.setVersion(QDataStream::Qt_5_15);

        qint32 column_count;
        in >> column_count;

        QStringList headers;
        for(qsizetype i = 0; i < column_count; ++i){
            QString header;
            in >> header;
            headers << header;
        }
        model->setHorizontalHeaderLabels(headers);

        while(!in.atEnd()){
            QList<QStandardItem *> items;
            for(qsizetype i = 0; i < column_count; ++i){
                QString data;
                in >> data;

                if(in.status() != QDataStream::Ok){
                    m_ui->statusBar->showMessage(QString("Ошибка чтения данных: %1").arg(in.status()));
                    return;
                }
                items.append(new QStandardItem(data));
            }
            model->appendRow(items);
        }
    }

    if(m_ui->table->model() != model){
        m_ui->table->setModel(model);
    }
}


void Client::onReadyRead()
{
    readFromServer();
    m_ui->statusBar->showMessage(QString("Данные получены"));
}


#include "moc_client.cpp"
