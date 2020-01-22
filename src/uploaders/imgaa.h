#ifndef IMGAA_H
#define IMGAA_H

#include <QObject>
#include <QJsonObject>
#include "imgup.h"

namespace imguploader {
class Imggmi;
}

class ImgAa : public ImgUp
{
    Q_OBJECT
    QString createHTML(QString filename, QString link, QString thumb_url, QString imageurl, QString deleteurl);
public:
    ImgAa(QWidget *parent, QString filePath=QString());

protected:
    virtual void postUploadRequest() Q_DECL_OVERRIDE;
    void showUploadPage(QJsonObject data);

private slots:
    void imageUploaded();
};

#endif // IMGAA_H
