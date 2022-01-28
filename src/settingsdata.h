/* Qt Movie Thumbnailer - movie screencast generator

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

#ifndef SETTINGSDATA_H
#define SETTINGSDATA_H

#include <QColor>
#include <QJsonObject>
#include <QStandardPaths>
#include <QCoreApplication>
#include <QDir>

#define REG_OUTPUT_DIRECTORY    "output_directory"
#define REG_COLUMNS             "columns"
#define REG_ROWS                "rows"
#define REG_WIDTH               "width"
#define REG_EDGE                "edge_detect"
#define REG_BLANK               "blank_skip"
#define REG_QUALITY             "quality"
#define REG_SUFFIX              "suffix"
#define REG_COVER               "cover"
#define REG_TITLE               "title"
#define REG_GAP                 "gap"
#define REG_OVERWRITE           "overwrite"
#define REG_SKIPBEGIN           "skip_begin"
#define REG_SKIPEND             "skip_end"
#define REG_STEP                "time_step"
#define REG_MINHEIGHT           "min_height"
#define REG_SHADOW_RADIUS       "shadow_radius"
#define REG_VERBOSE             "verbose"
#define REG_TRANSPARENT         "transparent"
#define REG_FOREGROUND          "foregound"
#define REG_BACKGROUND          "background"
#define REG_TIMECOLOR           "timecolor"
#define REG_TIMESHADOW          "shadow"
#define REG_INFOTEXT            "infotext"
#define REG_TIMESTAMP           "timestamp"
#define REG_FONTTEXT            "font_info_text"
#define REG_FONTTIME            "font_time_stamp"
#define REG_FONTTEXTSIZE        "font_info_text_size"
#define REG_FONTTIMESIZE        "font_time_stamp_size"
#define REG_FONTTEXTLOCATION    "font_info_text_location"
#define REG_FONTTIMELOCATION    "font_time_stamp_location"
#define REG_ADDITIONAL          "additional"

#define REG_MTN                 "mtn"
#define REG_MAXDIRDEPTH         "max_dir_depth"
#define REG_TIMEOUT             "timeout"
#define REG_SETTINGSNAME        "settings_name"

#ifdef Q_OS_WIN
    #define MTN_EXE "mtn.exe"
#else
    #define MTN_EXE "mtn"
#endif


enum columnItemNames {
    filename    =0,     /* file & folder */
    path        =1,     /* file only     */
    logtext     =2,     /* file only     */
    output      =3      /* file only     */
};


struct SettingsData
{
    /* mnt */
    QString output_directory, suffix, cover, title, fontInfotext, fontTimestamp;
    int columns,rows,width, edge_detect, quality, gap, fontInfoSize, fontTimeSize,
        fontInfoLocation, fontTimeLocation, step, minHeight, shadowRadius;
    double blank_skip, skip_begin, skip_end;
    bool overwrite, infotext, timestamp, verbose, transparent;
    QColor foreground, background, timecolor, timeshadow;
    QString additional;

    /* qmtn extra */
    QString settingsName, executable;
    int timeout;
    int max_dir_depth;

    SettingsData(void);
    SettingsData(QJsonObject obj);
    QJsonObject toJsonObject();
    QByteArray toByteArray();
    static SettingsData SettingsDataFromByteArray(QByteArray bytes, bool &ok, QString &errorText);
private:
    QString findExecutableMtn();
};

Q_DECLARE_METATYPE(SettingsData)

#endif // SETTINGSDATA_H
