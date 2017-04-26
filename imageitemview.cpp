#include "imageitemview.h"

ImageItemView::ImageItemView(QWidget *parent): QLabel(parent)
{

}

void ImageItemView::setTree(QTreeView *tree)
{
    if(tree)
    {
        m_tree = tree;
        connect(m_tree->selectionModel(), &QItemSelectionModel::currentRowChanged, this, &ImageItemView::changeItem);
    }
}

void ImageItemView::changeItem(const QModelIndex &current, const QModelIndex &/*previous*/)
{
    setText(current.sibling(current.row(), 2).data().toString());
}

