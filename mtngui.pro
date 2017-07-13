QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qmtn
TEMPLATE = app
DEFINES += VERSION_FROM_GIT_TAG=\\\"$$system(git describe --tags --abbrev=4)\\\"

target.path = /usr/bin
INSTALLS += target

#TODO use macros: VERSION, QMAKE_TARGET_COMPANY, QMAKE_TARGET_DESCRIPTION, QMAKE_TARGET_COPYRIGHT, QMAKE_TARGET_PRODUCT, RC_LANG, RC_CODEPAGE

ikona.path = /usr/share/pixmaps
ikona.files = rsc/qmtn.png

desktop.path = /usr/share/applications
desktop.files = assets/qmtn.desktop
unix: INSTALLS += ikona desktop

SOURCES += main.cpp\
        mainwindow.cpp \
    mtnworker.cpp \
    mtnjob.cpp \
    imageitemview.cpp \
    settingsdialog.cpp \
    fileselector.cpp \
    iconprovider.cpp

HEADERS  += mainwindow.h \
    mtnworker.h \
    mtnjob.h \
    imageitemview.h \
    settingsdialog.h \
    settingsdata.h \
    fileselector.h \
    iconprovider.h

FORMS    += mainwindow.ui \
    settingsdialog.ui

RESOURCES += \
    images.qrc

ICON = qmtn.ico
RC_ICONS = rsc/qmtn.ico

OTHER_FILES += \
    README
