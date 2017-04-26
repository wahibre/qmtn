#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "mtnworker.h"
#include <QMainWindow>
#include <QTreeWidgetItem>
#include <QDir>
#include <QUrl>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

    QDir minDirectory;
    MtnWorker worker;
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


private slots:
    void on_action_Settings_triggered();

private:
    Ui::MainWindow *ui;
    QTreeWidgetItem* dirInfoToDirItem(QDir directory);
    QTreeWidgetItem* fileInfoToDirItem(QFileInfo file);
    QTreeWidgetItem* fileInfoToFileItem(QFileInfo file);
    QStringList videoExtensions;

    bool isVideoFile(QFileInfo file);
    void dropEvent(QDropEvent *event) Q_DECL_OVERRIDE;
    void dragEnterEvent(QDragEnterEvent * event) Q_DECL_OVERRIDE;
};

#endif // MAINWINDOW_H
