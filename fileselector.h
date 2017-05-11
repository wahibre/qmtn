#ifndef MYBUTTONLINEEDIT_H
#define MYBUTTONLINEEDIT_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>

#define OPENFILETOOLTIP "Open File"

class FileSelector : public QWidget
{
    Q_OBJECT

    QLineEdit *edit;
    QString FileName, FilePath;
public:
    FileSelector(QWidget *parent);
    QString text() const;
    void setText(const QString &value);
    void setPlaceholderText(QString value);

private slots:
    void buttonClicked(bool);
signals:
    void textChanged(QString&);
};

#endif // MYBUTTONLINEEDIT_H
