#include "mtnjob.h"
#include "mtnworker.h"

#include <QSettings>
#include <QDir>
#include<QStandardPaths>
#include<QCoreApplication>

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

    settingsData.output_directory = s.value(REG_OUTPUT_DIRECTORY, QString()).toString();

    settingsData.columns        = s.value(REG_COLUMNS,   4      ).toInt();
    settingsData.rows           = s.value(REG_ROWS,      8      ).toInt();
    settingsData.width          = s.value(REG_WIDTH,     920    ).toInt();
    settingsData.gap            = s.value(REG_GAP,       3      ).toInt();
    settingsData.overwrite      = s.value(REG_OVERWRITE, true   ).toBool();
    settingsData.suffix         = s.value(REG_SUFFIX            ).toString();

    settingsData.edge_detect    = s.value(REG_EDGE,      12     ).toInt();
    settingsData.blank_skip     = s.value(REG_BLANK,     0.80   ).toDouble();
    settingsData.quality        = s.value(REG_QUALITY,   90     ).toInt();
    settingsData.skip_begin     = s.value(REG_SKIPBEGIN, 0.0    ).toReal();
    settingsData.skip_end       = s.value(REG_SKIPEND,   0.0    ).toReal();

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
}

SettingsData MtnWorker::data()
{
    return settingsData;
}

void MtnWorker::setData(SettingsData newData)
{
    settingsData = newData;
}

bool MtnWorker::findExecutable()
{
    const QString  mtn_cli=__mtn();
    QString najdenyMtnProcess;

    najdenyMtnProcess = QStandardPaths::findExecutable(mtn_cli);

    if(najdenyMtnProcess.isEmpty())
    {
        QStringList searchPaths;
        searchPaths << qApp->applicationDirPath() << QDir::currentPath();
        najdenyMtnProcess = QStandardPaths::findExecutable(mtn_cli, searchPaths);
    }

    if(najdenyMtnProcess.isEmpty())
        return false;

    settingsData.executable = najdenyMtnProcess;

    return true;
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
    if(parent->child(row))
    {
        QString outfil = outputFile(parent->child(row, dataItemNames::path)->text());
        QThreadPool::globalInstance()->start(new MtnJob(parent, row, settingsData, outfil));
    }
}

