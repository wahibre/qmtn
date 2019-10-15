#include <QFileInfo>
#include <QHttpMultiPart>
#include <QRegExp>
#include "imagevenue.h"
/*****************************************************************************/
Imagevenue::Imagevenue(QWidget *parent, QString filePath):
    ImgUp(parent, filePath)
{
    serverName="www.imagevenue.com";
    serverScheme="http";
}
/*****************************************************************************/
void Imagevenue::postUploadRequest()
{
    QFileInfo fi(imageFilePath);
    const QString fileName = fi.fileName();
    const QString filePath = fi.filePath();
    const QString filesuffix = fi.suffix().toLower();
    QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);
    const QByteArray boundary = multiPart->boundary();

    QNetworkRequest req;
    QUrl url;
    QStringList allowedSuff; allowedSuff << "jpeg" << "jpg";
    QString contentType = QString("image/%1").arg(fi.suffix().toLower());

    if(!allowedSuff.contains(filesuffix))
    {
        showErr("Image types allowed: "+allowedSuff.join(", "));
        reject();
        return;
    }

    url.setScheme(serverScheme);
    url.setHost(serverName);
    url.setPath("/upload.php");

    req.setUrl(url);
    req.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("multipart/form-data; boundary=" + boundary));

    QHttpPart userfilePart;
    userfilePart.setHeader(QNetworkRequest::ContentDispositionHeader, QString("form-data; name=\"userfile[]\"; filename=\"%1\"").arg(fileName));
    userfilePart.setHeader(QNetworkRequest::ContentTypeHeader,  contentType);
    QFile *imgFile = new QFile(filePath);
    imgFile->open(QIODevice::ReadOnly);
    userfilePart.setBodyDevice(imgFile);
    imgFile->setParent(multiPart); // we cannot delete the imgFile now, so delete it with the multiPart

    QHttpPart imgcontentPart;
    imgcontentPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"imgcontent\""));
    imgcontentPart.setBody("safe");

    QHttpPart resizePart;
    resizePart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"img_resize\""));
    resizePart.setBody(QByteArray());

    QHttpPart maxSizePart;
    maxSizePart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"MAX_FILE_SIZE\""));
    maxSizePart.setBody("10485760");

    QHttpPart actionPart;
    actionPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"action\""));
    actionPart.setBody("1");

    QHttpPart xPart;
    xPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"x\""));
    xPart.setBody("149");

    QHttpPart yPart;
    yPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"y\""));
    yPart.setBody("23");

    multiPart->append(userfilePart);
    multiPart->append(imgcontentPart);
    multiPart->append(resizePart);
    multiPart->append(maxSizePart);
    multiPart->append(actionPart);
    multiPart->append(xPart);
    multiPart->append(yPart);

    uploadReply = nm->post(req, multiPart);
    multiPart->setParent(uploadReply);    // delete the multiPart with the reply

    connect(uploadReply,   &QNetworkReply::finished, this, &Imagevenue::imageUploaded);
}
/*****************************************************************************/
void Imagevenue::imageUploaded()
{
    QNetworkReply *reply = uploadReply;

    if(reply->error() == QNetworkReply::NoError)
    {
        QByteArray data = reply->readAll();

    #ifdef QT_WEBENGINEWIDGETS_LIB
        webEngine->setHtml(QString::fromUtf8(data));
        showUploadPage(QString());
    #else
        auto r = QRegExp("ID='TextArea4'>(.*)<\\/textarea");
        r.setMinimal(true); // non-greedy match

        if(r.indexIn(data) >= 0)
        {
            auto resultLink = r.cap(1);
            auto htmltext = QString("<div align=center><img src=\":/icons/imagevenue.png\"><br><h2>Image has been uploaded.</h2><br><a href=\"%1\">%1</a></div>").arg(resultLink);
            webBrowser->setHtml(htmltext);
            showUploadPage(QString());
        }
        else
        {
            QFile resultFile(QString("%1/%2-response.html").arg(QDir::tempPath()).arg(hostName()));
            resultFile.open(QIODevice::WriteOnly);
            resultFile.write(data);
            resultFile.close();

            showErr(tr("Returned page does not contain TextArea4!\nResponse saved in %1").arg(resultFile.fileName()));
        }
    #endif
    }
    else
        showErr(reply->errorString());

    reply->deleteLater();
}
/*****************************************************************************/
void Imagevenue::showUploadPage(QString)
{
    show();
}
/*****************************************************************************/
