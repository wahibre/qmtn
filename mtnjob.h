#ifndef MTNJOB_H
#define MTNJOB_H

#include <QRunnable>
#include <QTreeWidgetItem>
#include <QThread>

class MtnJob : public QRunnable
{
    QTreeWidgetItem *m_item;

public:
    MtnJob(QTreeWidgetItem *itm);
private:
    void run() Q_DECL_OVERRIDE;
};

#endif // MTNJOB_H
