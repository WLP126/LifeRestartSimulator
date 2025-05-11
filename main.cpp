#include "LifeWindow.h"
#include <QApplication>
#include <QGuiApplication>
#include <QtCore>
#include <QDebug>
#include <QDir>

int main(int argc, char *argv[]) {

    QCoreApplication::addLibraryPath("./plugins");

    qDebug() << "资源根目录内容:" << QDir(":/").entryList();

    // 设置应用程序样式
    QApplication::setStyle("Fusion");


     QApplication a(argc, argv);

    // 创建并显示主窗口
    LifeWindow w;
    w.show();

    return a.exec();
}

