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

#include "settingsdata.h"
#include <QJsonArray>
#include <QJsonDocument>

/******************************************************************************************************/
SettingsData::SettingsData(){}
/******************************************************************************************************/
SettingsData::SettingsData(QJsonObject obj)
{
    /// mtn settings
    settingsName        = obj[REG_SETTINGSNAME      ].toString("default");

    output_directory    = obj[REG_OUTPUT_DIRECTORY  ].toString();

    columns             = obj[REG_COLUMNS           ].toInt(4);
    rows                = obj[REG_ROWS              ].toInt(8);
    width               = obj[REG_WIDTH             ].toInt(1920);
    gap                 = obj[REG_GAP               ].toInt(3);
    overwrite           = obj[REG_OVERWRITE         ].toBool(true);
    suffix              = obj[REG_SUFFIX            ].toString();

    edge_detect         = obj[REG_EDGE              ].toInt(12);
    blank_skip          = obj[REG_BLANK             ].toDouble(0.80);
    quality             = obj[REG_QUALITY           ].toInt(90);
    skip_begin          = obj[REG_SKIPBEGIN         ].toDouble(0.0);
    skip_end            = obj[REG_SKIPEND           ].toDouble(0.0);
    step                = obj[REG_STEP              ].toInt(0);
    minHeight           = obj[REG_MINHEIGHT         ].toInt(0);
    shadowRadius        = obj[REG_SHADOW_RADIUS     ].toInt(0);
    verbose             = obj[REG_VERBOSE           ].toBool(false);
    transparent         = obj[REG_TRANSPARENT       ].toBool(false);

    title               = obj[REG_TITLE             ].toString();
    infotext            = obj[REG_INFOTEXT          ].toBool(true);
    timestamp           = obj[REG_TIMESTAMP         ].toBool(true);

    background.setNamedColor(   obj[REG_BACKGROUND].toString(QColor(Qt::black).name()));
    foreground.setNamedColor(   obj[REG_FOREGROUND].toString(QColor(Qt::white).name()));
    timecolor.setNamedColor (   obj[REG_TIMECOLOR ].toString(QColor(Qt::black).name()));
    timeshadow.setNamedColor(   obj[REG_TIMESHADOW].toString(QColor(Qt::gray).name()));

    fontInfotext        = obj[REG_FONTTEXT          ].toString();
    fontTimestamp       = obj[REG_FONTTIME          ].toString();
    fontInfoSize        = obj[REG_FONTTEXTSIZE      ].toInt(0);
    fontTimeSize        = obj[REG_FONTTIMESIZE      ].toInt(0);
    fontInfoLocation    = obj[REG_FONTTEXTLOCATION  ].toInt(-1);
    fontTimeLocation    = obj[REG_FONTTIMELOCATION  ].toInt(-1);

    /// qmtn settings
    max_dir_depth       = obj[REG_MAXDIRDEPTH       ].toInt(10);
    executable          = obj[REG_MTN               ].toString(findExecutableMtn()); //mtn not searched if exists in file and is empty

    if(executable.isEmpty()) //empty in file
        executable = findExecutableMtn();
}
/******************************************************************************************************/
QJsonObject SettingsData::toJsonObject()
{
    QJsonObject o
    {
        {REG_SETTINGSNAME,      settingsName    },

        {REG_OUTPUT_DIRECTORY,  output_directory},

        {REG_COLUMNS,           columns         },
        {REG_ROWS,              rows            },
        {REG_WIDTH,             width           },
        {REG_GAP,               gap             },
        {REG_OVERWRITE,         overwrite       },
        {REG_SUFFIX,            suffix          },

        {REG_EDGE,              edge_detect     },
        {REG_BLANK,             blank_skip      },
        {REG_QUALITY,           quality         },
        {REG_SKIPBEGIN,         skip_begin      },
        {REG_SKIPEND,           skip_end        },
        {REG_STEP,              step            },
        {REG_MINHEIGHT,         minHeight       },
        {REG_SHADOW_RADIUS,     shadowRadius    },
        {REG_VERBOSE,           verbose         },
        {REG_TRANSPARENT,       transparent     },

        {REG_TITLE,             title           },
        {REG_INFOTEXT,          infotext        },
        {REG_TIMESTAMP,         timestamp       },

        {REG_BACKGROUND,        background.name()   },
        {REG_FOREGROUND,        foreground.name()   },
        {REG_TIMECOLOR,         timecolor.name()    },
        {REG_TIMESHADOW,        timeshadow.name()   },

        {REG_FONTTEXT,          fontInfotext    },
        {REG_FONTTIME,          fontTimestamp   },
        {REG_FONTTEXTSIZE,      fontInfoSize    },
        {REG_FONTTIMESIZE,      fontTimeSize    },
        {REG_FONTTEXTLOCATION,  fontInfoLocation},
        {REG_FONTTIMELOCATION,  fontTimeLocation},

        {REG_MTN,               executable      },
        {REG_MAXDIRDEPTH,       max_dir_depth   }
    };

    return o;
}
/******************************************************************************************************/
QString SettingsData::findExecutableMtn()
{
    const QString  mtn_cli=MTN_EXE;
    QString mtn_exe = QStandardPaths::findExecutable(mtn_cli);

    if(mtn_exe.isEmpty())
    {
        QStringList searchPaths;
        searchPaths << qApp->applicationDirPath() << QDir::currentPath() << qApp->applicationDirPath() + "/mtn";
        mtn_exe = QStandardPaths::findExecutable(mtn_cli, searchPaths);
    }
    return mtn_exe;
}
/******************************************************************************************************/
