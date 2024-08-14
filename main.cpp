#include "authwindow.h"


#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    AuthWindow authWindow;
    authWindow.show();
    return a.exec();
}
