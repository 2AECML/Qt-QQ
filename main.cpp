#include "windowmanager.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    // 创建窗口管理类
    WindowManager wm1;

    // WindowManager wm2;

    return a.exec();
}
