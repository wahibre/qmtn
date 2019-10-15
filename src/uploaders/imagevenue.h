#ifndef IMAGEVENUE_H
#define IMAGEVENUE_H

#include <QObject>
#include "imgup.h"

namespace imguploader {
class Imagevenue;
}

class Imagevenue: public ImgUp
{
    Q_OBJECT
public:
    Imagevenue(QWidget *parent, QString filePath=QString());

protected:
    virtual void postUploadRequest() Q_DECL_OVERRIDE;
    virtual void showUploadPage(QString pagePath) Q_DECL_OVERRIDE;

private slots:
    void imageUploaded();
};

#endif // IMAGEVENUE_H
