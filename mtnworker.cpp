#include "mtnworker.h"

MtnWorker::MtnWorker()
{

}

void MtnWorker::enqueue(QTreeWidgetItem *item)
{
//    fronta.enqueue(item);
    item->setText(2, "spracovane");
}
