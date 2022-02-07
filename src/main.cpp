/* Qt Movie Thumbnailer - movie screencast generator

Copyright (C) 2017 wahibre <wahibre@gmx.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#include "mainwindow.h"
#include <QApplication>
#include <QSettings>
#include <QDateTime>
#include <iostream>
#include <QTranslator>

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
#ifndef QT_DEBUG
    qInstallMessageHandler(stdoutMessageOutput);
#endif
    QApplication a(argc, argv);

    QCoreApplication::setOrganizationName("Rusty Pipe");
    QCoreApplication::setApplicationName("Qt Movie Thumbnailer");
//    QCoreApplication::setApplicationVersion(VERZIA);
    QCoreApplication::setQuitLockEnabled(true);

    auto lang = QLocale::system().name();
    if(lang.left(2) == "sk")
    {
        auto trans = new QTranslator();
        if(trans->load(":lang/sk.qm"))
            a.installTranslator(trans);
    }

    QSettings::setDefaultFormat(QSettings::IniFormat);

    MainWindow w;
    w.show();

    return a.exec();
}
