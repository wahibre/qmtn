#include "imageitemview.h"
#include <QAction>
#include <QFileInfo>
#include <QPicture>
#include <QDesktopServices>
#include <QUrl>
#include <QScrollBar>
#include <QWheelEvent>
#include <QGuiApplication>

/******************************************************************************************************/
ImageItemView::ImageItemView(QWidget *parent):
    QScrollArea(parent),
    imageLabel(new QLabel),
    scaleFactor(1.0),
    fitImageToWindow(true),
    dragEnabled(false)

{
    zoomFitAct = new QAction(tr("Zoom to &Fit Window"), parent);
    zoomFitAct->setEnabled(false);
    connect(zoomFitAct, &QAction::triggered, this, &ImageItemView::zoomToFitWindow);

    zoomInAct = new QAction(tr("Zoom &In (25%)"), parent);
    zoomInAct->setShortcut(QKeySequence::ZoomIn);
    zoomInAct->setEnabled(false);
    connect(zoomInAct, &QAction::triggered, this, &ImageItemView::zoomIn);

    zoomOutAct = new QAction(tr("Zoom &Out (25%)"), parent);
    zoomOutAct->setShortcut(QKeySequence::ZoomOut);
    zoomOutAct->setEnabled(false);
    connect(zoomOutAct, &QAction::triggered, this, &ImageItemView::zoomOut);

    zoomOrigAct = new QAction(tr("&Normal Size"), parent);
    zoomOrigAct->setEnabled(false);
    connect(zoomOrigAct, &QAction::triggered, this, &ImageItemView::normalSize);

    showImgAct = new QAction(/*ICON_IMAGE,*/ tr("Show Image using &Default Application"), parent);
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
void ImageItemView::enableImageActions(bool state)
{
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
        pix.load(imagepath);
        imageLabel->setPixmap(pix);
        zoomToFitWindow();
        fitImageToWindow = true;
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
void ImageItemView::resizeEvent(QResizeEvent */*event*/)
{
    if(fitImageToWindow && imageLabel->pixmap())
        zoomToFitWindow();
    else
        if(!imageLabel->text().isNull())
            imageLabel->adjustSize();
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
    //TODO new function 'imageLoaded'
    if(imageLabel->pixmap())
    {
        if(fitImageToWindow)
            normalSize();
        else
            zoomToFitWindow();
    }
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
