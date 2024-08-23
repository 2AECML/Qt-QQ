#include "windowmanager.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    WindowManager wm;

    return a.exec();
}
