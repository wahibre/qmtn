#ifndef KLIKR_H
#define KLIKR_H

#include <QObject>
#include "imgup.h"

namespace imguploader {
class Klikr;
}

class Klikr: public ImgUp
{
    Q_OBJECT
public:
    Klikr(QWidget *parent, QString filePath=QString());

protected:
    virtual void postUploadRequest() Q_DECL_OVERRIDE;
    void showUploadPage(QString pagePath);

private slots:
    void imageUploaded();
};

#endif // KLIKR_H
