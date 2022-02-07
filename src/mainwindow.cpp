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
#include <QToolBar>
#if QT_VERSION>=0x050600
#include <QVersionNumber>
#include <QDebug>
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

    ui->mainToolBar->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->action_Settings->setIcon(IconProvider::settings());
    ui->action_Quit->setIcon(IconProvider::exit());
    ui->actionAbout->setIcon(IconProvider::help());
    ui->actionAboutQt->setIcon(IconProvider::qt());
    ui->TabOutput->setTabIcon(0, IconProvider::image());
    ui->TabOutput->setTabIcon(1, IconProvider::text());
    ui->actionOpenDirectory->setIcon(IconProvider::folder());
    ui->actionOpenFile->setIcon(IconProvider::video());
    ui->actionRefreshThumbnail->setIcon(IconProvider::refresh());
    ui->actionRemoveItemfromSidebar->setIcon(IconProvider::remove());

    videoExtensions << "3gp"  << "3g2"    << "asf"   << "avi" << "avs"  << "dat"  << "divx"
                    << "dsm"  << "evo"    << "flv"   << "m1v" << "m2ts" << "m2v"  << "m4a"
                    << "mj2"  << "mjpg"   << "mjpeg" << "mkv" << "mov"  << "moov" << "mp4"
                    << "rmvb" << "mpeg"   << "mpv"   << "nut" << "ogg"  << "ogm"  << "xvid"
                    << "mpg"  << "swf"    << "ts"    << "vob" << "webm" << "wmv"  << "qt"
                    << "rm"   << "f4v";

    connect(ui->mainToolBar, &QToolBar::customContextMenuRequested, this, &MainWindow::toolbarContextMenuRequested);
    connect(ui->action_Quit, &QAction::triggered, this, &MainWindow::close);
    connect(ui->treeView->selectionModel(), &QItemSelectionModel::currentRowChanged, this, &MainWindow::currentRowChanged);
    connect(ui->treeView->selectionModel(), &QItemSelectionModel::selectionChanged,  this, &MainWindow::selectionChanged);
    connect(ui->treeView, &QTreeView::customContextMenuRequested, this, &MainWindow::treeContextMenuRequest);
    connect(ui->treeView, &QTreeView::doubleClicked, this, &MainWindow::treeItemDoubleClicked);
    connect(worker, &MtnWorker::changedProcessingItemsNumber, this, &MainWindow::changedProcessingItemsNumber);
    connect(worker, &MtnWorker::generatingSuccess, this, &MainWindow::updateItem);

    QSettings s;
    restoreGeometry(s.value("mainform/geometry").toByteArray());
    restoreState(s.value("mainform/state").toByteArray());
    ui->splitter->restoreState(s.value("mainform/splitter").toByteArray());
    recentFiles = s.value("recentFiles").toStringList();
    maxRecentFiles = s.value("MaxRecentFiles", 5).toInt();
    ui->mainToolBar->setToolButtonStyle(
                static_cast<Qt::ToolButtonStyle>(s.value("mainform/toolbarlabels", Qt::ToolButtonTextBesideIcon).toInt()));


#if (QT_VERSION >= QT_VERSION_CHECK(5, 5, 0))

    bool envIsSet = false;
    int envImgUpload = 0;

    envImgUpload = qEnvironmentVariableIntValue("QMTN_ENABLE_IMG_UPLOAD", &envIsSet);

    bool imgUploadEnabled = (envIsSet && envImgUpload == 1);
#else
    bool imgUploadEnabled = false;
#endif

    if(imgUploadEnabled)
    {
        ui->actionUploadToImagevenue->setVisible(true);
        ui->actionUploadToImgaa->setVisible(true);
        ui->actionUploadToKlikr->setVisible(true);
        //ui->actionUploadToImgmi->setVisible(false); // protected by cloudflare
    }

    createStatusBarWidgets();
    createRecentFiles();
    createRecentMenu();
    refreshStatusBar();
    updateActionState();
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
void MainWindow::toolbarContextMenuRequested(const QPoint &pos)
{
    QMenu m;

    QAction *a = m.addAction(tr("Display labels"), this, SLOT(toggleToolbarLabels()));
    a->setCheckable(true);
    a->setChecked(ui->mainToolBar->toolButtonStyle() == Qt::ToolButtonTextBesideIcon);

    m.exec(ui->mainToolBar->mapToGlobal(pos));
}
/******************************************************************************************************/
void MainWindow::toggleToolbarLabels()
{
    if(ui->mainToolBar->toolButtonStyle() == Qt::ToolButtonTextBesideIcon)
        ui->mainToolBar->setToolButtonStyle(Qt::ToolButtonIconOnly);
    else
        ui->mainToolBar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
}
/******************************************************************************************************/
void MainWindow::currentRowChanged(const QModelIndex &current, const QModelIndex &/*previous*/)
{
    QString log;

    log = current.sibling(current.row(), columnItemNames::logtext).data().toString();
    ui->logText->setPlainText(log);
}
/******************************************************************************************************/
void MainWindow::selectionChanged(const QItemSelection&, const QItemSelection&)
{
    updateActionState();
}
/******************************************************************************************************/
void MainWindow::treeContextMenuRequest(const QPoint &pos)
{
    auto treeContextMenu = new QMenu(this);

    treeContextMenu->addAction(IconProvider::folder(),  tr("Open &Directory"),      this, SLOT(treeOpenDirectory()));
    treeContextMenu->addAction(IconProvider::video(),   tr("Open &Movie"),          this, SLOT(treeOpenMovie()),        Qt::Key_F3/*to generate hint*/);
    treeContextMenu->addAction(ui->actionRemoveItemfromSidebar);
    treeContextMenu->addAction(IconProvider::zoomIn(),  tr("&Expand all"),          ui->treeView, SLOT(expandAll())     );
    treeContextMenu->addAction(IconProvider::zoomOut(), tr("&Collapse all"),        ui->treeView, SLOT(collapseAll())   );
    treeContextMenu->addAction(IconProvider::refresh(), tr("&Recreate Thumbnail"),  this, SLOT(recreateThumbnail()),    Qt::Key_F5/*to generate hint*/);
    if(ui->actionUploadToImgaa->isVisible())
        treeContextMenu->addAction(ui->actionUploadToImgaa);
    if(ui->actionUploadToImgmi->isVisible())
        treeContextMenu->addAction(ui->actionUploadToImgmi);
    if(ui->actionUploadToImagevenue->isVisible())
        treeContextMenu->addAction(ui->actionUploadToImagevenue);
    if(ui->actionUploadToKlikr->isVisible())
        treeContextMenu->addAction(ui->actionUploadToKlikr);

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

    /* all dropped files */
    foreach (QUrl files, urls)
    {
        qDebug() << "Processing URL: "<< files;
        QString localFile = files.toLocalFile();
        QFileInfo fi(localFile);
        addRecentFile(localFile);

        if(fi.isDir())
            dir2DirItem(QDir(fi.absoluteFilePath()), profileModel->getCurrentSettingsData().max_dir_depth, true);
        else
            if(fi.isFile())
                fileInfo2DirItem(fi);
    }

    qDebug() << "processingDirs: "<<processingDirs;

    updateRecentFileActions();

    /* all unique directories in tree */
    foreach (QStandardItem *d, processingDirs) {
       if(d && d->hasChildren())
           datamodel->appendRow(d);
    }

    /* all dropped files processed */
    processingDirs.clear();

    /* show tree if dropped video files */
    if(datamodel->rowCount()>0)
    {
        ui->placeholderLabel->hide();
        ui->treeView->show();
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
            QFileInfo f(selIndex.model()->index(0,columnItemNames::path, selIndex).data().toString());

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
void MainWindow::changedProcessingItemsNumber(int delta)
{
    gardian.lock();
    processingItems += delta;
    gardian.unlock();

    refreshStatusBar();
    updateActionState();
}
/******************************************************************************************************/
void MainWindow::recreateThumbnail()
{
    auto selIdx = ui->treeView->selectionModel()->selectedRows();
    QList<QModelIndex> refreshList;

    foreach (auto ix, selIdx)
    {
        if(ix.parent().isValid())
            refreshList.append(ix);
        else
        {
            int i=0;
            QModelIndex fileIx;

            while( (fileIx = ix.model()->index(i++, 0, ix)).isValid())
            {
                if( !refreshList.contains(fileIx))
                    refreshList.append(fileIx);
            }
        }
    }

    QModelIndex pathCell;

    foreach(auto refreshIx, refreshList)
    {
        pathCell = refreshIx.sibling(
                    refreshIx.row(),
                    columnItemNames::path
                    );

        if(!pathCell.data().toString().isEmpty())
            worker->enqueue(
                        datamodel->itemFromIndex(refreshIx.parent()),
                        refreshIx.row());
    }
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
    s.setValue("mainform/toolbarlabels", static_cast<int>(ui->mainToolBar->toolButtonStyle()));

    s.setValue("recentFiles", recentFiles);
    s.setValue("MaxRecentFiles", maxRecentFiles);

    if(ui->imageViewer->isFullScreen())
        ui->imageViewer->close();

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
QStandardItem* MainWindow::dir2DirItem(const QDir dir, const int recursion_depth, const bool topLevel)
{
    QStandardItem *iChildDir, *iDir=nullptr;
    QList<QStandardItem*> iChildren;

    if(recursion_depth > 0)
    {
        qDebug() << "Processing directory:" << dir.absolutePath();
        //Merge toplevel directories
        if(topLevel && processingDirs.contains(dir.absolutePath()))
            iDir = processingDirs[dir.absolutePath()];
        else
        {
            iDir = new QStandardItem(IconProvider::folder(), dir.dirName());
            iDir->setEditable(false);
        }
        //Folders
        foreach (QFileInfo fi, dir.entryInfoList(QDir::Dirs|QDir::NoDotAndDotDot, QDir::Name))
        {
            iChildDir = dir2DirItem(QDir(fi.absoluteFilePath()), recursion_depth-1, false);
            if(iChildDir)
                iChildren.append(iChildDir);
        }
        if(!iChildren.isEmpty())
            iDir->appendRows(iChildren);
        iChildren.clear();

        //Files
        foreach(QFileInfo entry, dir.entryInfoList(QDir::Files, QDir::Name))
            fileInfo2FileItem(entry, iDir);

        if(iDir->hasChildren())
        {
            if(topLevel)
                processingDirs[dir.absolutePath()] = iDir;
        }
        else
        {
            delete iDir;
            iDir=Q_NULLPTR;
        }
    }
    return iDir;
}
/******************************************************************************************************/
QStandardItem* MainWindow::fileInfo2DirItem(QFileInfo file)
{
    QStandardItem *iDir;
    QString absDirPath = file.dir().absolutePath();

    if(processingDirs.contains(absDirPath))
        iDir = processingDirs[absDirPath];
    else
    {
        iDir = new QStandardItem(IconProvider::folder(), file.dir().dirName());
        iDir->setEditable(false);
        processingDirs[absDirPath] = iDir;
    }
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

    qDebug() << "Processing file: "<<file.filePath();
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

    sProfile = new QLabel(s);
    sColumns = new QLabel(s);
    sRows = new QLabel(s);
    sOutput = new QLabel(s);
    sStep = new QLabel(s);
    sSuffix = new QLabel(s);

    sItemsCnt = new QLabel(s);

    sOverwrite = new QCheckBox(QString("%1:").arg(tr("Overwrite")), s);
    sOverwrite->setLayoutDirection(Qt::RightToLeft);
    sOverwrite->setAttribute(Qt::WA_TransparentForMouseEvents);
    sOverwrite->setFocusPolicy(Qt::NoFocus);

    s->addWidget(sProfile);
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

    sProfile->setText(QString("%1: %2 |").arg(tr("Profile"),d.settingsName));
    sColumns->setText(QString("%1: %2 |").arg(tr("Columns"), QString::number(d.columns)));

    sStep->setText(QString("%1: %2s |").arg(tr("Step")).arg(QString::number(d.step)));
    sStep->setVisible(d.rows<=0);

    sRows->setText(QString("%1: %2 |").arg(tr("Rows")).arg(d.rows));
    sRows->setVisible(d.rows>0);

    sOutput->setText(QString("%1: %2 |").arg(tr("Output"), d.output_directory));
    sOutput->setHidden(d.output_directory.isEmpty());

    sSuffix->setText(QString("%1: %2 |").arg(tr("Suffix"), d.suffix));
    sSuffix->setHidden(d.suffix.isEmpty());

    sOverwrite->setChecked(d.overwrite);

    if(processingItems == 0)
        sItemsCnt->clear();
    else
        sItemsCnt->setText(QString("%1: %2").arg(tr("Processing items")).arg(processingItems));
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
    QMessageBox::aboutQt(this, tr("About Qt"));
}
/******************************************************************************************************/
void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::about(this, tr("About..."),
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
            <li>Extracting album art</li>
        </ul>
    </p>
    <p>
    <code>
            2017-2022 &lt;<a href=
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
/*****************************************************************************************************/
void MainWindow::on_actionRemoveItemfromSidebar_triggered()
{
    auto treeModel = ui->treeView->model();
    auto selRows = ui->treeView->selectionModel()->selectedRows();
    QList<QModelIndex>::reverse_iterator sel;

    for (sel = selRows.rbegin(); sel != selRows.rend(); sel++)
    {
        treeModel->removeRow(sel->row(), sel->parent());

        if(sel->parent().isValid() && !treeModel->index(0, 0, sel->parent()).isValid())
            treeModel->removeRow(sel->parent().row());
    }
}
/******************************************************************************************************/
void MainWindow::uploadImage(ImgUp *imgUp)
{
    if(datamodel->rowCount()>0)
    {
        auto selectedIndex = ui->treeView->currentIndex();

        QString imageFileName = selectedIndex.sibling(
                    selectedIndex.row(),
                    columnItemNames::output
                    ).data().toString();

        if(!imageFileName.isEmpty())
        {
            imgUp->setImagePath(imageFileName);
            if(QMessageBox::question(this, tr("Question"), tr("Dou you want to upload file '%1' to '%2'?").arg(imageFileName, imgUp->hostName())) == QMessageBox::Yes)
                imgUp->upload();
        }
        else
            QMessageBox::information(this, tr("Information"), tr("Select a movie item"));
    }
    else
        QMessageBox::information(this, tr("Information"), tr("Nothing to upload"));
}

/******************************************************************************************************/
void MainWindow::updateActionState()
{
    bool allowed = (
        ui->treeView->selectionModel()->selectedIndexes().count() > 0
        &&
        processingItems == 0
    );

    ui->actionRemoveItemfromSidebar->setEnabled(allowed);
    ui->actionRefreshThumbnail->setEnabled(allowed);
}
/******************************************************************************************************/
void MainWindow::on_actionUploadToImgaa_triggered()
{
    uploadImage(new ImgAa(this));
}
/******************************************************************************************************/
void MainWindow::on_actionUploadToImgmi_triggered()
{
    uploadImage(new Imggmi(this));
}
/******************************************************************************************************/
void MainWindow::on_actionUploadToKlikr_triggered()
{
    uploadImage(new Klikr(this));
}
/******************************************************************************************************/
void MainWindow::on_actionUploadToImagevenue_triggered()
{
    uploadImage(new Imagevenue(this));
}
/******************************************************************************************************/
void MainWindow::openRecentFile()
{
    QAction *action = qobject_cast<QAction *>(sender());
    if (action)
        processUrls({QUrl::fromLocalFile(action->data().toString())});

}
/******************************************************************************************************/
void MainWindow::updateRecentFileActions()
{
    int numRecentFiles = qMin(recentFiles.size(), maxRecentFiles);

    for (int i = 0; i < numRecentFiles; ++i) {
        QString text = QString("&%1 %2").arg(i + 1).arg(strippedName(recentFiles[i]));
        recentFileActs[i]->setText(text);
        recentFileActs[i]->setData(recentFiles[i]);
        recentFileActs[i]->setVisible(true);
    }
    for (int j = numRecentFiles; j < maxRecentFiles; ++j)
        recentFileActs[j]->setVisible(false);

    separatorAct->setVisible(numRecentFiles > 0);
}
/******************************************************************************************************/
void MainWindow::addRecentFile(QString fileName)
{
    recentFiles.removeAll(fileName);
    recentFiles.prepend(fileName);

    while(recentFiles.size() > maxRecentFiles)
        recentFiles.removeLast();
}
/******************************************************************************************************/
void MainWindow::createRecentFiles()
{
    for (int i = 0; i < maxRecentFiles; ++i) {
        recentFileActs.append(new QAction(this));
        recentFileActs[i]->setVisible(false);
        connect(recentFileActs[i], SIGNAL(triggered()),
                this, SLOT(openRecentFile()));
    }
}
/******************************************************************************************************/
void MainWindow::createRecentMenu()
{
    ui->menu_File->addSeparator();

    separatorAct = ui->menu_File->addSeparator();

    for (int i = 0; i < maxRecentFiles; ++i)
        ui->menu_File->addAction(recentFileActs[i]);

    updateRecentFileActions();
}
/******************************************************************************************************/
QString MainWindow::strippedName(const QString &fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}
/******************************************************************************************************/
