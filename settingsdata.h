#ifndef SETTINGSDATA_H
#define SETTINGSDATA_H

#include <QColor>

#define ICON_FOLDER  QIcon::fromTheme("folder")
#define ICON_LOADING QIcon::fromTheme("image-loading")
#define ICON_VIDEO   QIcon::fromTheme("video-x-generic")

struct SettingsData
{
    QString output_directory, suffix, title;
    int columns,rows,width, edge_detect, quality, gap;
    double blank_skip, skip_begin, skip_end;
    bool overwrite, infotext, timestamp;
    QColor foreground, background, timecolor, timeshadow;
};

#endif // SETTINGSDATA_H
