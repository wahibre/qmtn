#include "mtnjob.h"

MtnJob::MtnJob(QTreeWidgetItem *itm):
    QRunnable(),
    m_item(itm)
{

}

void MtnJob::run()
{
    m_item->setIcon(0, QIcon::fromTheme("video-x-generic"));
    m_item->setText(2, QString::asprintf("spracovane vo vlakne: %p", QThread::currentThread()));
}
