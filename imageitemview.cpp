#include "imageitemview.h"
#include <QFileInfo>
#include <QPicture>

ImageItemView::ImageItemView(QWidget *parent): QLabel(parent)
{

}

void ImageItemView::setTree(QTreeView *tree)
{
    if(tree)
    {
        m_tree = tree;
//        connect(m_tree->selectionModel(), &QItemSelectionModel::currentRowChanged, this, &ImageItemView::changeItem);
        connect(m_tree->selectionModel(), &QItemSelectionModel::currentChanged, this, &ImageItemView::changeItem);
    }
}

void ImageItemView::changeItem(const QModelIndex &current, const QModelIndex &/*previous*/)
{
//    setText(current.sibling(current.row(), 2).data().toString());
    QString imagepath, log;

    imagepath = current.sibling(current.row(), 3).data().toString();
    log = current.sibling(current.row(), 2).data().toString();

    if(QFileInfo::exists(imagepath))
    {
        QPixmap pix;
        pix.load(imagepath);

        this->setPixmap(pix);
//        setText(imagepath);

    }
    else
        this->setText(log);

}

