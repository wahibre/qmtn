#include <QDropEvent>
#include <QMimeData>
#include <QMimeDatabase>
#include <QFileInfo>
#include <QSettings>
#include <QMessageBox>
#include <QDesktopServices>

#include "mainwindow.h"
#include "settingsdialog.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)

{
    ui->setupUi(this);
    setWindowTitle(QString("%1 (%2)").arg(qApp->applicationName()).arg(LAST_TAG));

    datamodel = new QStandardItemModel(this);
    datamodel->setColumnCount(4);

    ui->treeView->hide();
    ui->treeView->setModel(datamodel);
    ui->treeView->header()->hideSection(1);
    ui->treeView->header()->hideSection(2);
    ui->treeView->header()->hideSection(3);

    ui->imageViewer->setModel(ui->treeView->selectionModel());
    ui->imageViewer->addAction(ui->actionShowImage);

    //TODO add alternative for WIN Icon from theme
    ui->action_Settings->setIcon(QIcon::fromTheme("applications-system"));
    ui->action_Quit->setIcon(QIcon::fromTheme("application-exit"));

    videoExtensions << "mp4" << "avi" << "mpeg" << "mkv";

    connect(ui->action_Quit, &QAction::triggered, this, &MainWindow::close);
    connect(ui->treeView->selectionModel(), &QItemSelectionModel::currentRowChanged, this, &MainWindow::currentRowChanged);
    connect(ui->treeView, &QTreeView::customContextMenuRequested, this, &MainWindow::treeContextMenuRequest);

    QSettings s;
    restoreGeometry(s.value("mainform/geometry").toByteArray());
    restoreState(s.value("mainform/state").toByteArray());
    ui->splitter->restoreState(s.value("mainform/splitter").toByteArray());

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::currentRowChanged(const QModelIndex &current, const QModelIndex &/*previous*/)
{
    QString log;

    log = current.sibling(current.row(), 2).data().toString();
    ui->logText->setPlainText(log);
}

void MainWindow::treeContextMenuRequest(const QPoint &pos)
{
    auto treeContextMenu = new QMenu(this);

    treeContextMenu->addAction(ICON_FOLDER, "Open Directory", this, SLOT(openDirectory()));
    treeContextMenu->exec(ui->treeView->mapToGlobal(pos));
}

void MainWindow::openDirectory()
{
    //TODO Open directory for directory item
    if(datamodel->rowCount()>0)
    {
        auto s = ui->treeView->currentIndex().sibling(
                    ui->treeView->currentIndex().row(),
                    1
                    ).data().toString();

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
}


void MainWindow::closeEvent(QCloseEvent *event)
{
    QSettings s;

    s.setValue("mainform/geometry", saveGeometry());
    s.setValue("mainform/state", saveState());
    s.setValue("mainform/splitter", ui->splitter->saveState());

    QMainWindow::closeEvent(event);
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    if(event->mimeData()->hasUrls())
        event->acceptProposedAction();
}

void MainWindow::dropEvent(QDropEvent *event)

{
    QStandardItem *iDir;

    // All droped files
    foreach (QUrl files, event->mimeData()->urls())
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

QStandardItem* MainWindow::dir2DirItem(QDir dir)
{
    QStandardItem *iChildDir, *iDir;
    QList<QStandardItem*> iChildren;

    iDir = new QStandardItem(ICON_FOLDER, dir.dirName());
    iDir->setEditable(false);

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

QStandardItem* MainWindow::fileInfo2DirItem(QFileInfo file)
{
    QStandardItem *iDir;

    iDir = new QStandardItem(ICON_FOLDER, file.dir().dirName());
    iDir->setEditable(false);
    fileInfo2FileItem(file, iDir);

    return iDir;
}

/**
 * @return true if fileitem created, othewise false
 */
bool MainWindow::fileInfo2FileItem(QFileInfo file, QStandardItem *parent)
{
    QStandardItem *iFile, *iAbsFile, *iLog, *iOutputFile;

    if(isVideoFile(file))
    {
        iFile = new QStandardItem(ICON_LOADING, file.fileName());   iFile->setEditable(false);
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

bool MainWindow::isVideoFile(QFileInfo file)
{
    /* according to extension */
    return (videoExtensions.contains(file.suffix(), Qt::CaseInsensitive));

    /* according to mime type
    QMimeDatabase mimedb;
    QMimeType mType = mimedb.mimeTypeForFile(entry, QMimeDatabase::MatchContent);
    */
}

void MainWindow::on_action_Settings_triggered()
{
    SettingsDialog *dial = new SettingsDialog(this, worker.data());

    if(dial->exec() == QDialog::Accepted)
    {
        worker.setData(dial->settingsData());
    }
}

void MainWindow::on_actionAboutQt_triggered()
{
    QMessageBox::aboutQt(this, "About Qt");
}

void MainWindow::on_actionShowImage_triggered()
{
    QDesktopServices::openUrl(QUrl::fromLocalFile(ui->imageViewer->imagePath()));
}
