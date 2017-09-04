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

#include "fileselector.h"
#include <QHBoxLayout>
#include <QFileDialog>

FileSelector::FileSelector(QWidget *parent):QWidget(parent)
{
    edit = new QLineEdit(this);
    QPushButton *button = new QPushButton("...", this);
    QHBoxLayout *l = new QHBoxLayout(this);

    l->setSpacing(2);
    l->setContentsMargins(0,0,0,0);
    l->setMargin(0);

    edit->setReadOnly(true);    

    button->setMaximumWidth(32);
    button->setToolTip(tr(OPENFILETOOLTIP));

    l->addWidget(edit);
    l->addWidget(button);

    connect(button, &QPushButton::clicked, this, &FileSelector::buttonClicked);
}

void FileSelector::buttonClicked(bool /*checked*/)
{
    QString novyText = QFileDialog::getOpenFileName(this, tr(OPENFILETOOLTIP), text());

    if(!novyText.isEmpty())
    {
        setText(novyText);
        emit textChanged(novyText);
    }
}

QString FileSelector::text() const
{
    return FilePath;
}

void FileSelector::setText(const QString &value)
{
    QFileInfo fi(value);

    FilePath = value;
    FileName = fi.fileName();

    edit->setText(FileName);
    edit->setToolTip(FilePath);
}

void FileSelector::setPlaceholderText(QString value)
{
    edit->setPlaceholderText(value);
}
