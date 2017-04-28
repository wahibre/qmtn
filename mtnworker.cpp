#include "mtnworker.h"
#include "mtnjob.h"
#include <QSettings>

MtnWorker::MtnWorker()
{
    dataLoad();
}

MtnWorker::~MtnWorker()
{
    dataSave();
}

void MtnWorker::dataLoad()
{
    QSettings s;

    settingsData.output_directory = s.value(REG_OUTPUT_DIRECTORY, QString()).toString();

    settingsData.columns        = s.value(REG_COLUMNS,   4      ).toInt();
    settingsData.rows           = s.value(REG_ROWS,      8      ).toInt();
    settingsData.width          = s.value(REG_WIDTH,     920    ).toInt();
    settingsData.gap            = s.value(REG_GAP,       3      ).toInt();
    settingsData.overwrite      = s.value(REG_OVERWRITE, true   ).toBool();

    settingsData.edge_detect    = s.value(REG_EDGE,      12     ).toInt();
    settingsData.blank_skip     = s.value(REG_BLANK,     0.80   ).toDouble();
    settingsData.quality        = s.value(REG_QUALITY,   90     ).toInt();
    settingsData.suffix         = s.value(REG_SUFFIX            ).toString();

}

void MtnWorker::dataSave()
{
    QSettings s;

    s.setValue(REG_OUTPUT_DIRECTORY, settingsData.output_directory);

    s.setValue(REG_COLUMNS,         settingsData.columns);
    s.setValue(REG_ROWS,            settingsData.rows);
    s.setValue(REG_WIDTH,           settingsData.width);
    s.setValue(REG_GAP,             settingsData.gap);
    s.setValue(REG_OVERWRITE,       settingsData.overwrite);

    s.setValue(REG_EDGE,            settingsData.edge_detect);
    s.setValue(REG_BLANK,           settingsData.blank_skip);
    s.setValue(REG_QUALITY,         settingsData.quality);
    s.setValue(REG_SUFFIX,          settingsData.suffix);
}

SettingsData MtnWorker::data()
{
    return settingsData;
}

void MtnWorker::setData(SettingsData newData)
{
    settingsData = newData;
}

void MtnWorker::enqueue(QTreeWidgetItem *item)
{
    QThreadPool::globalInstance()->start(new MtnJob(item, settingsData));
}

