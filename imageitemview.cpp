/* Qt Movie Thumbnailer - movie screencast generator

Copyright (C) 2017 wahibre <wahibre@gmx.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#include <QAction>
#include <QFileInfo>
#include <QPicture>
#include <QDesktopServices>
#include <QUrl>
#include <QScrollBar>
#include <QWheelEvent>
#include <QGuiApplication>
#include <QLayout>
//#include <QDebug>

#include "imageitemview.h"
#include "iconprovider.h"
/******************************************************************************************************/
ImageItemView::ImageItemView(QWidget *parent):
    QScrollArea(parent),
    dockParentWidget(parent),
    imageLabel(new QLabel),
    scaleFactor(1.0),
    fitImageToWindow(true),
    dragEnabled(false)

{
    zoomFullScreenAct = new QAction(IconProvider::fullScreen(), tr("Toggle &Fullscreen"), parent);
    zoomFullScreenAct->setShortcut(QKeySequence::FullScreen);
    zoomFullScreenAct->setEnabled(false);
    connect(zoomFullScreenAct, &QAction::triggered, this, &ImageItemView::toggleFullScreen);

    zoomFitAct = new QAction(IconProvider::zoomFit(), tr("Zoom to Fit &Window"), parent);
    zoomFitAct->setEnabled(false);
    connect(zoomFitAct, &QAction::triggered, this, &ImageItemView::zoomToFitWindow);

    zoomInAct = new QAction(IconProvider::zoomIn(), tr("Zoom &In (25%)"), parent);
    zoomInAct->setShortcut(QKeySequence::ZoomIn);
    zoomInAct->setEnabled(false);
    connect(zoomInAct, &QAction::triggered, this, &ImageItemView::zoomIn);

    zoomOutAct = new QAction(IconProvider::zoomOut(), tr("Zoom &Out (25%)"), parent);
    zoomOutAct->setShortcut(QKeySequence::ZoomOut);
    zoomOutAct->setEnabled(false);
    connect(zoomOutAct, &QAction::triggered, this, &ImageItemView::zoomOut);

    zoomOrigAct = new QAction(IconProvider::zoomOrig(), tr("&Normal Size"), parent);
    zoomOrigAct->setEnabled(false);
    connect(zoomOrigAct, &QAction::triggered, this, &ImageItemView::normalSize);

    showImgAct = new QAction(IconProvider::image(), tr("Open Image using &Default Application"), parent);
    showImgAct->setEnabled(false);
    connect(showImgAct, &QAction::triggered, this, &ImageItemView::on_contextMenuRequest);
}
/******************************************************************************************************/
void ImageItemView::setModel(QItemSelectionModel *model)
{
//    imageLabel->setBackgroundRole(QPalette::Base);
    imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    imageLabel->setScaledContents(true);
    imageLabel->setContextMenuPolicy(Qt::ActionsContextMenu);
    imageLabel->setAlignment(Qt::AlignCenter);
    imageLabel->addAction(zoomFullScreenAct);
    imageLabel->addAction(zoomFitAct);
    imageLabel->addAction(zoomInAct);
    imageLabel->addAction(zoomOutAct);
    imageLabel->addAction(zoomOrigAct);
    imageLabel->addAction(showImgAct);

//    setBackgroundRole(QPalette::Dark);
    setWidget(imageLabel);
    setWidgetResizable(false);
    setVisible(false);

    if(model)
    {
        m_model = model;
        connect(m_model, &QItemSelectionModel::currentChanged, this, &ImageItemView::currentChanged);
    }
}/******************************************************************************************************/
void ImageItemView::scaleImage(double factor, QPoint pointAt)
{
    Q_ASSERT(imageLabel->pixmap());
    double factorX, factorY;
    scaleFactor *= factor;

    QGuiApplication::setOverrideCursor(Qt::WaitCursor);
    imageLabel->resize(scaleFactor * imageLabel->pixmap()->size());
    fitImageToWindow = false;

    /** mouse scroll pointing at some place in image */
    if(!pointAt.isNull())
    {
        factorX = (double)pointAt.x() / (double)width();
        factorY = (double)pointAt.y() / (double)height();
    }
    else
    /** middle of the image */
        factorX=factorY=0.5;

    adjustScrollBar(horizontalScrollBar(), factor, factorX);
    adjustScrollBar(verticalScrollBar(), factor, factorY);

    zoomInAct->setEnabled(scaleFactor < 3.0);
    zoomOutAct->setEnabled(scaleFactor > 0.2);
    QGuiApplication::restoreOverrideCursor();
}
/******************************************************************************************************/
QString ImageItemView::imagePath() const
{
    return imagepath;
}
/******************************************************************************************************/
void ImageItemView::zoomIn()
{
    scaleImage(DEFAULT_ZOOM_IN_STEP);
}
/******************************************************************************************************/
void ImageItemView::zoomOut()
{
    scaleImage(DEFAULT_ZOOM_OUT_STEP);
}
/******************************************************************************************************/
void ImageItemView::normalSize()
{
    imageLabel->adjustSize();
    scaleFactor = 1.0;
    fitImageToWindow =false;
}
/******************************************************************************************************/
void ImageItemView::zoomToFitWindow()
{
    Q_ASSERT(imageLabel->pixmap());

    auto imgSize = imageLabel->pixmap()->size();
    imgSize.scale(width()-2, height()-2, Qt::KeepAspectRatio);
    imageLabel->resize(imgSize);
    scaleFactor = ((double)imgSize.height() / (double)imageLabel->pixmap()->size().height());
    fitImageToWindow = true;
}
/******************************************************************************************************/
void ImageItemView::toggleFullScreen()
{
    Q_ASSERT(imageLabel->pixmap());

    if(this->isFullScreen())
    {
        setParent(dockParentWidget);
        dockParentWidget->layout()->addWidget(this);
        setGeometry(dockGeometry);
        showNormal();
    }
    else
    {
        dockGeometry = geometry();
        setParent(Q_NULLPTR);
        fitImageToWindow = true;
        showFullScreen();
    }
}
/******************************************************************************************************/
void ImageItemView::enableImageActions(bool state)
{
    zoomFullScreenAct->setEnabled(state);
    showImgAct->setEnabled(state);
    zoomInAct->setEnabled(state);
    zoomOutAct->setEnabled(state);
    zoomFitAct->setEnabled(state);
    zoomOrigAct->setEnabled(state);
}
/******************************************************************************************************/
void ImageItemView::adjustScrollBar(QScrollBar *scrollBar, double factor, double positionFactor)
{
    scrollBar->setValue(int(factor * scrollBar->value()
                            + ((factor - 1) * scrollBar->pageStep() * positionFactor)));
}
/******************************************************************************************************/
void ImageItemView::currentChanged(const QModelIndex &current, const QModelIndex &/*previous*/)
{
    bool fileExists;

    setVisible(true);
    imagepath = current.sibling(current.row(), columnItemNames::output).data().toString();
    fileExists = (!imagepath.isEmpty() && QFileInfo::exists(imagepath));

    enableImageActions(fileExists);
    setWidgetResizable(!fileExists);

    if(fileExists)
    {
        if(pix.load(imagepath))
        {
            imageLabel->setPixmap(pix);
            zoomToFitWindow();
            fitImageToWindow = true;
        }
        else
        {
            imageLabel->setText(tr("Loading image failed!")+"\n"+imagepath);
            imagepath.clear();
        }
    }
    else
    {
        imageLabel->setText(tr("Preview not available")+"\n"+imagepath);
        imagepath.clear();
    }
}
/******************************************************************************************************/
void ImageItemView::on_contextMenuRequest(bool /*checked*/)
{
    QDesktopServices::openUrl(QUrl::fromLocalFile(imagePath()));
}
/******************************************************************************************************/
void ImageItemView::resizeEvent(QResizeEvent *event)
{
    if(fitImageToWindow && imageLabel->pixmap())
    {
        zoomToFitWindow();
        return;
    }
    else
    {
        if(!imageLabel->text().isNull())
        {
            imageLabel->adjustSize();
            return;
        }
    }
    QScrollArea::resizeEvent(event);
}
/******************************************************************************************************/
void ImageItemView::wheelEvent(QWheelEvent *event)
{
    auto steps = event->angleDelta()/8/15;

    if(steps.isNull())
    {
        event->ignore();
        return;
    }

    /* checking action state is not the best solution - improove! */
    if(steps.y() > 0 && zoomInAct->isEnabled())
        scaleImage(qAbs(steps.y()) * DEFAULT_ZOOM_IN_STEP, event->pos());
    else
        if(steps.y() < 0 && zoomOutAct->isEnabled())
            scaleImage(qAbs(steps.y()) * DEFAULT_ZOOM_OUT_STEP, event->pos());

    event->accept();
}
/******************************************************************************************************/
void ImageItemView::mouseDoubleClickEvent(QMouseEvent */*event*/)
{
    if(imageLabel->pixmap())
    {
        if(fitImageToWindow)
            normalSize();
        else
            zoomToFitWindow();
    }
}
/******************************************************************************************************/
void ImageItemView::keyReleaseEvent(QKeyEvent *event)
{
    if(isFullScreen() && event->key() == Qt::Key_Escape)
    {
        toggleFullScreen();
        event->accept();
        return;
    }

    event->ignore();
}
/******************************************************************************************************/
bool ImageItemView::event(QEvent *event)
{
    switch (event->type()) {
    case QEvent::Close:
        toggleFullScreen();
        event->accept();
        break;
    default:
        return QScrollArea::event(event);
        break;
    }
    return true;
}
/******************************************************************************************************/
void ImageItemView::mousePressEvent(QMouseEvent *event)
{
    if(event->buttons() | Qt::LeftButton
            && imageLabel->pixmap()
            && !fitImageToWindow)
    {
        mouseEventInitPos = event->pos();
        mouseEventInitBars.setX(horizontalScrollBar()->value());
        mouseEventInitBars.setY(verticalScrollBar()->value());
        dragEnabled = true;
        QGuiApplication::setOverrideCursor(Qt::ClosedHandCursor);
    }
}
/******************************************************************************************************/
void ImageItemView::mouseMoveEvent(QMouseEvent *event)
{
    if(dragEnabled)
    {
        QPoint delta = mouseEventInitPos - event->pos();
        horizontalScrollBar()->setValue(mouseEventInitBars.x()+delta.x());
        verticalScrollBar()->setValue(mouseEventInitBars.y() + delta.y());
    }
}
/******************************************************************************************************/
void ImageItemView::mouseReleaseEvent(QMouseEvent */*event*/)
{
    dragEnabled = false;
    QGuiApplication::restoreOverrideCursor();
}
/******************************************************************************************************/
