#ifndef DATAMODEL_H
#define DATAMODEL_H

#include <QStandardItemModel>


//TODO own datamodel

class DataModel : public QStandardItemModel
{
public:
    explicit DataModel(QObject *parent = Q_NULLPTR);

};
#endif // DATAMODEL_H
