#ifndef SETTINGSDATA_H
#define SETTINGSDATA_H

#include <QColor>

#define ICON_FOLDER  QIcon::fromTheme("folder")
#define ICON_LOADING QIcon::fromTheme("image-loading")
#define ICON_VIDEO   QIcon::fromTheme("video-x-generic")
#define ICON_IMAGE   QIcon::fromTheme("image-x-generic")
#define ICON_ERROR   QIcon::fromTheme("sync_error")

enum dataItemNames {
    filename    =0,
    path        =1,
    log         =2,
    output      =3
};

struct SettingsData
{
    QString output_directory, suffix, title, fontInfotext, fontTimestamp;
    int columns,rows,width, edge_detect, quality, gap, fontInfoSize, fontTimeSize;
    double blank_skip, skip_begin, skip_end;
    bool overwrite, infotext, timestamp;
    QColor foreground, background, timecolor, timeshadow;

    QString executable;
};

#endif // SETTINGSDATA_H
