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

#ifndef MTNJOB_H
#define MTNJOB_H

#include <QRunnable>
#include <QTreeWidgetItem>
#include <QThread>
#include <QStandardItem>

#include "mtnworker.h"

#define WAIT_STARTTIMEOUT   -1
#define WAIT_FINISHTIMEOUT  20000

#define FINISH_STATUS_OK      0
#define FINISH_STATUS_WARNING 1

class MtnJob : public QRunnable
{
    QObject *m_parent;
    QStandardItem *m_stditem;
    int m_row;
    SettingsData m_sett;
    QString m_outputfilename;

public:
    MtnJob(QObject *parent, QStandardItem *item, int row, SettingsData settingsData, QString outputfilename);
private:
    void run() Q_DECL_OVERRIDE;
    QStringList createArguments();
    QString color2hex(QColor color);
    static QString timeString(QString text);
};

#endif // MTNJOB_H
