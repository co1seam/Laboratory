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
    model->clear();
    emit disconnected();
}

void Client::onSslErrors(const QList<QSslError> &errors)
{
    for(const QSslError &error: errors){
        qDebug().noquote() << QString("Ошибка: %1").arg(error.errorString());
    }

    emit sslErrorOccured(errors);
}

void Client::readFromServer()
{
    model->clear();

    QByteArray data_block_size = m_socket->readLine();
    bool ok;
    int block_size = data_block_size.toInt(&ok);
    qDebug().noquote() << QString("Block size is: %1").arg(block_size);
    if(!ok){
        return;
    }

    QByteArray block;
    while(block.size() < block_size){
        block.append(m_socket->read(block_size - block.size()));
    }

    QDataStream in(&block, QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_5_15);

    quint32 column_count;
    in >> block_size >> column_count;
    qDebug().noquote() << QString("Column count is: %1").arg(column_count);

    QStringList headers;

    for(qsizetype i = 0; i < column_count; ++i){
        QString header;
        in >> header;
        qDebug().noquote() << QString("Header is: %1").arg(header);
        headers << header;
    }

    model->setHorizontalHeaderLabels(headers);

    while(in.status() == QDataStream::Ok && !in.atEnd()){
        QList<QStandardItem *> items;
        for(qsizetype i = 0; i < column_count; ++i){
            QString data;
            in >> data;
            qDebug().noquote() << (QString("Data is: %1").arg(data));
            items.append(new QStandardItem(data));
        }
        model->appendRow(items);
    }

    if(m_ui->statusBar && in.status() != QDataStream::Ok){
        m_ui->statusBar->showMessage(QString("Ошибка чтения данных: %1").arg(in.status()));
        delete model;
        return;
    }
    m_ui->table->setModel(model);
}



void Client::onReadyRead()
{
    readFromServer();
}

void Client::add_employee(){
    QLineEdit *LineEdits[] = {
        m_ui->LastNameInput1, m_ui->FirstNameInput1, m_ui->MiddleNameInput1, m_ui->AgeInput1,
        m_ui->LastNameInput2, m_ui->FirstNameInput2, m_ui->MiddleNameInput2, m_ui->AgeInput2,
        m_ui->LastNameInput3, m_ui->FirstNameInput3, m_ui->MiddleNameInput3, m_ui->AgeInput3,
        m_ui->LastNameInput4, m_ui->FirstNameInput4, m_ui->MiddleNameInput4, m_ui->AgeInput4,
        m_ui->LastNameInput5, m_ui->FirstNameInput5, m_ui->MiddleNameInput5, m_ui->AgeInput5
    };

    QComboBox *QComboBoxes[] = {
        m_ui->SexInput1, m_ui->AvailabilityOfChildren1, m_ui->MaritalStatus1, m_ui->Position1, m_ui->AcademicDegree1,
        m_ui->SexInput2, m_ui->AvailabilityOfChildren2, m_ui->MaritalStatus2, m_ui->Position2, m_ui->AcademicDegree2,
        m_ui->SexInput3, m_ui->AvailabilityOfChildren3, m_ui->MaritalStatus3, m_ui->Position3, m_ui->AcademicDegree3,
        m_ui->SexInput4, m_ui->AvailabilityOfChildren4, m_ui->MaritalStatus4, m_ui->Position4, m_ui->AcademicDegree4,
        m_ui->SexInput5, m_ui->AvailabilityOfChildren5, m_ui->MaritalStatus5, m_ui->Position5, m_ui->AcademicDegree5
    };

    for(qsizetype i = 0; i < 20; i += 4){
        QString lastname = LineEdits[i]->text();
        QString firstname = LineEdits[i+1]->text();
        QString secondname = LineEdits[i+2]->text();
        qint32 age = LineEdits[i+3]->text()->toInt();
        QString sex = QComboBoxes[i/4*5]->currentText();
        QString availability_of_children = QComboBoxes[i/4*5+1]->currentText();
        QString marital_status = QComboBoxes[i/4*5+2]->currentText();
        QString position = QComboBoxes[i/4*5+3]->currentText();
        QString academic_degree = QComboBoxes[i/4*5+4]->currentText();

        if(firstname.isEmpty() &&
            lastname.isEmpty() &&
            secondname.isEmpty() &&
            age.isNull() &&
            sex.isEmpty()&&
            availability_of_children.isEmpty() &&
            marital_status.isEmpty() &&
            position.isEmpty() &&
            academic_degree.isEmpty()){
            continue;
        }

        QByteArray block;
        QDataStream out(&block, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_5_15);

        out << quint32(0);

        quint32 block_size = 0;
        {

        }

    }
}

#include "moc_client.cpp"
