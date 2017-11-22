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

#ifndef MTNWORKER_H
#define MTNWORKER_H

#include <QTreeWidgetItem>
#include <QThreadPool>
#include <QMutex>
#include <QStandardItem>
#include "settingsdata.h"

class MtnWorker: public QObject
{
    Q_OBJECT

    QMutex mutex;
    SettingsPool settingsPool;
public:
    MtnWorker();
    ~MtnWorker();
    void dataLoad();
    void dataSave();

    SettingsData currentSettings();
    SettingsPool &allSettings();
    QString outputFile(QString inputfilename);

    void enqueue(QStandardItem* parent, int row);

signals:
    void changedProcessingItemsNumber(int delta);
    void generatingSuccess(QStandardItem*, int);
public slots:
    void jobFinished(QStandardItem *parent, int row, bool success, QString logtexttext, QString outFileName);
};

#endif // MTNWORKER_H
