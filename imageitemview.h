#ifndef IMAGEITEMVIEW_H
#define IMAGEITEMVIEW_H

#include "settingsdata.h"
#include <QLabel>
#include <QTreeView>
#include <QScrollArea>

#define DEFAULT_ZOOM_IN_STEP  1.25
#define DEFAULT_ZOOM_OUT_STEP 0.8

class ImageItemView : public QScrollArea
{
    //TODO mouse move and scale image
    QItemSelectionModel *m_model;
    QString imagepath;    
    QPixmap pix;
    QLabel *imageLabel;
    double scaleFactor;
    bool fitImageToWindow;
    QPoint mouseEventInitBars, mouseEventInitPos;
    bool dragEnabled;
    QAction *showImgAct,
            *zoomInAct,
            *zoomOutAct,
            *zoomFitAct,
            *zoomOrigAct;
public:
            ImageItemView(QWidget *parent = Q_NULLPTR);
    void    setModel(QItemSelectionModel *model);
    QString imagePath() const;
public slots:
    void currentChanged(const QModelIndex &current, const QModelIndex &);
    void on_contextMenuRequest(bool);
private:
    void scaleImage(double factor, QPoint pointAt=QPoint());
    void zoomIn();
    void zoomOut();
    void zoomToFitWindow();
    void enableImageActions(bool state);
    void normalSize();
    void adjustScrollBar(QScrollBar *scrollBar, double factor, double positionFactor=0.5);

    // QWidget interface
protected:
    virtual void resizeEvent(QResizeEvent *) Q_DECL_OVERRIDE;
    virtual void wheelEvent(QWheelEvent *event) Q_DECL_OVERRIDE;
    virtual void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    virtual void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    virtual void mouseReleaseEvent(QMouseEvent *) Q_DECL_OVERRIDE;
    virtual void mouseDoubleClickEvent(QMouseEvent *) Q_DECL_OVERRIDE;
};

#endif // IMAGEITEMVIEW_H
