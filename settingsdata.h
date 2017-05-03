#ifndef SETTINGSDATA_H
#define SETTINGSDATA_H


#define ICON_FOLDER  QIcon::fromTheme("folder")
#define ICON_LOADING QIcon::fromTheme("image-loading")
#define ICON_VIDEO   QIcon::fromTheme("video-x-generic")

struct SettingsData
{
    QString output_directory, suffix;
    int columns,rows,width, edge_detect, quality, gap;
    double blank_skip;
    bool overwrite;
};

#endif // SETTINGSDATA_H
