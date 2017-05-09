#include "imageitemview.h"
#include <QAction>
#include <QFileInfo>
#include <QPicture>
#include <QDesktopServices>
#include <QUrl>

QString ImageItemView::imagePath() const
{
    return imagepath;
}

ImageItemView::ImageItemView(QWidget *parent): QLabel(parent)
{
    showImageAction = new QAction(ICON_IMAGE, "Show Image using default Application", parent);
    showImageAction->setDisabled(true);
    connect(showImageAction, &QAction::triggered, this, &ImageItemView::on_contextMenuRequest);

    this->addAction(showImageAction);
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

    imagepath = current.sibling(current.row(), dataItemNames::output).data().toString();

    if(QFileInfo::exists(imagepath))
    {
        QPixmap pix;
        pix.load(imagepath);
        this->setPixmap(pix);
        showImageAction->setEnabled(true);
    }
    else
    {
        this->setText("Preview not available \n"+imagepath);
         imagepath.clear();
         showImageAction->setEnabled(false);
    }
}

void ImageItemView::on_contextMenuRequest(bool /*checked*/)
{
    QDesktopServices::openUrl(QUrl::fromLocalFile(imagePath()));
}
