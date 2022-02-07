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

#ifndef MYBUTTONLINEEDIT_H
#define MYBUTTONLINEEDIT_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>

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
