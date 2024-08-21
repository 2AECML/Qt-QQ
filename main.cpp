#include "authwindow.h"


#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    AuthWindow w1;
    w1.show();

    // AuthWindow w2;
    // w2.show();

    // AuthWindow w3;
    // w3.show();

    // AuthWindow w4;
    // w4.show();
    return a.exec();
}
