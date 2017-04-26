#include "mtnworker.h"
#include "mtnjob.h"

void MtnWorker::enqueue(QTreeWidgetItem *item)
{
//    fronta.enqueue(item);
    QThreadPool::globalInstance()->start(new MtnJob(item));
}
