/* Qt Movie Thumbnailer - movie screencast generator

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

#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QJsonArray>
#include "mtnworker.h"
#include "profilemodel.h"

namespace Ui {
class Dialog;
}

class SettingsDialog : public QDialog
{
    Q_OBJECT

    Ui::Dialog *ui;
    SettingsData m_data;
    int m_currIdx;
    ProfileModel *profileModel;

    void getUserColor(QColor &c);
    void setBackGroundColor(QPushButton *button, QColor color);
    void setSettingsData(SettingsData data);
public:
    explicit SettingsDialog(QWidget *parent, ProfileModel *model);
    ~SettingsDialog();
    SettingsData settingsData();
private slots:
    void settingsTextChanged(const QString &text);
    void on_btnOutputDir_clicked();
    void on_btnBackground_clicked();
    void on_btnForeground_clicked();
    void on_btnTimeColor_clicked();
    void on_btnTimeShadow_clicked();
    void on_profilesComboBox_currentIndexChanged(int index);
    void on_btnAddProfile_clicked();
    void on_btnDelProfile_clicked();
    void on_btnExport_clicked();
    void on_btnImport_clicked();

public slots:
    virtual void accept() override;
};

#endif // SETTINGSDIALOG_H
