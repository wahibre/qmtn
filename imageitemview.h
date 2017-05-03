#ifndef IMAGEITEMVIEW_H
#define IMAGEITEMVIEW_H

#include <QLabel>
#include <QTreeView>

class ImageItemView : public QLabel
{
    QItemSelectionModel *m_model;
public:
    ImageItemView(QWidget *parent = Q_NULLPTR);
    void setModel(QItemSelectionModel *model);

public slots:
    void currentChanged(const QModelIndex &current, const QModelIndex &);
};

#endif // IMAGEITEMVIEW_H
