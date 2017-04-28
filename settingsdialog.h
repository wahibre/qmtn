#ifndef SETTINGSDIALOG2_H
#define SETTINGSDIALOG2_H

#include <QDialog>
#include "mtnworker.h"

namespace Ui {
class Dialog;
}

class SettingsDialog : public QDialog
{
    Q_OBJECT

    SettingsData m_data;

public:
    explicit SettingsDialog(QWidget *parent, SettingsData data);
    ~SettingsDialog();

    SettingsData settingsData();


private slots:
    void on_btnOutputDir_clicked();

private:
    Ui::Dialog *ui;
};

#endif // SETTINGSDIALOG2_H
