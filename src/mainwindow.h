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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "mtnworker.h"
#include "profilemodel.h"

#include <QMainWindow>
#include <QTreeWidgetItem>
#include <QDir>
#include <QUrl>
#include <QLabel>
#include <QCheckBox>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

    MtnWorker *worker;
    QStandardItemModel *datamodel;
    QStringList videoExtensions;
    QMutex gardian;
    int processingItems;
    ProfileModel *profileModel=Q_NULLPTR;        
    QMap<QString,QStandardItem*> processingDirs;    // directories in last drop

    /* statusbar widgets */
    QLabel *sColumns, *sRows, *sOutput, *sStep, *sSuffix, *sItemsCnt, *sProfile;
    QCheckBox *sOverwrite;

    int maxRecentFiles;
    QList<QAction*> recentFileActs;
    QStringList recentFiles;
    QAction *separatorAct;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void updateItem(QStandardItem *parent, int row);
private slots:    
    void toolbarContextMenuRequested(const QPoint &pos);
    void toggleToolbarLabels();
    void currentRowChanged(const QModelIndex &current, const QModelIndex &);
    void treeContextMenuRequest(const QPoint &pos);
    void treeItemDoubleClicked(const QModelIndex &selIndex);
    void treeOpenDirectory();
    void treeOpenMovie();
    void recreateThumbnail();
    void recreateThumbnail(const QModelIndex idx);
    void changedProcessingItemsNumber(int delta);
    void openRecentFile();

    void on_action_Settings_triggered();
    void on_actionAboutQt_triggered();
    void on_actionAbout_triggered();
    void on_actionOpenFile_triggered();
    void on_actionOpenDirectory_triggered();
    void on_actionRefreshThumbnail_triggered();
    void on_actionUploadToImgmi_triggered();

private:
    Ui::MainWindow *ui;

    /* Overrides */
    void dropEvent(QDropEvent *event) Q_DECL_OVERRIDE;
    void dragEnterEvent(QDragEnterEvent * event) Q_DECL_OVERRIDE;
    void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;
    virtual void keyPressEvent(QKeyEvent *event) override;

    /* Own */
    QStandardItem *dir2DirItem(const QDir dir, int recursion_depth, const bool topLevel);
    QStandardItem *fileInfo2DirItem(QFileInfo file);
    bool fileInfo2FileItem(QFileInfo file, QStandardItem *parent);
    bool isVideoFile(QFileInfo file);
    void createStatusBarWidgets();
    void createRecentFiles();
    void createRecentMenu();
    void updateRecentFileActions();
    void addRecentFile(QString fileName);
    void refreshStatusBar();
    void processUrls(QList<QUrl> urls);
    QString strippedName(const QString &fullFileName);
};

#endif // MAINWINDOW_H
