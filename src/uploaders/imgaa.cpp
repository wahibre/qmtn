#include <QFileInfo>
#include <QHttpMultiPart>
#include <QJsonDocument>

#include "imgaa.h"
/*****************************************************************************/
ImgAa::ImgAa(QWidget *parent, QString filePath):
    ImgUp(parent, filePath)
{
    serverName="imgaa.com";
    serverScheme="https";
}
/*****************************************************************************/
void ImgAa::postUploadRequest()
{
    QFileInfo fi(imageFilePath);
    const QString fileName = fi.fileName();
    const QString filePath = fi.filePath();
    const QByteArray boundary = "---------------------------10136583214395";

    QNetworkRequest req;
    QUrl url;
    QString contentType = QString("image/%1").arg((fi.suffix().toLower() == "png") ? "png" : "jpeg");

    url.setScheme(serverScheme);
    url.setHost(serverName);
    url.setPath("/api/upload");

    req.setUrl(url);
    req.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("multipart/form-data; boundary=" + boundary));

    QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);

    QHttpPart imgPart;
    imgPart.setHeader(QNetworkRequest::ContentDispositionHeader, QString("form-data; name=\"smfile\"; filename=\"%1\"").arg(fileName));
    imgPart.setHeader(QNetworkRequest::ContentTypeHeader,  contentType);
    QFile *imgFile = new QFile(filePath);
    imgFile->open(QIODevice::ReadOnly);
    imgPart.setBodyDevice(imgFile);
    imgFile->setParent(multiPart); // we cannot delete the imgFile now, so delete it with the multiPart

    QHttpPart filePart;
    filePart.setHeader(QNetworkRequest::ContentDispositionHeader, QString("form-data; name=\"file_id\""));
    filePart.setBody("0");

    QHttpPart contentPart;
    contentPart.setHeader(QNetworkRequest::ContentDispositionHeader, QString("form-data; name=\"content_type\""));
    contentPart.setBody("fs");

    QHttpPart thumbPart;
    thumbPart.setHeader(QNetworkRequest::ContentDispositionHeader, QString("form-data; name=\"thumb_size\""));
    thumbPart.setBody("180");

    QHttpPart aspectPart;
    aspectPart.setHeader(QNetworkRequest::ContentDispositionHeader, QString("form-data; name=\"thumb_aspect_ratio\""));
    aspectPart.setBody("resize");

    QHttpPart galleryPart;
    galleryPart.setHeader(QNetworkRequest::ContentDispositionHeader, QString("form-data; name=\"galleryid\""));
    galleryPart.setBody("");

    multiPart->append(imgPart);
    multiPart->append(filePart);
    multiPart->append(contentPart);
    multiPart->append(thumbPart);
    multiPart->append(aspectPart);
    multiPart->append(galleryPart);

    multiPart->setBoundary(boundary);

    uploadReply = nm->post(req, multiPart);
    multiPart->setParent(uploadReply);    // delete the multiPart with the reply

    connect(uploadReply,   &QNetworkReply::finished, this, &ImgAa::imageUploaded);
}
/*****************************************************************************/
void ImgAa::imageUploaded()
{
/* Reply:
    {
        "code":"success",
        "data":{
            "width":960,
            "height":960,
            "filename":"mighty_mouse_wolf_wolf_512kb_s.jpg",
            "storename":"5e27157f82de1.jpg",
            "extension":".jpg",
            "size":228109,
            "path":"\/2020\/01\/21\/5e27157f82de1.jpg",
            "hash":"srUHJGq3Lm6Y8SW",
            "timestamp":1579619711,
            "type":1,"ip":"84.16.51.161",
            "thumb":{"url":"https:\/\/t.imgaa.com\/2020\/01\/21\/5e27157f82de1_180x180.jpg","width":180,"height":180},
            "imageurl":"https:\/\/i.imgaa.com\/2020\/01\/21\/5e27157f82de1.jpg",
            "deleteurl":"https:\/\/imgaa.com\/delete\/srUHJGq3Lm6Y8SW",
            "link":"https:\/\/imgaa.com\/image\/5e27157f82de1"
        }
    }
*/
    QNetworkReply *reply = uploadReply;

    if(reply->error() == QNetworkReply::NoError)
    {
        QByteArray data = reply->readAll();

        qDebug() << "Reply:" << data;

        auto jsonDoc = QJsonDocument::fromJson(data);

        if(!jsonDoc.isNull())
        {
            if(jsonDoc.isObject())
            {
                auto o = jsonDoc.object();

                if(!o.isEmpty())
                {
                    QString code = o["code"].toString();

                    if(code == "success")
                        showUploadPage(o["data"].toObject());
                    else
                        showErr(QString("%1: %2!").arg(tr("Result").arg(code)));
                }
                else
                    showErr(tr("Empty JSON object!"));
            }
            else
                showErr(tr("Returned page does not contain JSON object!"));
        }
        else
            showErr(tr("Empty result!"));
    }
    else
        showErr(reply->errorString());

    reply->deleteLater();
}
/*****************************************************************************/
void ImgAa::showUploadPage(QJsonObject data)
{
    auto filename   = data["filename"].toString();
    auto link       = data["link"].toString();
    auto imageurl   = data["imageurl"].toString();
    auto deleteurl  = data["deleteurl"].toString();
    auto thumb      = data["thumb"].toObject();
    auto thumb_url  = thumb["url"].toString();

    auto htmltext = createHTML(filename, link, thumb_url, imageurl, deleteurl);

#ifdef QT_WEBENGINEWIDGETS_LIB
    webEngine->setHtml(htmltext);
#else
    webBrowser->setHtml(htmltext);
#endif
    show();
}
/*****************************************************************************/
QString ImgAa::createHTML(QString filename, QString link, QString thumb_url, QString imageurl, QString deleteurl)
{
#ifdef QT_WEBENGINEWIDGETS_LIB
    return QString(
R"(
<head>
    <style>
        .dlinput_header {
            font-size: 11px;
            margin-top: 10px;
        }
    </style>
</head>
<body>

    <div align=center><img src=":/icons/imgaa.png">
        <br>
        <h2>Image has been uploaded.</h2>
        <br>
    </div>

    <div>
        <fieldset>
            <legend>%1</legend>
            <table>
                <td style="width:180px;">
                    <a href="%2" target="_blank">
                        <img src="%3" style="max-width: 300px;max-height: 300px;">
                    </a>
                </td>
                <td style="padding-left: 10px;">
                    <div class=dlinput_header>HTML</div>
                    <div><input onclick="this.select();" style="width:400px;" type="text" value='&lt;a href="%2" target="_blank"&gt;&lt;img src="%3" &gt;&lt;/a&gt;'></div>
                    <div class=dlinput_header>BBCode</div>
                    <div><input onclick="this.select();" style="width:400px;" type="text" value="[url=%2][img]%3[/img][/url]"></div>
                    <div class=dlinput_header>Markdown</div>
                    <div><input onclick="this.select();" style="width:400px;" type="text" value="![%1](%4) "></div>
                    <div class=dlinput_header>Link only</div>
                    <div><input onclick="this.select();" style="width:400px;" type="text" value="%2"></div>
                    <div class=dlinput_header>Removal Link (to delte the image)</div>
                    <div><input onclick="this.select();" style="width:400px;" type="text" value="%5"></div>
                </td>
            </table>
        </fieldset>
    </div>
</body>
)"

            ).arg(filename).arg(link).arg(thumb_url).arg(imageurl).arg(deleteurl);
#else
    return QString(
R"(
    <div align=center><h2>www.Imgaa.com</h2>
        <br>
        <h2>Image has been uploaded.</h2>
        <br>
    </div>

    <div>
        <table align=center border=2px>
            <tr>
                <td align=center><b>HTML</b></td>
                <td>&lt;a href="%2" target="_blank"&gt;&lt;img src="%3" &gt;&lt;/a&gt;</td>
            </tr>
            <tr>
                <td align=center><b>BBCode</b></td>
                <td>[url=%2][img]%3[/img][/url]</td>
            </tr>
            <tr>
                <td align=center><b>Markdown</b></td>
                <td>![%1](%4)</td>
            </tr>
            <tr>
                <td align=center><b>Link only</b></td>
                <td><a href="%2">%2</a></td>
            </tr>
            <tr>
                <td align=center><b>Removal Link (to delte the image)</b></td>
                <td><a href="%5">%5</a></td>
            </tr>
        </table>
    </div>
)"

    ).arg(filename).arg(link).arg(thumb_url).arg(imageurl).arg(deleteurl);
#endif
}
/*****************************************************************************/
