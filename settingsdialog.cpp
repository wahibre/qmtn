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
#include <QFileDialog>
#include <QCompleter>
#include <QDirModel>
#include <QColorDialog>
#include <QJsonArray>
#include <QJsonDocument>
#include <QTextStream>
#include <QDebug>

#define SETTINGS_FILE "settings.json"

SettingsDialog::SettingsDialog(QWidget *parent, SettingsData data) :
    QDialog(parent),
    ui(new Ui::Dialog),
    m_data(data)
{
    ui->setupUi(this);
    setWindowFlags(~(~windowFlags()|Qt::WindowMaximizeButtonHint|Qt::WindowMinimizeButtonHint));

    ui->wFontInfo->setPlaceholderText(tr("Choose font file"));
    ui->wFontTimestamp->setPlaceholderText(tr("Choose font file"));

    QCompleter *completer = new QCompleter(this);
    completer->setModel(new QDirModel(completer));
    ui->eOutputDir->setCompleter(completer);

    ui->lblMtnExe->setText(QString(MTN_EXE)+":");

    connect(ui->cbSettingsName, &QComboBox::editTextChanged, this, &SettingsDialog::settingsTextChanged);

    loadDataFromJSON();

    foreach(QJsonValue o, m_dataArray)
        ui->cbSettingsName->addItem(o.toObject()[REG_SETTINGSNAME].toString());

    ui->cbSettingsName->setCurrentIndex(m_currIdx);
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

SettingsData SettingsDialog::settingsData()
{
    SettingsData data = m_data;

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
    data.verbose          = ui->verboseCheckBox->isChecked();

    data.title            = ui->eTitle->text();
    data.infotext         = ui->groupInfotext->isChecked();
    data.timestamp        = ui->groupTimestamp->isChecked();

    //m_data.background     in on_btnBackground_clicked()
    //m_data.foregound      in on_btnForeground_clicked()
    //m_data.timecolor      in on_btnTimeColor_clicked();
    //m_data.timeshadow     in on_btnTimeShadow_clicked();

    data.fontInfotext       = ui->wFontInfo->text();
    data.fontTimestamp      = ui->wFontTimestamp->text();
    data.fontInfoSize       = ui->sbFontInfoSize->value();
    data.fontTimeSize       = ui->sbFontTimestampSize->value();
    data.fontInfoLocation   = ui->cbFontInfoLocation->currentIndex();
    data.fontTimeLocation   = ui->cbFontTimestampLocation->currentIndex();

    data.executable       = ui->eMtnSelector->text();
    data.max_dir_depth    = ui->sbDepth->value();
    data.settingsName       = ui->cbSettingsName->currentText();

    return data;
}

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
    ui->verboseCheckBox->setChecked(data.verbose);

    ui->eTitle->setText(        data.title);
    ui->groupInfotext->setChecked(      data.infotext);
    ui->groupTimestamp->setChecked(     data.timestamp);

    setBackGroundColor(ui->btnBackground, data.background);
    setBackGroundColor(ui->btnForeground, data.foreground);
    setBackGroundColor(ui->btnTimeColor,  data.timecolor);
    setBackGroundColor(ui->btnTimeShadow, data.timeshadow);

    ui->wFontInfo->setText(                         data.fontInfotext);
    ui->wFontTimestamp->setText(                    data.fontTimestamp);
    ui->sbFontInfoSize->setValue(                   data.fontInfoSize);
    ui->sbFontTimestampSize->setValue(              data.fontTimeSize);
    ui->cbFontInfoLocation->setCurrentIndex(        data.fontInfoLocation);
    ui->cbFontTimestampLocation->setCurrentIndex(   data.fontTimeLocation);

    ui->eMtnSelector->setText(data.executable);
    ui->sbDepth->setValue(data.max_dir_depth);
}

void SettingsDialog::loadDataFromJSON()
{
    QString filename = getSettingsFileName();

    if(!filename.isEmpty() && QFile::exists(filename))
    {
        QFile settFile(filename);
        if(settFile.open(QIODevice::ReadOnly))
        {
            QByteArray data = settFile.readAll();
            QJsonDocument doc = QJsonDocument::fromJson(data);

            if(doc.isObject())
            {
                m_currIdx = doc.object()["CurrentSetting"].toInt(0);
                m_dataArray = doc.object()["SettingsList"].toArray();
            }

            if(m_dataArray.count() == 0)
                m_dataArray.append(SettingsData(QJsonObject()).toJsonObject());
        }
        else
            qCritical() << tr("Cannon open file %1 for reading!").arg(filename);
    }
    else
    {
        m_currIdx = 0;
        m_dataArray.append(SettingsData(QJsonObject()).toJsonObject());
    }
}

void SettingsDialog::saveDataToJSON()
{
    while(ui->cbSettingsName->count()>m_dataArray.count())
        m_dataArray.append(settingsData().toJsonObject());

    m_dataArray.replace(ui->cbSettingsName->currentIndex(), settingsData().toJsonObject());

    QJsonObject jsonOut = {
        {"CurrentSetting", ui->cbSettingsName->currentIndex()},
        {"SettingsList", m_dataArray}
    };

    QJsonDocument doc(jsonOut);

    QByteArray arr = doc.toJson(QJsonDocument::Indented);

    QString filename = getSettingsFileName();
    if(!filename.isEmpty())
    {
        QFile settfile(filename);
        if(settfile.open(QIODevice::WriteOnly))
        {
            QTextStream settstream(&settfile);
            settstream << arr;
            settfile.close();
        }
        else
            qCritical() << tr("Cannot open file %1!").arg(settfile.fileName());
    }
}

QString SettingsDialog::getSettingsFileName()
{
    QString settdir = QStandardPaths::writableLocation(QStandardPaths::DataLocation);

    if(!settdir.isEmpty())
    {
        QDir dir(settdir);

        if(!dir.exists())
        {
            if(!dir.mkpath(settdir))
            {
                qCritical() << tr("Cannot create DataLocation %1!").arg(settdir);
                return QString();
            }
        }

        QFileInfo settfileinfo(settdir, SETTINGS_FILE);
        return settfileinfo.absoluteFilePath();
    }
    else
        qCritical() << tr("Cannot find DataLocation and thus store settigns!");

    return QString();
}

void SettingsDialog::on_btnOutputDir_clicked()
{

    QString newDir = QFileDialog::getExistingDirectory(this, tr("Output directory"), ui->eOutputDir->text(), QFileDialog::ShowDirsOnly);
    if(!newDir.isEmpty())
        ui->eOutputDir->setText(newDir);
}

void SettingsDialog::getUserColor(QColor &c)
{
    QColor cTmp;

    if(c.isValid())
        cTmp = QColor(c);

    cTmp = QColorDialog::getColor(cTmp, this);

    if(cTmp.isValid())
        c = cTmp;
}

void SettingsDialog::setBackGroundColor(QPushButton *button, QColor color)
{
    button->setStyleSheet(QString("background-color: %1").arg(color.name()));
}

void SettingsDialog::on_btnBackground_clicked()
{
    getUserColor(m_data.background);
    setBackGroundColor(ui->btnBackground, m_data.background);
}

void SettingsDialog::on_btnForeground_clicked()
{
    getUserColor(m_data.foreground);
    setBackGroundColor(ui->btnForeground, m_data.foreground);
}

void SettingsDialog::on_btnTimeColor_clicked()
{
    getUserColor(m_data.timecolor);
    setBackGroundColor(ui->btnTimeColor, m_data.timecolor);
}

void SettingsDialog::on_btnTimeShadow_clicked()
{
    getUserColor(m_data.timeshadow);
    setBackGroundColor(ui->btnTimeShadow, m_data.timeshadow);
}

/// Do not allow empty settings name
void SettingsDialog::settingsTextChanged(const QString &text)
{
    QFont f = QApplication::font(ui->cbSettingsName);

    if(text.isEmpty())
    {
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
        f.setBold(true);
        ui->cbSettingsName->setFont(f);
    }
    else
    {
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
        f.setBold(false);
        ui->cbSettingsName->setFont(f);
    }
}

void SettingsDialog::on_cbSettingsName_currentIndexChanged(int /*index*/)
{
    while(ui->cbSettingsName->count()>m_dataArray.count())
        m_dataArray.append(settingsData().toJsonObject());

    setSettingsData(SettingsData(m_dataArray[ui->cbSettingsName->currentIndex()].toObject()));
}

void SettingsDialog::accept()
{
    if(!ui->cbSettingsName->hasFocus())
    {
        saveDataToJSON();
        QDialog::accept();
    }
}
