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

#include <QSettings>
#include <QDir>
#include <QStandardPaths>
#include <QCoreApplication>
#include <QMutexLocker>

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
    QSettings s;

    /// mtn settings
    settingsData.output_directory = s.value(REG_OUTPUT_DIRECTORY, QString()).toString();

    settingsData.columns        = s.value(REG_COLUMNS,   4      ).toInt();
    settingsData.rows           = s.value(REG_ROWS,      8      ).toInt();
    settingsData.width          = s.value(REG_WIDTH,     1920   ).toInt();
    settingsData.gap            = s.value(REG_GAP,       3      ).toInt();
    settingsData.overwrite      = s.value(REG_OVERWRITE, true   ).toBool();
    settingsData.suffix         = s.value(REG_SUFFIX            ).toString();

    settingsData.edge_detect    = s.value(REG_EDGE,      12     ).toInt();
    settingsData.blank_skip     = s.value(REG_BLANK,     0.80   ).toDouble();
    settingsData.quality        = s.value(REG_QUALITY,   90     ).toInt();
    settingsData.skip_begin     = s.value(REG_SKIPBEGIN, 0.0    ).toReal();
    settingsData.skip_end       = s.value(REG_SKIPEND,   0.0    ).toReal();
    settingsData.step           = s.value(REG_STEP,      0      ).toInt();
    settingsData.minHeight      = s.value(REG_MINHEIGHT, 0      ).toInt();
    settingsData.verbose        = s.value(REG_VERBOSE,  false   ).toBool();

    settingsData.title          = s.value(REG_TITLE             ).toString();
    settingsData.infotext       = s.value(REG_INFOTEXT,  true   ).toBool();
    settingsData.timestamp      = s.value(REG_TIMESTAMP, true   ).toBool();

    settingsData.background.setNamedColor(s.value(REG_BACKGROUND,   QColor(Qt::black)).toString());
    settingsData.foreground.setNamedColor(s.value(REG_FOREGROUND,   QColor(Qt::white)).toString());
    settingsData.timecolor.setNamedColor(s.value(REG_TIMECOLOR,     QColor(Qt::black)).toString());
    settingsData.timeshadow.setNamedColor(s.value(REG_TIMESHADOW,   QColor(Qt::gray)) .toString());

    settingsData.fontInfotext   = s.value(REG_FONTTEXT          ).toString();
    settingsData.fontTimestamp  = s.value(REG_FONTTIME          ).toString();
    settingsData.fontInfoSize   = s.value(REG_FONTTEXTSIZE, 0   ).toInt();
    settingsData.fontTimeSize   = s.value(REG_FONTTIMESIZE, 0   ).toInt();
    settingsData.fontInfoLocation = s.value(REG_FONTTEXTLOCATION, -1).toInt();
    settingsData.fontTimeLocation = s.value(REG_FONTTIMELOCATION, -1).toInt();

    /// qmtn settings
    settingsData.executable     = s.value(REG_MTN, findExecutableMtn()).toString();
    settingsData.max_dir_depth  = s.value(REG_MAXDIRDEPTH, 999).toInt();
}

void MtnWorker::dataSave()
{
    QSettings s;

    s.setValue(REG_OUTPUT_DIRECTORY, settingsData.output_directory);

    s.setValue(REG_COLUMNS,         settingsData.columns);
    s.setValue(REG_ROWS,            settingsData.rows);
    s.setValue(REG_WIDTH,           settingsData.width);
    s.setValue(REG_GAP,             settingsData.gap);
    s.setValue(REG_OVERWRITE,       settingsData.overwrite);
    s.setValue(REG_SUFFIX,          settingsData.suffix);

    s.setValue(REG_EDGE,            settingsData.edge_detect);
    s.setValue(REG_QUALITY,         settingsData.quality);
    s.setValue(REG_BLANK,           settingsData.blank_skip);
    s.setValue(REG_SKIPBEGIN,       settingsData.skip_begin);
    s.setValue(REG_SKIPEND,         settingsData.skip_end);
    s.setValue(REG_STEP,            settingsData.step);
    s.setValue(REG_MINHEIGHT,       settingsData.minHeight);
    s.setValue(REG_VERBOSE,         settingsData.verbose);

    s.setValue(REG_TITLE,           settingsData.title);
    s.setValue(REG_INFOTEXT,        settingsData.infotext);
    s.setValue(REG_TIMESTAMP,       settingsData.timestamp);

    s.setValue(REG_BACKGROUND,      settingsData.background.name());
    s.setValue(REG_FOREGROUND,      settingsData.foreground.name());
    s.setValue(REG_TIMECOLOR,       settingsData.timecolor.name());
    s.setValue(REG_TIMESHADOW,      settingsData.timeshadow.name());

    s.setValue(REG_FONTTEXT,        settingsData.fontInfotext);
    s.setValue(REG_FONTTIME,        settingsData.fontTimestamp);
    s.setValue(REG_FONTTEXTSIZE,    settingsData.fontInfoSize);
    s.setValue(REG_FONTTIMESIZE,    settingsData.fontTimeSize);
    s.setValue(REG_FONTTEXTLOCATION, settingsData.fontInfoLocation);
    s.setValue(REG_FONTTIMELOCATION, settingsData.fontTimeLocation);

    /// qmtn settings
    s.setValue(REG_MTN,             settingsData.executable);
    s.setValue(REG_MAXDIRDEPTH,     settingsData.max_dir_depth);
}

SettingsData MtnWorker::data()
{
    return settingsData;
}

void MtnWorker::setData(SettingsData newData)
{
    settingsData = newData;
}

QString MtnWorker::findExecutableMtn()
{
    const QString  mtn_cli=MTN_EXE;
    QString mtn_exe=settingsData.executable;

    if(mtn_exe.isEmpty())
    {
        mtn_exe = QStandardPaths::findExecutable(mtn_cli);

        if(mtn_exe.isEmpty())
        {
            QStringList searchPaths;
            searchPaths << qApp->applicationDirPath() << QDir::currentPath() << qApp->applicationDirPath() + "/mtn";
            mtn_exe = QStandardPaths::findExecutable(mtn_cli, searchPaths);
        }
    }
    return mtn_exe;
}

QString MtnWorker::outputFile(const QString inputfilename)
{
    QString filename;
    QDir directory;

    if (inputfilename.isEmpty())
        return QString();

    if(settingsData.output_directory.isEmpty())
    {
        directory = QFileInfo(inputfilename).dir();
    }
    else
    {
        directory = QDir(settingsData.output_directory);
    }

    if(settingsData.suffix.isEmpty())
    {
        QFileInfo f = QFileInfo(inputfilename);
        filename = f.completeBaseName() + "_s.jpg";
    }
    else
    {
        QFileInfo f = QFileInfo(inputfilename);
        filename = f.completeBaseName() + settingsData.suffix;
    }

    return QFileInfo(directory, filename).absoluteFilePath();

}

void MtnWorker::enqueue(QStandardItem* parent, int row)
{
    if(parent && parent->child(row))
    {
        QString outfile = outputFile(parent->child(row, columnItemNames::path)->text());

        QThreadPool::globalInstance()->start(new MtnJob(this, parent, row, settingsData, outfile));
        emit changedProcessingItemsNumber(+1);
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
