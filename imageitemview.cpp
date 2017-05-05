#include "imageitemview.h"
#include <QFileInfo>
#include <QPicture>

QString ImageItemView::imagePath() const
{
    return imagepath;
}

ImageItemView::ImageItemView(QWidget *parent): QLabel(parent)
{

}

void ImageItemView::setModel(QItemSelectionModel *model)
{
    if(model)
    {
        m_model = model;
        connect(m_model, &QItemSelectionModel::currentChanged, this, &ImageItemView::currentChanged);
    }
}

void ImageItemView::currentChanged(const QModelIndex &current, const QModelIndex &/*previous*/)
{
    //TODO add cache
//    QString log;

    imagepath = current.sibling(current.row(), 3).data().toString();
//    log = current.sibling(current.row(), 2).data().toString();

    if(QFileInfo::exists(imagepath))
    {
        QPixmap pix;
        pix.load(imagepath);
        this->setPixmap(pix);
    }
    else
    {
        this->setText("Preview not available");
         imagepath.clear();
    }
}
