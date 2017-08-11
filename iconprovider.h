#ifndef ICONPROVIDER_H
#define ICONPROVIDER_H

#include <QIcon>

class IconProvider
{
public:
    IconProvider();
    static QIcon folder();
    static QIcon loading();
    static QIcon video();
    static QIcon image();
    static QIcon error();
    static QIcon refresh();
    static QIcon help();
    static QIcon settings();
    static QIcon exit();
    static QIcon text();
    static QIcon zoomIn();
    static QIcon zoomOut();
    static QIcon zoomOrig();
    static QIcon zoomFit();
    static QIcon qt();
};

#endif // ICONPROVIDER_H
