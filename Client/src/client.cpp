#include "client.h"
#include "ui_client.h"

Client::Client(QWidget *parent) :
    QMainWindow(parent),
    m_ui(new Ui::Client)
{
    m_ui->setupUi(this);
}

Client::~Client() = default;

#include "moc_client.cpp"
