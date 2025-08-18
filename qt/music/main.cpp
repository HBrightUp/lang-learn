#include "widget.h"

#include <QApplication>
#include<QFontDatabase>
#include<QDebug>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(":/app-icon.ico"));
    a.setApplicationDisplayName("Music Player");

    Widget w;
    w.show();
    return a.exec();
}
