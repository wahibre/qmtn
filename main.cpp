#include "mainwindow.h"
#include <QApplication>
#include <QSettings>
#include <QDateTime>
#include <iostream>

void stdoutMessageOutput(QtMsgType type, const QMessageLogContext &/*context*/, const QString &msg)
{
    const char* localMsg = msg.toLocal8Bit().constData();
    const char* cas = QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss").toLatin1().constData();

    switch (type) {
    case QtDebugMsg:
        std::cout << cas << " Debug:    " << localMsg << std::endl;
        break;
    case QtInfoMsg:
        std::cout << cas << " Info:     " << localMsg << std::endl;
        break;
    case QtWarningMsg:
        std::cout << cas << " Warning:  " << localMsg << std::endl;
        break;
    case QtCriticalMsg:
        std::cout << cas << " Critical: " << localMsg << std::endl;
        break;
    case QtFatalMsg:
        std::cout << cas << " Fatal:    " << localMsg << std::endl;
        abort();
    }
}

int main(int argc, char *argv[])
{
    qInstallMessageHandler(stdoutMessageOutput);
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
