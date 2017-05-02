#ifndef MTNJOB_H
#define MTNJOB_H

#include <QRunnable>
#include <QTreeWidgetItem>
#include <QThread>
#include "mtnworker.h"

class MtnJob : public QRunnable
{
    QTreeWidgetItem *m_item;
    SettingsData m_sett;
    QString m_outputfilename;

public:
    MtnJob(QTreeWidgetItem *itm, SettingsData settingsData, QString outputfilename);
private:
    void run() Q_DECL_OVERRIDE;
    QStringList createArguments();
};

#endif // MTNJOB_H
