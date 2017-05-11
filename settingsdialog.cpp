#include "settingsdialog.h"
#include "ui_settingsdialog.h"
#include <QFileDialog>
#include <QCompleter>
#include <QDirModel>
#include <QColorDialog>
#include <QStandardPaths>

SettingsDialog::SettingsDialog(QWidget *parent, SettingsData data) :
    QDialog(parent),
    ui(new Ui::Dialog),
    m_data(data)
{
    ui->setupUi(this);
    ui->wFontInfo->setPlaceholderText("Choose font file");
    ui->wFontTimestamp->setPlaceholderText("Choose font file");

    QCompleter *completer = new QCompleter(this);
    completer->setModel(new QDirModel(completer));

    ui->eOutputDir->setText(data.output_directory);
    ui->eOutputDir->setCompleter(completer);

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

    ui->eTitle->setText(        data.title);
    ui->groupInfotext->setChecked(      data.infotext);
    ui->groupTimestamp->setChecked(     data.timestamp);

    setBackGroundColor(ui->btnBackground, data.background);
    setBackGroundColor(ui->btnForeground, data.foreground);
    setBackGroundColor(ui->btnTimeColor,  data.timecolor);
    setBackGroundColor(ui->btnTimeShadow, data.timeshadow);

    ui->wFontInfo->setText(             data.fontInfotext);
    ui->wFontTimestamp->setText(        data.fontTimestamp);
    ui->sbFontInfoSize->setValue(       data.fontInfoSize);
    ui->sbFontTimestampSize->setValue(  data.fontTimeSize);

    connect(ui->cbSettingsName, &QComboBox::editTextChanged, this, &SettingsDialog::settingsTextChanged);
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

    data.title            = ui->eTitle->text();
    data.infotext         = ui->groupInfotext->isChecked();
    data.timestamp        = ui->groupTimestamp->isChecked();

//    auto f = ui->fontComboBox->currentFont();
//    auto fi = QFontInfo(f);
//    auto f2 = QFont(fi.family()).rawName();
    //m_data.background     in on_btnBackground_clicked()
    //m_data.foregound      in on_btnForeground_clicked()
    //m_data.timecolor      in on_btnTimeColor_clicked();
    //m_data.timeshadow     in on_btnTimeShadow_clicked();

    data.fontInfotext       = ui->wFontInfo->text();
    data.fontTimestamp      = ui->wFontTimestamp->text();
    data.fontInfoSize       = ui->sbFontInfoSize->value();
    data.fontTimeSize       = ui->sbFontTimestampSize->value();

    return data;
}

void SettingsDialog::on_btnOutputDir_clicked()
{

    QString newDir = QFileDialog::getExistingDirectory(this, "Output directory", ui->eOutputDir->text(), QFileDialog::ShowDirsOnly);
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
