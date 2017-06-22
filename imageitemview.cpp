#include "imageitemview.h"
#include <QAction>
#include <QFileInfo>
#include <QPicture>
#include <QDesktopServices>
#include <QUrl>


ImageItemView::ImageItemView(QWidget *parent): QLabel(parent)
{
    showImageAction = new QAction(ICON_IMAGE, "Show Image using default Application", parent);
    showImageAction->setDisabled(true);
//    imagecache.setMaxCost(10);
    connect(showImageAction, &QAction::triggered, this, &ImageItemView::on_contextMenuRequest);

    this->addAction(showImageAction);
}

QString ImageItemView::imagePath() const
{
    return imagepath;
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
    QPixmap pix;

//    if(imagecache.contains(current))
//    {
//        pix = *imagecache[current];
//    }
//    else
    {
        imagepath = current.sibling(current.row(), columnItemNames::output).data().toString();
        if(QFileInfo::exists(imagepath))
        {
//            QPixmap *cashed = new QPixmap(imagepath);
//            pix = *cashed;
//            imagecache.insert(current, cashed, 1);
            pix.load(imagepath);
        }
        else
        {
            this->setText("Preview not available \n"+imagepath);
             imagepath.clear();
             showImageAction->setEnabled(false);
             return;
        }
    }

    this->setPixmap(pix);
    showImageAction->setEnabled(true);
}

void ImageItemView::on_contextMenuRequest(bool /*checked*/)
{
    QDesktopServices::openUrl(QUrl::fromLocalFile(imagePath()));
}
