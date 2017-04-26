#include <QDropEvent>
#include <QMimeData>
#include <QMimeDatabase>
#include <QFileInfo>

#include "mainwindow.h"
#include "settingsdialog.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)

{
    ui->setupUi(this);
    ui->imageViewer->setTree(ui->treeWidget);

    videoExtensions << "mp4" << "avi" << "mpeg" << "mkv";

    connect(ui->action_Quit, &QAction::triggered, qApp, &QApplication::quit);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::dropEvent(QDropEvent *event)
{
    foreach (QUrl files, event->mimeData()->urls())
    {
        QFileInfo fi(files.toLocalFile());
        QTreeWidgetItem *itmDir=NULL;

        if(fi.isDir())
            itmDir = dirInfoToDirItem(QDir(fi.absoluteFilePath()));
        else
            if(fi.isFile())
                itmDir = fileInfoToDirItem(fi);
            else return;

        if(itmDir && itmDir->childCount()>0)
            ui->treeWidget->addTopLevelItem(itmDir);
    }
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    if(event->mimeData()->hasUrls())
        event->acceptProposedAction();
}

QTreeWidgetItem* MainWindow::dirInfoToDirItem(QDir directory)
{
    QTreeWidgetItem *iChildDir, *iDir = new QTreeWidgetItem();
    iDir->setText(0, directory.dirName());
    iDir->setIcon(0, QIcon::fromTheme("folder"));

    //Folders
    foreach (QFileInfo fi, directory.entryInfoList(QDir::Dirs|QDir::NoDotAndDotDot, QDir::Name))
    {
        iChildDir = dirInfoToDirItem(QDir(fi.absoluteFilePath()));
        if(iChildDir)
        iDir->addChild(iChildDir);
    }

    //Files
    foreach(QFileInfo entry, directory.entryInfoList(QDir::Files, QDir::Name))
    {
        if(isVideoFile(entry))
            iDir->addChild(fileInfoToFileItem(entry));
    }

    if(iDir->childCount()>0)
        return iDir;

    delete iDir;
    return NULL;
}

QTreeWidgetItem* MainWindow::fileInfoToDirItem(QFileInfo file)
{
    QTreeWidgetItem *iDir = new QTreeWidgetItem();
    iDir->setText(0, file.dir().dirName());
    iDir->setIcon(0, QIcon::fromTheme("folder"));
//    iDir->setIcon(0, QIcon::fromTheme("image-loading"));
//    iDir->setIcon(0, QIcon::fromTheme("image-missing"));

    if(isVideoFile(file))
    {
        QTreeWidgetItem *iFile = fileInfoToFileItem(file);
        iDir->addChild(iFile);
    }
    return iDir;
}

QTreeWidgetItem *MainWindow::fileInfoToFileItem(QFileInfo file)
{
    QTreeWidgetItem *iFile = new QTreeWidgetItem();

//    iFile->setIcon(0, QIcon::fromTheme("video-x-generic"));
    iFile->setIcon(0, QIcon::fromTheme("image-loading"));
    iFile->setText(0, file.fileName());
    iFile->setText(1, file.absoluteFilePath());
    worker.enqueue(iFile);
    return iFile;
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
    (new SettingsDialog(this))->exec();
}
