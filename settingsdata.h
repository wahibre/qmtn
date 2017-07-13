#ifndef SETTINGSDATA_H
#define SETTINGSDATA_H

#include <QColor>

enum columnItemNames {
    filename    =0,     /* file & folder */
    path        =1,     /* file only     */
    log         =2,     /* file only     */
    output      =3      /* file only     */
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
