#include "mainwindow.h"
#include <QApplication>
#include <QSettings>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QCoreApplication::setOrganizationName("Rusty Pipe");
    QCoreApplication::setApplicationName("Movie Thumbnailer");
//    QCoreApplication::setApplicationVersion(VERZIA);
    QCoreApplication::setQuitLockEnabled(true);

    QSettings::setDefaultFormat(QSettings::IniFormat);

    MainWindow w;
    w.show();

    return a.exec();
}
