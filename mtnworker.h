#ifndef MTNWORKER_H
#define MTNWORKER_H

#include <QQueue>
#include <QTreeWidgetItem>

class MtnWorker
{

    QQueue<QTreeWidgetItem*> fronta;

public:
    MtnWorker();
    void enqueue(QTreeWidgetItem* item);
};

#endif // MTNWORKER_H
