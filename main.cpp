#include "LifeWindow.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    // 设置应用程序样式
    QApplication::setStyle("Fusion");

    // 创建并显示主窗口
    LifeWindow w;
    w.show();

    return a.exec();
}
