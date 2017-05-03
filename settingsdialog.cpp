#include "settingsdialog.h"
#include "ui_settingsdialog.h"
#include <QFileDialog>
#include <QCompleter>
#include <QDirModel>

SettingsDialog::SettingsDialog(QWidget *parent, SettingsData data) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);

    QCompleter *completer = new QCompleter(this);
    completer->setModel(new QDirModel(completer));

    //TODO add validator
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
    ui->eSuffix->setText(       data.suffix);
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

SettingsData SettingsDialog::settingsData()
{
    SettingsData m_data;

    m_data.output_directory = ui->eOutputDir->text();
    m_data.columns          = ui->sbColumns->value();
    m_data.rows             = ui->sbRows->value();
    m_data.width            = ui->sbWidth->value();
    m_data.gap              = ui->sbGap->value();
    m_data.overwrite        = ui->overwriteCheckBox->isChecked();

    m_data.edge_detect      = ui->sbEdgeDetect->value();
    m_data.blank_skip       = ui->sbBlankSkip->value();
    m_data.quality          = ui->sbQuality->value();
    m_data.suffix           = ui->eSuffix->text();


    return m_data;
}

void SettingsDialog::on_btnOutputDir_clicked()
{

    QString newDir = QFileDialog::getExistingDirectory(this, "Output directory", ui->eOutputDir->text(), QFileDialog::ShowDirsOnly);
    if(!newDir.isEmpty())
        ui->eOutputDir->setText(newDir);
}
