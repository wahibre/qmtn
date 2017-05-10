#ifndef IMAGEITEMVIEW_H
#define IMAGEITEMVIEW_H

#include "settingsdata.h"
#include <QLabel>
#include <QTreeView>
//#include <QCache>

class ImageItemView : public QLabel
{
    QItemSelectionModel *m_model;
    QString imagepath;
    QAction *showImageAction;
//    QCache<QModelIndex, QPixmap> imagecache;

public:
    ImageItemView(QWidget *parent = Q_NULLPTR);
    void setModel(QItemSelectionModel *model);

    QString imagePath() const;

public slots:
    void currentChanged(const QModelIndex &current, const QModelIndex &);
    void on_contextMenuRequest(bool);
};

#endif // IMAGEITEMVIEW_H
