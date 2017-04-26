#ifndef SETTINGSDIALOG2_H
#define SETTINGSDIALOG2_H

#include <QDialog>

namespace Ui {
class Dialog;
}

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(QWidget *parent = 0);
    ~SettingsDialog();

private:
    Ui::Dialog *ui;
};

#endif // SETTINGSDIALOG2_H
