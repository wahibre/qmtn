#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "mtnworker.h"

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

    MtnWorker worker;
    QStandardItemModel *datamodel;
    QStringList videoExtensions;
    QMutex gardian;
    int processingItems;

    /* statusbar widgets */
    QLabel *sColumns, *sRows, *sOutput, *sStep, *sSuffix, *sItemsCnt;
    QCheckBox *sOverwrite;
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


private slots:    
    void currentRowChanged(const QModelIndex &current, const QModelIndex &);
    void treeContextMenuRequest(const QPoint &pos);
    void openDirectory();
    void recreateThumbnail();
    void recreateThumbnail(const QModelIndex idx);
    void changedProcessingItemsNumber(int delta);

    void on_action_Settings_triggered();
    void on_actionAboutQt_triggered();
    void on_actionAbout_triggered();
    void on_actionOpenFile_triggered();
    void on_actionOpenDirectory_triggered();

private:
    Ui::MainWindow *ui;

    /* Overrides */
    void dropEvent(QDropEvent *event) Q_DECL_OVERRIDE;
    void dragEnterEvent(QDragEnterEvent * event) Q_DECL_OVERRIDE;
    void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;

    /* Own */
    QStandardItem *dir2DirItem(QDir dir);
    QStandardItem *fileInfo2DirItem(QFileInfo file);
    bool fileInfo2FileItem(QFileInfo file, QStandardItem *parent);
    bool isVideoFile(QFileInfo file);
    void createStatusBarWidgets();
    void refreshStatusBar();
    void processUrls(QList<QUrl> urls);
};

#endif // MAINWINDOW_H
