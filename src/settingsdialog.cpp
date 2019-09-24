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

#include "settingsdialog.h"
#include "settingsdata.h"
#include "ui_settingsdialog.h"

#include <QCompleter>
#include <QDirModel>
#include <QFileDialog>
#include <QColorDialog>
#include <QInputDialog>
#include <QMessageBox>
#include <QTextStream>

/******************************************************************************************************/
SettingsDialog::SettingsDialog(QWidget *parent, ProfileModel *model) :
    QDialog(parent),
    ui(new Ui::Dialog),
    profileModel(model)
{
    ui->setupUi(this);
    resize(QSize());
    setWindowFlags(~(~windowFlags()|Qt::WindowMaximizeButtonHint|Qt::WindowMinimizeButtonHint));

    QCompleter *completer = new QCompleter(this);
    completer->setModel(new QDirModel(completer));
    ui->eOutputDir->setCompleter(completer);

    ui->lblMtnExe->setText(QString(MTN_EXE)+":");

    connect(ui->profilesComboBox, &QComboBox::editTextChanged, this, &SettingsDialog::settingsTextChanged);

    connect(ui->eTitle, &QLineEdit::textEdited, [=](const QString&/*newText*/){
        if(!ui->groupInfotext->isChecked())
            ui->groupInfotext->setChecked(true);
    });

    ui->profilesComboBox->setModel(profileModel);
    ui->profilesComboBox->setCurrentIndex(profileModel->getCurrentProfileIdx());
}
/******************************************************************************************************/
SettingsDialog::~SettingsDialog()
{
    delete ui;
}
/******************************************************************************************************/
SettingsData SettingsDialog::settingsData()
{
    SettingsData data;

    data.output_directory = ui->eOutputDir->text();
    data.columns          = ui->sbColumns->value();
    data.rows             = ui->sbRows->value();
    data.width            = ui->sbWidth->value();
    data.gap              = ui->sbGap->value();
    data.overwrite        = ui->overwriteCheckBox->isChecked();

    data.edge_detect      = ui->sbEdgeDetect->value();
    data.blank_skip       = ui->sbBlankSkip->value();
    data.quality          = ui->sbQuality->value();
    data.skip_begin       = ui->sbSkipBegin->value();
    data.skip_end         = ui->sbSkipEnd->value();
    data.suffix           = ui->eSuffix->text();
    data.step             = ui->sbStep->value();
    data.minHeight        = ui->sbHeight->value();
    data.shadowRadius     = ui->sbShadow->value();
    data.verbose          = ui->verboseCheckBox->isChecked();
    data.transparent      = ui->transparentCheckBox->isChecked();

    data.title            = ui->eTitle->text();
    data.infotext         = ui->groupInfotext->isChecked();
    data.timestamp        = ui->groupTimestamp->isChecked();

    // color are set in pushbuttons callback metods
    data.background       = m_data.background;
    data.foreground       = m_data.foreground;
    data.timecolor        = m_data.timecolor;
    data.timeshadow       = m_data.timeshadow;

    data.fontInfotext       = ui->wFontInfo->text();
    data.fontTimestamp      = ui->wFontTimestamp->text();
    data.fontInfoSize       = ui->sbFontInfoSize->value();
    data.fontTimeSize       = ui->sbFontTimestampSize->value();
    data.fontInfoLocation   = ui->cbFontInfoLocation->currentIndex();
    data.fontTimeLocation   = ui->cbFontTimestampLocation->currentIndex();

    data.executable       = ui->eMtnSelector->text();
    data.max_dir_depth    = ui->sbDepth->value();
    data.settingsName       = ui->profilesComboBox->currentText();

    return data;
}
/******************************************************************************************************/
///fill form with data
void SettingsDialog::setSettingsData(SettingsData data)
{
    ui->eOutputDir->setText(data.output_directory);

    ui->sbColumns->setValue(    data.columns);
    ui->sbRows->setValue(       data.rows);
    ui->sbWidth->setValue(      data.width);
    ui->sbGap->setValue(        data.gap);
    ui->overwriteCheckBox->setChecked(data.overwrite);

    ui->sbEdgeDetect->setValue( data.edge_detect);
    ui->sbBlankSkip->setValue(  data.blank_skip);
    ui->sbQuality->setValue(    data.quality);
    ui->sbSkipBegin->setValue(  data.skip_begin);
    ui->sbSkipEnd->setValue(    data.skip_end);
    ui->eSuffix->setText(       data.suffix);
    ui->sbStep->setValue(       data.step);
    ui->sbHeight->setValue(     data.minHeight);
    ui->sbShadow->setValue(     data.shadowRadius);
    ui->verboseCheckBox->setChecked(data.verbose);
    ui->transparentCheckBox->setChecked(data.transparent);

    ui->eTitle->setText(        data.title);
    ui->groupInfotext->setChecked(      data.infotext);
    ui->groupTimestamp->setChecked(     data.timestamp);

    setBackGroundColor(ui->btnBackground, data.background); m_data.background=data.background;
    setBackGroundColor(ui->btnForeground, data.foreground); m_data.foreground=data.foreground;
    setBackGroundColor(ui->btnTimeColor,  data.timecolor);  m_data.timecolor=data.timecolor;
    setBackGroundColor(ui->btnTimeShadow, data.timeshadow); m_data.timeshadow=data.timeshadow;

    ui->wFontInfo->setText(                         data.fontInfotext);
    ui->wFontTimestamp->setText(                    data.fontTimestamp);
    ui->sbFontInfoSize->setValue(                   data.fontInfoSize);
    ui->sbFontTimestampSize->setValue(              data.fontTimeSize);
    ui->cbFontInfoLocation->setCurrentIndex(        data.fontInfoLocation);
    ui->cbFontTimestampLocation->setCurrentIndex(   data.fontTimeLocation);

    ui->eMtnSelector->setText(data.executable);
    ui->sbDepth->setValue(data.max_dir_depth);
}
/******************************************************************************************************/
void SettingsDialog::on_btnOutputDir_clicked()
{
    QString newDir = QFileDialog::getExistingDirectory(this, tr("Output directory"), ui->eOutputDir->text(), QFileDialog::ShowDirsOnly);
    if(!newDir.isEmpty())
        ui->eOutputDir->setText(newDir);
}
/******************************************************************************************************/
void SettingsDialog::getUserColor(QColor &c)
{
    QColor cTmp;

    if(c.isValid())
        cTmp = QColor(c);

    cTmp = QColorDialog::getColor(cTmp, this);

    if(cTmp.isValid())
        c = cTmp;
}
/******************************************************************************************************/
void SettingsDialog::setBackGroundColor(QPushButton *button, QColor color)
{
    button->setStyleSheet(QString("background-color: %1").arg(color.name()));
}
/******************************************************************************************************/
void SettingsDialog::on_btnBackground_clicked()
{
    getUserColor(m_data.background);
    setBackGroundColor(ui->btnBackground, m_data.background);
}
/******************************************************************************************************/
void SettingsDialog::on_btnForeground_clicked()
{
    getUserColor(m_data.foreground);
    setBackGroundColor(ui->btnForeground, m_data.foreground);
}
/******************************************************************************************************/
void SettingsDialog::on_btnTimeColor_clicked()
{
    getUserColor(m_data.timecolor);
    setBackGroundColor(ui->btnTimeColor, m_data.timecolor);
}
/******************************************************************************************************/
void SettingsDialog::on_btnTimeShadow_clicked()
{
    getUserColor(m_data.timeshadow);
    setBackGroundColor(ui->btnTimeShadow, m_data.timeshadow);
}
/******************************************************************************************************/
/// Do not allow empty settings name
void SettingsDialog::settingsTextChanged(const QString &text)
{
    QFont f = QApplication::font(ui->lblProfile);

    if(text.isEmpty())
    {
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
        f.setBold(true);
        ui->lblProfile->setFont(f);
    }
    else
    {
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
        f.setBold(false);
        ui->lblProfile->setFont(f);
    }
}
/******************************************************************************************************/
void SettingsDialog::on_profilesComboBox_currentIndexChanged(int index)
{
    setSettingsData(profileModel->data(profileModel->index(index, 1), Qt::DisplayRole).value<SettingsData>());
}
/******************************************************************************************************/
void SettingsDialog::accept()
{
    profileModel->setData(profileModel->index(ui->profilesComboBox->currentIndex(), 1), QVariant::fromValue<SettingsData>(settingsData()), Qt::EditRole);
    profileModel->setCurrentProfileIdx(ui->profilesComboBox->currentIndex());
    QDialog::accept();
}
/******************************************************************************************************/
void SettingsDialog::on_btnAddProfile_clicked()
{
    bool Ok=false;
    QString newProfile = QInputDialog::getText(this, tr("Profile"), tr("Name:"), QLineEdit::Normal, QString(), &Ok);

    if(Ok)
    {
        SettingsData s = settingsData();
        s.settingsName = newProfile;
        int currRow = profileModel->rowCount();

        ui->profilesComboBox->setInsertPolicy(QComboBox::InsertAfterCurrent);
        profileModel->insertRow(currRow);
        if(profileModel->setData(profileModel->index(currRow, 1), QVariant::fromValue<SettingsData>(s), Qt::EditRole))
            ui->profilesComboBox->setCurrentIndex(currRow);
        ui->profilesComboBox->setInsertPolicy(QComboBox::NoInsert);     //disable manual inserting with ENTER
    }
}
/******************************************************************************************************/
void SettingsDialog::on_btnDelProfile_clicked()
{
    if(profileModel->rowCount()>1)
    {
        if(QMessageBox::question(this, tr("Delete profile"), tr("Do you really want to remove \"%1\"?").arg(ui->profilesComboBox->currentText()))
                == QMessageBox::Yes)
        {
                if(profileModel->removeRow(ui->profilesComboBox->currentIndex()))
                    ui->profilesComboBox->setCurrentIndex(0);
        }
    }
}
/******************************************************************************************************/
void SettingsDialog::on_btnExport_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"));

    if(!fileName.isEmpty())
    {
        QFile f(fileName);

        if(f.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            QTextStream ts(&f);
            ts << settingsData().toByteArray();
            f.close();
        }
    }
}
/******************************************************************************************************/
void SettingsDialog::on_btnImport_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"));

    if(!fileName.isEmpty())
    {
        QFile f(fileName);

        if(f.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QTextStream ts(&f);
            SettingsData loadedSettings;
            QString errorText;
            bool ok=false;

            QByteArray bytes = ts.readAll().toLatin1();

            f.close();

            loadedSettings = SettingsData::SettingsDataFromByteArray(bytes, ok, errorText);
            if(ok)
                setSettingsData(loadedSettings);
            else
                QMessageBox::warning(this,
                         QObject::tr("Error"),
                         QString("%1\n\n  %2").arg(QObject::tr("Error loading settings!")).arg(errorText)
                    );
        }
    }
}
/******************************************************************************************************/
