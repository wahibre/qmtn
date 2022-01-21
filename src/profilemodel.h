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

#ifndef PROFILEMODEL_H
#define PROFILEMODEL_H

#include <QAbstractItemModel>
#include <QObject>
#include <QJsonArray>

#include "settingsdata.h"

#define SETTINGS_FILE "settings.json"


class ProfileModel : public QAbstractItemModel
{
public:
    ProfileModel(QObject *parent);
    ~ProfileModel();

private:
    QJsonArray m_dataArray;
    int currSettingsIdx;
    QString getSettingsFileName();
    void loadFromJSON();
    void saveToJSON();

public:
    int getCurrentProfileIdx();
    void setCurrentProfileIdx(int idx);
    SettingsData getCurrentSettingsData();

    // QAbstractItemModel interface
    virtual int rowCount(const QModelIndex &parent=QModelIndex()) const Q_DECL_OVERRIDE;
    virtual QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
    virtual QModelIndex index(int row, int column, const QModelIndex &parent=QModelIndex()) const Q_DECL_OVERRIDE;
    virtual QModelIndex parent(const QModelIndex &child) const Q_DECL_OVERRIDE;
    virtual int columnCount(const QModelIndex &parent) const Q_DECL_OVERRIDE;
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role) Q_DECL_OVERRIDE;
    virtual bool insertRows(int row, int count, const QModelIndex &parent) Q_DECL_OVERRIDE;
    virtual bool removeRows(int row, int count, const QModelIndex &parent) Q_DECL_OVERRIDE;
    };

#endif // PROFILEMODEL_H
