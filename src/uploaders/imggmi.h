#ifndef IMGGMI_H
#define IMGGMI_H

#include <QObject>
#include "imgup.h"

namespace imguploader {
class Imggmi;
}

class Imggmi : public ImgUp
{
   Q_OBJECT
public:
    Imggmi(QWidget *parent, QString filePath=QString());

protected:
    virtual void postUploadRequest() Q_DECL_OVERRIDE;
    void showUploadPage(QString pagePath);

private slots:
    void imageUploaded();
};

#endif // IMGGMI_H
