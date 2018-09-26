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

#include "ttfselector.h"

#if defined(Q_OS_WIN) || defined(WIN32)
#include <QPushButton>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QStandardPaths>
#include <QAbstractItemView>


TTFSelector::TTFSelector(QWidget *parent) : QWidget(parent)
{
    combo = new QComboBox(this);
    QPushButton *button = new QPushButton("...", this);
    QHBoxLayout *l = new QHBoxLayout(this);

    l->setSpacing(2);
    l->setContentsMargins(0,0,0,0);
    l->setMargin(0);

    button->setMaximumWidth(32);
    button->setToolTip(tr(OPENFONTTOOLTIP));

    l->addWidget(combo);
    l->addWidget(button);

    combo->view()->setTextElideMode(Qt::ElideLeft);
    combo->setEditable(true);
    combo->addItems(listOfTTF());
    combo->setCurrentIndex(-1);

    connect(button, &QPushButton::clicked, this, &TTFSelector::buttonClicked);
}

QString TTFSelector::text() const
{
    return combo->currentText();
}

void TTFSelector::setText(const QString &value)
{
    FontName = value;
    combo->setCurrentText(value);
}

QStringList TTFSelector::listOfTTF()
{
    static QStringList listOfFonts;

    if(listOfFonts.isEmpty())
    {
        listOfFonts = listOfTTF(QStandardPaths::standardLocations(QStandardPaths::FontsLocation));

        if(listOfFonts.isEmpty())
            listOfFonts.append("arial.ttf");
        else
            listOfFonts.sort(Qt::CaseInsensitive);
    }

    return listOfFonts;
}
QStringList TTFSelector::listOfTTF(QStringList dirs)
{
    QStringList listOfFonts;

    foreach(QString directory, dirs)
    {
        QDir fonts(directory);
        if(fonts.exists())
        {
            foreach(QString fontName, fonts.entryList(QStringList("*.ttf"), QDir::Files, QDir::NoSort))
                listOfFonts << fontName;

            listOfFonts << listOfTTF(fonts.entryList(QDir::AllDirs|QDir::NoDotAndDotDot, QDir::NoSort));
        }
    }

    return listOfFonts;
}

void TTFSelector::buttonClicked(bool)
{
    QString novyText = QFileDialog::getOpenFileName(this, tr(OPENFONTTOOLTIP), text());

    if(!novyText.isEmpty())
    {
        setText(novyText);
        //emit textChanged(novyText);
    }
}

#else

TTFSelector::TTFSelector(QWidget *parent) : FileSelector(parent)
{

}

#endif
