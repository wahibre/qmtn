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

#include "mtnworker.h"
#include "mtnjob.h"
#include "iconprovider.h"

MtnWorker::MtnWorker()
{
    dataLoad();
}

MtnWorker::~MtnWorker()
{
    dataSave();
}

void MtnWorker::dataLoad()
{
    settingsPool.loadFromJSON();
}

void MtnWorker::dataSave()
{
    settingsPool.saveToJSON();
}

SettingsData MtnWorker::currentSettings()
{
    return settingsPool.currentSettings();
}

SettingsPool& MtnWorker::allSettings()
{
    return settingsPool;
}

QString MtnWorker::outputFile(const QString inputfilename)
{
    QString filename;
    QDir directory;

    if (inputfilename.isEmpty())
        return QString();

    if(settingsPool.currentSettings().output_directory.isEmpty())
    {
        directory = QFileInfo(inputfilename).dir();
    }
    else
    {
        directory = QDir(settingsPool.currentSettings().output_directory);
    }

    if(settingsPool.currentSettings().suffix.isEmpty())
    {
        QFileInfo f = QFileInfo(inputfilename);
        filename = f.completeBaseName() + "_s.jpg";
    }
    else
    {
        QFileInfo f = QFileInfo(inputfilename);
        filename = f.completeBaseName() + settingsPool.currentSettings().suffix;
    }

    return QFileInfo(directory, filename).absoluteFilePath();

}

void MtnWorker::enqueue(QStandardItem* parent, int row)
{
    if(parent && parent->child(row))
    {
        QString outfile = outputFile(parent->child(row, columnItemNames::path)->text());

        emit changedProcessingItemsNumber(+1);
        QThreadPool::globalInstance()->start(new MtnJob(this, parent, row, settingsPool.currentSettings(), outfile));
    }
}

void MtnWorker::jobFinished(QStandardItem* parent, int row, bool success, QString log, QString outFileName=QString())
{
    mutex.lock();
    if(success)
    {
        parent->child(row, columnItemNames::filename)->setIcon(IconProvider::video());
        parent->child(row, columnItemNames::logtext)->setText(log);
        parent->child(row, columnItemNames::output)->setText(outFileName);
    }
    else
    {
        parent->child(row, columnItemNames::filename)->setIcon(IconProvider::error());
        parent->child(row, columnItemNames::logtext)->setText(log);
    }
    emit changedProcessingItemsNumber(-1);
    mutex.unlock();
}
