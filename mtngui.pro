#-------------------------------------------------
#
# Project created by QtCreator 2017-04-20T18:32:30
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = mtngui
TEMPLATE = app
DEFINES += LAST_TAG=\\\"$$system(git describe --tags --abbrev=4)\\\"


SOURCES += main.cpp\
        mainwindow.cpp \
    mtnworker.cpp \
    mtnjob.cpp \
    imageitemview.cpp \
    settingsdialog.cpp \
    datamodel.cpp

HEADERS  += mainwindow.h \
    mtnworker.h \
    mtnjob.h \
    imageitemview.h \
    settingsdialog.h \
    datamodel.h

FORMS    += mainwindow.ui \
    settingsdialog.ui
