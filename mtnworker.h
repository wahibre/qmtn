#ifndef MTNWORKER_H
#define MTNWORKER_H

#include <QTreeWidgetItem>
#include <QThreadPool>

#define REG_OUTPUT_DIRECTORY    "output_directory"
#define REG_COLUMNS             "columns"
#define REG_ROWS                "rows"
#define REG_WIDTH               "width"
#define REG_EDGE                "edge_detect"
#define REG_BLANK               "blank_skip"
#define REG_QUALITY             "quality"
#define REG_SUFFIX              "suffix"
#define REG_GAP                 "gap"
#define REG_OVERWRITE           "overwrite"


struct SettingsData
{
    QString output_directory, suffix;
    int columns,rows,width, edge_detect, quality, gap;
    double blank_skip;
    bool overwrite;
};




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

    void enqueue(QTreeWidgetItem* item);
};

#endif // MTNWORKER_H
