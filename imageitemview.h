#ifndef IMAGEITEMVIEW_H
#define IMAGEITEMVIEW_H

#include <QLabel>
#include <QTreeView>

class ImageItemView : public QLabel
{
    QItemSelectionModel *m_model;
    QString imagepath;

public:
    ImageItemView(QWidget *parent = Q_NULLPTR);
    void setModel(QItemSelectionModel *model);

    QString imagePath() const;

public slots:
    void currentChanged(const QModelIndex &current, const QModelIndex &);
};

#endif // IMAGEITEMVIEW_H
