#include <QDropEvent>
#include <QMimeData>
#include <QMimeDatabase>
#include <QFileInfo>
#include <QSettings>
#include <QMessageBox>
#include <QDesktopServices>
#include <QFileDialog>
#include <QVersionNumber>

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
    setWindowTitle(QString("%1 (%2)").arg(qApp->applicationName()).arg(QVersionNumber::fromString(QString(VERSION_FROM_GIT_TAG).replace('-','.')).toString()));

    datamodel = new QStandardItemModel(this);
    datamodel->setColumnCount(4);

    ui->treeView->hide();
    ui->treeView->setModel(datamodel);
    ui->treeView->header()->hideSection(columnItemNames::path);
    ui->treeView->header()->hideSection(columnItemNames::log);
    ui->treeView->header()->hideSection(columnItemNames::output);

    ui->imageViewer->setModel(ui->treeView->selectionModel());


    //TODO add alternative Icon from theme for WIN
    ui->action_Settings->setIcon(IconProvider::system());
    ui->action_Quit->setIcon(IconProvider::exit());
    ui->actionAbout->setIcon(IconProvider::help());
    ui->actionAboutQt->setIcon(IconProvider::help());
    ui->TabOutput->setTabIcon(0, IconProvider::image());
    ui->TabOutput->setTabIcon(1, IconProvider::text());
    ui->actionOpenDirectory->setIcon(IconProvider::folder());
    ui->actionOpenFile->setIcon(IconProvider::video());

    videoExtensions << "mp4" << "avi" << "mpeg" << "mkv" << "wmv" << "asf";

    connect(ui->action_Quit, &QAction::triggered, this, &MainWindow::close);
    connect(ui->treeView->selectionModel(), &QItemSelectionModel::currentRowChanged, this, &MainWindow::currentRowChanged);
    connect(ui->treeView, &QTreeView::customContextMenuRequested, this, &MainWindow::treeContextMenuRequest);
    connect(&worker, &MtnWorker::changedProcessingItemsNumber, this, &MainWindow::changedProcessingItemsNumber);

    QSettings s;
    restoreGeometry(s.value("mainform/geometry").toByteArray());
    restoreState(s.value("mainform/state").toByteArray());
    ui->splitter->restoreState(s.value("mainform/splitter").toByteArray());

    worker.findExecutable();

    createStatusBarWidgets();
    refreshStatusBar();
}
/******************************************************************************************************/
MainWindow::~MainWindow()
{
    delete ui;
}
/******************************************************************************************************/
void MainWindow::currentRowChanged(const QModelIndex &current, const QModelIndex &/*previous*/)
{
    QString log;

    log = current.sibling(current.row(), columnItemNames::log).data().toString();
    ui->logText->setPlainText(log);
}
/******************************************************************************************************/
void MainWindow::treeContextMenuRequest(const QPoint &pos)
{
    auto treeContextMenu = new QMenu(this);

    treeContextMenu->addAction(IconProvider::folder(),  "&Open Directory",      this, SLOT(openDirectory()));
    treeContextMenu->addAction(IconProvider::refresh(), "&Recreate Thumbnail",  this, SLOT(recreateThumbnail()));
    treeContextMenu->exec(ui->treeView->mapToGlobal(pos));
}
/******************************************************************************************************/
void MainWindow::processUrls(QList<QUrl> urls)
{
    //TODO merge multiple files
    //TODO add support for external URL (SMB, ...)


    QStandardItem *iDir;

    foreach (QUrl files, urls)
    {
        iDir=NULL;
        QFileInfo fi(files.toLocalFile());

        if(fi.isDir())
            iDir = dir2DirItem(QDir(fi.absoluteFilePath()));
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
void MainWindow::openDirectory()
{
    if(datamodel->rowCount()>0)
    {
        QModelIndex selIndex = ui->treeView->currentIndex();

        QString s = selIndex.sibling(
                    selIndex.row(),
                    columnItemNames::path
                    ).data().toString();

        // File Item
        if(!s.isEmpty())
        {
            QFileInfo f(s);
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
void MainWindow::recreateThumbnail(const QModelIndex selIndex)
{
    QModelIndex pathCell = selIndex.sibling(
                selIndex.row(),
                columnItemNames::path       /* empty for directories */
                );

    // File Item
    if(!pathCell.data().toString().isEmpty())
        worker.enqueue(datamodel->itemFromIndex(selIndex.parent()), selIndex.row());
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
QStandardItem* MainWindow::dir2DirItem(QDir dir)
{
    QStandardItem *iChildDir, *iDir;
    QList<QStandardItem*> iChildren;

    iDir = new QStandardItem(IconProvider::folder(), dir.dirName()); iDir->setEditable(false);

    //Folders
    foreach (QFileInfo fi, dir.entryInfoList(QDir::Dirs|QDir::NoDotAndDotDot, QDir::Name))
    {
        iChildDir = dir2DirItem(QDir(fi.absoluteFilePath()));
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

        worker.enqueue(parent, parent->rowCount()-1);

        return true;
    }
    return false;
}
/******************************************************************************************************/
bool MainWindow::isVideoFile(QFileInfo file)
{
    /* according to extension */
    return (videoExtensions.contains(file.suffix(), Qt::CaseInsensitive));

    /* according to mime type
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
    auto d = worker.data();

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
    SettingsDialog *dial = new SettingsDialog(this, worker.data());

    if(dial->exec() == QDialog::Accepted)
    {
        worker.setData(dial->settingsData());
        refreshStatusBar();
    }
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
    <p>Movie Thumbnailer for creating thumbnails is frontend of great <a href="http://moviethumbnail.sourceforge.net">mtn</a>.</p>
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
