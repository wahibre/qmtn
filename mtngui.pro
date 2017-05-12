QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = mtngui
TEMPLATE = app
DEFINES += LAST_TAG=\\\"$$system(git describe --tags --abbrev=4)\\\"
target.path = /usr/bin
INSTALLS += target

SOURCES += main.cpp\
        mainwindow.cpp \
    mtnworker.cpp \
    mtnjob.cpp \
    imageitemview.cpp \
    settingsdialog.cpp \
    fileselector.cpp

HEADERS  += mainwindow.h \
    mtnworker.h \
    mtnjob.h \
    imageitemview.h \
    settingsdialog.h \
    settingsdata.h \
    fileselector.h

FORMS    += mainwindow.ui \
    settingsdialog.ui
