#ifndef MTNJOB_H
#define MTNJOB_H

#include <QRunnable>
#include <QTreeWidgetItem>
#include <QThread>
#include <QStandardItem>

#include "mtnworker.h"

#define WAIT_STARTTIMEOUT   10000
#define WAIT_FINISHTIMEOUT  20000

class MtnJob : public QRunnable
{
    QStandardItem *m_stditem;
    int m_row;
    SettingsData m_sett;
    QString m_outputfilename;

public:
    MtnJob(QStandardItem *parent, int row, SettingsData settingsData, QString outputfilename);
private:
    void run() Q_DECL_OVERRIDE;
    QStringList createArguments();
};

#endif // MTNJOB_H
