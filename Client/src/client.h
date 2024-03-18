#ifndef CLIENT_H
#define CLIENT_H

#include <QMainWindow>
#include <QScopedPointer>

namespace Ui {
class Client;
}

class Client : public QMainWindow
{
    Q_OBJECT

public:
    explicit Client(QWidget *parent = nullptr);
    ~Client() override;

private:
    QScopedPointer<Ui::Client> m_ui;
};

#endif // CLIENT_H
