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

#ifndef SETTINGSDIALOG2_H
#define SETTINGSDIALOG2_H

#include <QDialog>
#include <QJsonArray>
#include "mtnworker.h"

namespace Ui {
class Dialog;
}

//TODO add remove profile button
class SettingsDialog : public QDialog
{
    Q_OBJECT

    Ui::Dialog *ui;
    SettingsData m_data;
    QJsonArray m_dataArray;
    int m_currIdx;

    void getUserColor(QColor &c);
    void setBackGroundColor(QPushButton *button, QColor color);
    QString getSettingsFileName();
    void loadDataFromJSON();
    void saveDataToJSON();
    void setSettingsData(SettingsData data);
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

    // QDialog interface
    void on_cbSettingsName_currentIndexChanged(int);

public slots:
    virtual void accept() override;
};

#endif // SETTINGSDIALOG2_H
