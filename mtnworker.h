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
#define REG_FOREGROUND          "foregound"
#define REG_BACKGROUND          "background"
#define REG_TIMECOLOR           "timecolor"
#define REG_TIMESHADOW          "shadow"
#define REG_INFOTEXT            "infotext"
#define REG_TIMESTAMP           "timestamp"

class MtnWorker
{

    SettingsData settingsData;

public:
    MtnWorker();
    ~MtnWorker();
    void dataLoad();
    void dataSave();

    SettingsData data();
    void setData(SettingsData newData);
    QString outputFile(QString inputfilename);

    void enqueue(QStandardItem* parent, int row);
};

#endif // MTNWORKER_H
