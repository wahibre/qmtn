#ifndef SETTINGSDATA_H
#define SETTINGSDATA_H

#include <QColor>

#define ICON_FOLDER  QIcon::fromTheme("folder")
#define ICON_LOADING QIcon::fromTheme("image-loading")
#define ICON_VIDEO   QIcon::fromTheme("video-x-generic")
#define ICON_IMAGE   QIcon::fromTheme("image-x-generic")
#define ICON_ERROR   QIcon::fromTheme("sync_error")
#define ICON_REFRESH QIcon::fromTheme("view-refresh")
#define ICON_HELP    QIcon::fromTheme("help-about")
#define ICON_SYSTEM  QIcon::fromTheme("applications-system")
#define ICON_EXIT    QIcon::fromTheme("application-exit")
#define ICON_TEXT    QIcon::fromTheme("accessories-text-editor")

enum columnItemNames {
    filename    =0,     /* file, folder */
    path        =1,     /* file         */
    log         =2,     /* file         */
    output      =3      /* file         */
};

struct SettingsData
{
    QString output_directory, suffix, title, fontInfotext, fontTimestamp;
    int columns,rows,width, edge_detect, quality, gap, fontInfoSize, fontTimeSize,
        fontInfoLocation, fontTimeLocation, step, minHeight;
    double blank_skip, skip_begin, skip_end;
    bool overwrite, infotext, timestamp, verbose;
    QColor foreground, background, timecolor, timeshadow;

    QString executable;
};

#endif // SETTINGSDATA_H
