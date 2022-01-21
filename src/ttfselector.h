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

#ifndef TTFSELECTOR_H
#define TTFSELECTOR_H

#if defined(Q_OS_WIN) || defined(WIN32)

#include <QComboBox>

#define OPENFONTTOOLTIP "Open Font"

class TTFSelector : public QWidget
{
    Q_OBJECT

    QComboBox *combo;
    QString FontName;
public:
    static QStringList listOfTTF();
    static QStringList listOfTTF(QStringList dirs);
    explicit TTFSelector(QWidget *parent = nullptr);
    QString text() const;
    void setText(const QString &value);

signals:

private slots:
    void buttonClicked(bool);
};

#else

#include "fileselector.h"
class TTFSelector : public FileSelector
{
public:
    explicit TTFSelector(QWidget *parent = nullptr);
};
#endif


#endif // TTFSELECTOR_H
