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

#include <QDropEvent>
#include <QMimeData>
#include <QMimeDatabase>
#include <QFileInfo>
#include <QSettings>
#include <QMessageBox>
#include <QDesktopServices>
#include <QFileDialog>
#if QT_VERSION>=0x050600
#include <QVersionNumber>
#endif
#include "mainwindow.h"
#include "settingsdialog.h"
#include "ui_mainwindow.h"
#include "iconprovider.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    processingItems(0),    
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    /*
     *                  VERSION_FROM_GIT_TAG    window title
     *                  --------------------    ------------
     *  tagged commit       0.1                 0.1
     *  after the tag       0.1-10-g23fc        0.1.10
     */
#if QT_VERSION>=0x050600
    setWindowTitle(QString("%1 (%2)").arg(qApp->applicationName()).arg(QVersionNumber::fromString(QString(VERSION_FROM_GIT_TAG).replace('-','.')).toString()));
#else
    setWindowTitle(QString("%1 (%2)").arg(qApp->applicationName()).arg(VERSION_FROM_GIT_TAG));
#endif
    datamodel = new QStandardItemModel(this);
    datamodel->setColumnCount(4);

    profileModel = new ProfileModel(this);
    worker = new MtnWorker(profileModel);

    ui->treeView->hide();
    ui->treeView->setModel(datamodel);
    ui->treeView->header()->hideSection(columnItemNames::path);
    ui->treeView->header()->hideSection(columnItemNames::logtext);
    ui->treeView->header()->hideSection(columnItemNames::output);

    ui->imageViewer->setModel(ui->treeView->selectionModel());


    ui->action_Settings->setIcon(IconProvider::settings());
    ui->action_Quit->setIcon(IconProvider::exit());
    ui->actionAbout->setIcon(IconProvider::help());
    ui->actionAboutQt->setIcon(IconProvider::qt());
    ui->TabOutput->setTabIcon(0, IconProvider::image());
    ui->TabOutput->setTabIcon(1, IconProvider::text());
    ui->actionOpenDirectory->setIcon(IconProvider::folder());
    ui->actionOpenFile->setIcon(IconProvider::video());

    videoExtensions << "3gp"  << "3g2"    << "asf"   << "avi" << "avs"  << "dat"  << "divx"
                    << "dsm"  << "evo"    << "flv"   << "m1v" << "m2ts" << "m2v"  << "m4a"
                    << "mj2"  << "mjpg"   << "mjpeg" << "mkv" << "mov"  << "moov" << "mp4"
                    << "rmvb" << "mpeg"   << "mpv"   << "nut" << "ogg"  << "ogm"  << "xvid"
                    << "mpg"  << "swf"    << "ts"    << "vob" << "webm" << "wmv"  << "qt"
                    << "rm"   << "f4v";


    connect(ui->action_Quit, &QAction::triggered, this, &MainWindow::close);
    connect(ui->treeView->selectionModel(), &QItemSelectionModel::currentRowChanged, this, &MainWindow::currentRowChanged);
    connect(ui->treeView, &QTreeView::customContextMenuRequested, this, &MainWindow::treeContextMenuRequest);
    connect(ui->treeView, &QTreeView::doubleClicked, this, &MainWindow::treeItemDoubleClicked);
    connect(worker, &MtnWorker::changedProcessingItemsNumber, this, &MainWindow::changedProcessingItemsNumber);
    connect(worker, &MtnWorker::generatingSuccess, this, &MainWindow::updateItem);

    QSettings s;
    restoreGeometry(s.value("mainform/geometry").toByteArray());
    restoreState(s.value("mainform/state").toByteArray());
    ui->splitter->restoreState(s.value("mainform/splitter").toByteArray());


    createStatusBarWidgets();
    refreshStatusBar();
}
/******************************************************************************************************/
MainWindow::~MainWindow()
{
    delete ui;
    delete worker;
}
/******************************************************************************************************/
void MainWindow::updateItem(QStandardItem *parent, int row)
{
    if(ui->treeView->currentIndex().parent() == parent->index() &&
       ui->treeView->currentIndex().row()    == row)
    {
        // update log
        currentRowChanged(ui->treeView->currentIndex(), QModelIndex());
        // update image
        ui->imageViewer->currentChanged(ui->treeView->currentIndex(), QModelIndex());
    }
}
/******************************************************************************************************/
void MainWindow::currentRowChanged(const QModelIndex &current, const QModelIndex &/*previous*/)
{
    QString log;

    log = current.sibling(current.row(), columnItemNames::logtext).data().toString();
    ui->logText->setPlainText(log);
}
/******************************************************************************************************/
void MainWindow::treeContextMenuRequest(const QPoint &pos)
{
    auto treeContextMenu = new QMenu(this);

    treeContextMenu->addAction(IconProvider::folder(),  "Open &Directory",      this, SLOT(treeOpenDirectory()));
    treeContextMenu->addAction(IconProvider::video(),   "Open &Movie",          this, SLOT(treeOpenMovie()),        Qt::Key_F3/*to generate hint*/);
    treeContextMenu->addAction(IconProvider::refresh(), "&Recreate Thumbnail",  this, SLOT(recreateThumbnail()),    Qt::Key_F5/*to generate hint*/);
    treeContextMenu->addAction(IconProvider::zoomIn(),  "&Expand all",          ui->treeView, SLOT(expandAll())     );
    treeContextMenu->addAction(IconProvider::zoomOut(), "&Collapse all",        ui->treeView, SLOT(collapseAll())   );
    treeContextMenu->exec(ui->treeView->mapToGlobal(pos));
}
/******************************************************************************************************/
void MainWindow::treeItemDoubleClicked(const QModelIndex &selIndex)
{
    QString movieFileName = selIndex.sibling(
                selIndex.row(),
                columnItemNames::path
                ).data().toString();

    if(!movieFileName.isEmpty())
    {
        QFileInfo f(movieFileName);
        if(f.exists() && f.isFile())
            QDesktopServices::openUrl(QUrl::fromLocalFile(movieFileName));
    }
}
/******************************************************************************************************/
void MainWindow::processUrls(QList<QUrl> urls)
{
    //TODO merge multiple files in the same directory in one node

    QStandardItem *iDir;

    foreach (QUrl files, urls)
    {
        iDir=NULL;
        QFileInfo fi(files.toLocalFile());

        if(fi.isDir())
            iDir = dir2DirItem(QDir(fi.absoluteFilePath()), profileModel->getCurrentSettingsData().max_dir_depth);
        else
            if(fi.isFile())
                iDir = fileInfo2DirItem(fi);
            else
                continue;

        if(iDir && iDir->hasChildren())
        {
            datamodel->appendRow(iDir);
            ui->placeholderLabel->hide();
            ui->treeView->show();
        }
    }
}
/******************************************************************************************************/
void MainWindow::treeOpenDirectory()
{
    if(datamodel->rowCount()>0)
    {
        QModelIndex selIndex = ui->treeView->currentIndex();

        QString movieFileName = selIndex.sibling(
                    selIndex.row(),
                    columnItemNames::path
                    ).data().toString();

        // File Item
        if(!movieFileName.isEmpty())
        {
            QFileInfo f(movieFileName);
            if(f.exists())
            {
                if(f.isDir())
                    QDesktopServices::openUrl(QUrl::fromLocalFile(f.absoluteFilePath()));
                else
                    if(f.isFile())
                        QDesktopServices::openUrl(QUrl::fromLocalFile(f.absoluteDir().absolutePath()));
            }
        }
        else
        // Directory Item
        {
            QFileInfo f(selIndex.child(0,columnItemNames::path).data().toString());

            if(f.exists() && f.isFile())
                QDesktopServices::openUrl(QUrl::fromLocalFile(f.absoluteDir().absolutePath()));
        }
    }
}
/******************************************************************************************************/
void MainWindow::treeOpenMovie()
{
    if(datamodel->rowCount()>0)
        treeItemDoubleClicked(ui->treeView->currentIndex());
}
/******************************************************************************************************/
void MainWindow::recreateThumbnail(const QModelIndex selIndex)
{
    QModelIndex pathCell = selIndex.sibling(
                selIndex.row(),
                columnItemNames::path       /* empty for directories */
                );

    // File Item
    if(!pathCell.data().toString().isEmpty())
        worker->enqueue(datamodel->itemFromIndex(selIndex.parent()), selIndex.row());
    else
    // Directory Item
    {
        int i=0;

        while(selIndex.child(i, 0).isValid())
            recreateThumbnail(selIndex.child(i++, 0));
    }
}
/******************************************************************************************************/
void MainWindow::changedProcessingItemsNumber(int delta)
{
    gardian.lock();
    processingItems += delta;
    gardian.unlock();

    refreshStatusBar();
}
/******************************************************************************************************/
void MainWindow::recreateThumbnail()
{
    if(datamodel->rowCount()>0)
        recreateThumbnail(ui->treeView->currentIndex());
}
/******************************************************************************************************/
void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if(event->modifiers() == Qt::NoModifier)
    {
        switch (event->key()) {
        case Qt::Key_F3:
            this->treeOpenMovie();
            break;

        case Qt::Key_F5:
            this->recreateThumbnail();
                break;
        default:
            event->ignore();
            return;
        }
        event->accept();
    }
    event->ignore();
}
/******************************************************************************************************/
void MainWindow::closeEvent(QCloseEvent *event)
{
    QSettings s;

    s.setValue("mainform/geometry", saveGeometry());
    s.setValue("mainform/state", saveState());
    s.setValue("mainform/splitter", ui->splitter->saveState());

    QMainWindow::closeEvent(event);
}
/******************************************************************************************************/
void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    if(event->mimeData()->hasUrls())
        event->acceptProposedAction();
}
/******************************************************************************************************/
void MainWindow::dropEvent(QDropEvent *event)
{
    // All droped files and directories
    processUrls(event->mimeData()->urls());
}
/******************************************************************************************************/
QStandardItem* MainWindow::dir2DirItem(QDir dir, int recursion_depth)
{
    QStandardItem *iChildDir, *iDir=nullptr;
    QList<QStandardItem*> iChildren;

    if(recursion_depth > 0)
    {
        iDir = new QStandardItem(IconProvider::folder(), dir.dirName()); iDir->setEditable(false);

        //Folders
        foreach (QFileInfo fi, dir.entryInfoList(QDir::Dirs|QDir::NoDotAndDotDot, QDir::Name))
        {
            iChildDir = dir2DirItem(QDir(fi.absoluteFilePath()), recursion_depth-1);
            if(iChildDir)
                iChildren.append(iChildDir);
        }
        iDir->appendRows(iChildren);
        iChildren.clear();

        //Files
        foreach(QFileInfo entry, dir.entryInfoList(QDir::Files, QDir::Name))
            fileInfo2FileItem(entry, iDir);

        if(!iDir->hasChildren())
        {
            delete iDir;
            iDir=0;
        }
    }
    return iDir;
}
/******************************************************************************************************/
QStandardItem* MainWindow::fileInfo2DirItem(QFileInfo file)
{
    QStandardItem *iDir;

    iDir = new QStandardItem(IconProvider::folder(), file.dir().dirName()); iDir->setEditable(false);
    fileInfo2FileItem(file, iDir);

    return iDir;
}
/******************************************************************************************************/
/**
 * @return true if fileitem created, othewise false
 */
bool MainWindow::fileInfo2FileItem(QFileInfo file, QStandardItem *parent)
{
    QStandardItem *iFile, *iAbsFile, *iLog, *iOutputFile;

    if(isVideoFile(file))
    {
        iFile = new QStandardItem(file.fileName());                 iFile->setEditable(false);
        iAbsFile = new QStandardItem(file.absoluteFilePath());      iAbsFile->setEditable(false);
        iLog = new QStandardItem();                                 iLog->setEditable(false);
        iOutputFile = new QStandardItem();                          iOutputFile->setEditable(false);

        QList<QStandardItem*> cols;
        cols << iFile << iAbsFile << iLog << iOutputFile;
        parent->appendRow(cols);

        worker->enqueue(parent, parent->rowCount()-1);

        return true;
    }
    return false;
}
/******************************************************************************************************/
bool MainWindow::isVideoFile(QFileInfo file)
{
    /* based on extension */
    return (videoExtensions.contains(file.suffix(), Qt::CaseInsensitive));

    /* based on mime type
    QMimeDatabase mimedb;
    QMimeType mType = mimedb.mimeTypeForFile(entry, QMimeDatabase::MatchContent);
    */
}
/******************************************************************************************************/
void MainWindow::createStatusBarWidgets()
{
    auto s = statusBar();
    qApp->setStyleSheet("QStatusBar::item { border-width: 0; }");

    sColumns = new QLabel(s);
    sRows = new QLabel(s);
    sOutput = new QLabel(s);
    sStep = new QLabel(s);
    sSuffix = new QLabel(s);

    sItemsCnt = new QLabel(s);

    sOverwrite = new QCheckBox("Overwrite:", s);
    sOverwrite->setLayoutDirection(Qt::RightToLeft);
    sOverwrite->setAttribute(Qt::WA_TransparentForMouseEvents);
    sOverwrite->setFocusPolicy(Qt::NoFocus);

    s->addWidget(sColumns);
    s->addWidget(sStep);
    s->addWidget(sRows);
    s->addWidget(sOutput);
    s->addWidget(sSuffix);
    s->addWidget(sOverwrite);
    s->addPermanentWidget(sItemsCnt);
}
/******************************************************************************************************/
void MainWindow::refreshStatusBar()
{
    auto d = profileModel->getCurrentSettingsData();

    sColumns->setText(QString("Columns: %1 |").arg(d.columns));

    sStep->setText(QString("Step: %1s |").arg(d.step));
    sStep->setVisible(d.rows<=0);

    sRows->setText(QString("Rows: %1 |").arg(d.rows));
    sRows->setVisible(d.rows>0);

    sOutput->setText(QString("Output: %1 |").arg(d.output_directory));
    sOutput->setHidden(d.output_directory.isEmpty());

    sSuffix->setText(QString("Suffix: %1 |").arg(d.suffix));
    sSuffix->setHidden(d.suffix.isEmpty());

    sOverwrite->setChecked(d.overwrite);

    if(processingItems == 0)
        sItemsCnt->clear();
    else
        sItemsCnt->setText(tr("Processing items: %1").arg(processingItems));
}
/******************************************************************************************************/
void MainWindow::on_action_Settings_triggered()
{
    if(profileModel == Q_NULLPTR)
        profileModel = new ProfileModel(this);
    SettingsDialog *dial = new SettingsDialog(this, /*worker.allSettings(), */profileModel);

    if(dial->exec() == QDialog::Accepted)
        refreshStatusBar();
}
/******************************************************************************************************/
void MainWindow::on_actionAboutQt_triggered()
{
    QMessageBox::aboutQt(this, "About Qt");
}
/******************************************************************************************************/
void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::about(this, "About...",
R"(
<html>
    <p>Movie Thumbnailer for creating thumbnails is frontend of CLI <a href="https://gitlab.com/movie_thumbnailer/mtn/wikis/home/">mtn</a>.</p>
    <p>More details at <a href="https://gitlab.com/movie_thumbnailer/qmtn/wikis/home/">HomePage</a>.</p>
    <p>Features:
        <ul>
            <li>Drag&drop files and folders</li>
            <li>Recursive search for movie files</li>
            <li>Instant image making in background</li>
            <li>Image and output log preview</li>
            <li>Open image in external image viewer</li>
            <li>Recreate image with new settings</li>
            <li>Settings for managing mtn switches</li>
        </ul>
    </p>
    <p>
    <code>
            Copyright (C) 2017 &lt;<a href=
)"

+QString("\"mailto:wahibre@gmx.com?Subject=%1\"").arg(windowTitle().toHtmlEscaped())+

R"(
>wahibre@gmx.com</a>&gt;<br>
            Qt Movie Thumbnailer comes with ABSOLUTELY NO WARRANTY.
            This is free software, and you are welcome
            to redistribute it under certain conditions; see <a href="https://www.gnu.org/licenses/gpl.html">GPLv3</a> for details.
    </code>
    </p>
</html>
            )"
    );

}
/******************************************************************************************************/
void MainWindow::on_actionOpenFile_triggered()
{
    auto files = QFileDialog::getOpenFileUrls(this, qApp->applicationName());

    if(!files.isEmpty())
        processUrls(files);
}
/******************************************************************************************************/
void MainWindow::on_actionOpenDirectory_triggered()
{
    QUrl directory = QFileDialog::getExistingDirectoryUrl(this, qApp->applicationName(), QUrl(), QFileDialog::ReadOnly|QFileDialog::ShowDirsOnly);

    if(!directory.isEmpty())
        processUrls({directory});
}
/******************************************************************************************************/
void MainWindow::on_actionRefreshThumbnail_triggered()
{
    recreateThumbnail();
}
/******************************************************************************************************/
