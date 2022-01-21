/* Qt Movie Thumbnailer - movie screencast generator

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
    QWidget *dockParentWidget;
    QRect dockGeometry;
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
            *zoomOrigAct,
            *zoomFullScreenAct;
public:
            ImageItemView(QWidget *parent = Q_NULLPTR);
    void    setModel(QItemSelectionModel *model);
    QString imagePath() const;
public slots:
    void currentChanged(const QModelIndex &current, const QModelIndex &);
    void on_contextMenuRequest(bool);
private:
    void scaleImage(double factor, QPointF pointAt=QPointF());
    void zoomIn();
    void zoomOut();
    void zoomToFitWindow();
    void toggleFullScreen();
    void enableImageActions(bool state);
    void normalSize();
    void adjustScrollBar(QScrollBar *scrollBar, double factor, double positionFactor=0.5);

    // QWidget interface
protected:
    virtual void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;
    virtual void wheelEvent(QWheelEvent *event) Q_DECL_OVERRIDE;
    virtual void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    virtual void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    virtual void mouseReleaseEvent(QMouseEvent *) Q_DECL_OVERRIDE;
    virtual void mouseDoubleClickEvent(QMouseEvent *) Q_DECL_OVERRIDE;
    // QWidget interface
    virtual void keyReleaseEvent(QKeyEvent *event) Q_DECL_OVERRIDE;
    // QObject interface
    virtual bool event(QEvent *event) Q_DECL_OVERRIDE;

};

#endif // IMAGEITEMVIEW_H
