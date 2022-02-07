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

#include "profilemodel.h"
#include "settingsdata.h"
#include <QDir>
#include <QJsonDocument>
#include <QJsonObject>
#include <QStandardPaths>
#include <QtDebug>

/******************************************************************************************************/
ProfileModel::ProfileModel(QObject *parent):
    QAbstractItemModel(parent)
{
    loadFromJSON();
}
/******************************************************************************************************/
ProfileModel::~ProfileModel()
{
    saveToJSON();
}
/******************************************************************************************************/
QString ProfileModel::getSettingsFileName()
{
    QString settdir = QStandardPaths::writableLocation(QStandardPaths::DataLocation);

    if(!settdir.isEmpty())
    {
        QDir dir(settdir);

        if(!dir.exists())
        {
            if(!dir.mkpath(settdir))
            {
                qCritical() << QObject::tr("Cannot create DataLocation %1!").arg(settdir);
                return QString();
            }
        }

        QFileInfo settfileinfo(settdir, SETTINGS_FILE);
        return settfileinfo.absoluteFilePath();
    }
    else
        qCritical() << QObject::tr("Cannot find DataLocation and thus store settings!");

    return QString();
}
/******************************************************************************************************/
void ProfileModel::loadFromJSON()
{
    QString filename = getSettingsFileName();
    currSettingsIdx = 0;

    if(!filename.isEmpty() && QFile::exists(filename))
    {
        QFile settFile(filename);
        if(settFile.open(QIODevice::ReadOnly))
        {
            QByteArray data = settFile.readAll();
            QJsonDocument doc = QJsonDocument::fromJson(data);

            if(doc.isObject())
            {
                currSettingsIdx = doc.object()["CurrentSetting"].toInt(0);
                m_dataArray = doc.object()["SettingsList"].toArray();
            }

            if(m_dataArray.count() == 0)
                m_dataArray.append(SettingsData(QJsonObject()).toJsonObject());
        }
        else
            qCritical() << tr("Cannon open file %1 for reading!").arg(filename);
    }
    else
    {
        //Default values
        m_dataArray.append(SettingsData(QJsonObject()).toJsonObject());
    }
}
/******************************************************************************************************/
void ProfileModel::saveToJSON()
{
    QString filename = getSettingsFileName();

    if(!filename.isEmpty())
    {
        QFile settfile(filename);
        if(settfile.open(QIODevice::WriteOnly))
        {
            QJsonObject jsonOut = {
                {"CurrentSetting", currSettingsIdx},
                {"SettingsList", m_dataArray}
            };

            QJsonDocument doc(jsonOut);
            QByteArray arr = doc.toJson(QJsonDocument::Indented);
            QTextStream settstream(&settfile);

            settstream << arr;
            settfile.close();
        }
        else
            qCritical() << QObject::tr("Cannot open file %1!").arg(settfile.fileName());
    }
}
/******************************************************************************************************/
int ProfileModel::getCurrentProfileIdx()
{
    return currSettingsIdx;
}
/******************************************************************************************************/
void ProfileModel::setCurrentProfileIdx(int idx)
{
    currSettingsIdx = idx;
}
/******************************************************************************************************/
SettingsData ProfileModel::getCurrentSettingsData()
{
    return SettingsData(m_dataArray[currSettingsIdx].toObject());
}
/******************************************************************************************************/
int ProfileModel::rowCount(const QModelIndex &/*parent*/) const
{
    return m_dataArray.count();
}
/******************************************************************************************************/
QVariant ProfileModel::data(const QModelIndex &index, int role) const
{
    if(role == Qt::DisplayRole || role == Qt::EditRole)
    {
        if(index.column() == 0)
            return m_dataArray[index.row()].toObject()[REG_SETTINGSNAME].toString();
        if(index.column() == 1 )
            return QVariant::fromValue(SettingsData(m_dataArray[index.row()].toObject()));
    }

    return QVariant();
}
/******************************************************************************************************/
QModelIndex ProfileModel::index(int row, int column, const QModelIndex &/*parent*/) const
{
    return createIndex(row, column);
}
/******************************************************************************************************/
QModelIndex ProfileModel::parent(const QModelIndex &/*child*/) const
{
    return QModelIndex();
}
/******************************************************************************************************/
int ProfileModel::columnCount(const QModelIndex &/*parent*/) const
{
    return 2;
}
/******************************************************************************************************/
bool ProfileModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(role == Qt::EditRole && index.column() == 1)
    {
        m_dataArray.replace(index.row(), value.value<SettingsData>().toJsonObject());
        return true;
    }
    return false;
}
/******************************************************************************************************/
bool ProfileModel::insertRows(int row, int count, const QModelIndex &parent)
{
    if(row<0 || row > m_dataArray.count())
        return false;

    beginInsertRows(parent, row, row+count-1);

    for(int i=0; i<count; i++)
        m_dataArray.insert(row, QJsonObject());

    endInsertRows();

    return true;
}
/******************************************************************************************************/
bool ProfileModel::removeRows(int row, int count, const QModelIndex &parent)
{
    if(row+count > m_dataArray.count() || row < 0)
        return false;

    beginRemoveRows(parent, row, row+count-1);

    for(int i=0; i<count; i++)
        m_dataArray.removeAt(row);

    endRemoveRows();

    return true;
}
/******************************************************************************************************/
