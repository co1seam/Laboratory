#include "client.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    Client w("192.168.88.113", 2321);
    w.connectToServer();
    w.show();

    return app.exec();
}

