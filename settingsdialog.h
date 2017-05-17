#ifndef SETTINGSDIALOG2_H
#define SETTINGSDIALOG2_H

#include <QDialog>
#include "mtnworker.h"

namespace Ui {
class Dialog;
}

//TODO multiple settings profiles
//TODO fit dialog size to content
class SettingsDialog : public QDialog
{
    Q_OBJECT

    Ui::Dialog *ui;
    SettingsData m_data;

    void getUserColor(QColor &c);
    void setBackGroundColor(QPushButton *button, QColor color);
public:
    explicit SettingsDialog(QWidget *parent, SettingsData data);
    ~SettingsDialog();
    SettingsData settingsData();
private slots:
    void on_btnOutputDir_clicked();
    void on_btnBackground_clicked();
    void on_btnForeground_clicked();
    void on_btnTimeColor_clicked();
    void on_btnTimeShadow_clicked();
    void settingsTextChanged(const QString &text);
};

#endif // SETTINGSDIALOG2_H
