#ifndef MTNWORKER_H
#define MTNWORKER_H

#include <QTreeWidgetItem>
#include <QThreadPool>
#include <QStandardItem>

#include "settingsdata.h"

#define REG_OUTPUT_DIRECTORY    "output_directory"
#define REG_COLUMNS             "columns"
#define REG_ROWS                "rows"
#define REG_WIDTH               "width"
#define REG_EDGE                "edge_detect"
#define REG_BLANK               "blank_skip"
#define REG_QUALITY             "quality"
#define REG_SUFFIX              "suffix"
#define REG_TITLE               "title"
#define REG_GAP                 "gap"
#define REG_OVERWRITE           "overwrite"
#define REG_SKIPBEGIN           "skip_begin"
#define REG_SKIPEND             "skip_end"
#define REG_STEP                "time_step"
#define REG_MINHEIGHT           "min_height"
#define REG_VERBOSE             "verbose"
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

class MtnWorker
{

    SettingsData settingsData;

public:
    MtnWorker();
    ~MtnWorker();
    void dataLoad();
    void dataSave();
    static QString __mtn(){return "mtn";}

    SettingsData data();
    void setData(SettingsData newData);
    QString outputFile(QString inputfilename);
    bool findExecutable();

    void enqueue(QStandardItem* parent, int row);
};

#endif // MTNWORKER_H
