#ifndef IMAGEITEMVIEW_H
#define IMAGEITEMVIEW_H

//#include<QLabel>
#include<QPlainTextEdit>
#include<QTreeView>

class ImageItemView : public QPlainTextEdit
{
    QTreeView *m_tree;
public:
    ImageItemView(QWidget *parent = Q_NULLPTR);
    void setTree(QTreeView *tree);

public slots:
    void changeItem(const QModelIndex &current, const QModelIndex &);
};

#endif // IMAGEITEMVIEW_H
