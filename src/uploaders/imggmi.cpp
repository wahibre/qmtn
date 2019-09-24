#include <QMessageBox>
#include <QFileInfo>
#include <QHttpMultiPart>
#include <QJsonDocument>
#include <QJsonObject>

#include "imggmi.h"

Imggmi::Imggmi(QWidget *parent, QString filePath):
    QDialog(parent),
    imageFilePath (filePath)
{
    nm = new QNetworkAccessManager(this);
    layout = new QHBoxLayout(this);
#ifdef QT_WEBENGINEWIDGETS_LIB
    webEngine = new QWebEngineView(this);
    layout->addWidget(webEngine);
#else
    webBrowser = new QTextEdit(this);
    webBrowser->setReadOnly(true);
    layout->addWidget(webBrowser);
#endif
    setLayout(layout);
    setAttribute(Qt::WA_DeleteOnClose);

    setWindowTitle(serverName);
    setWindowFlags(windowFlags() | Qt::WindowMaximizeButtonHint);
    resize(parent->size() * 0.8);
    setVisible(false);
}

void Imggmi::upload()
{
    if(QFile::exists(imageFilePath))
        postUploadRequest();
    else
        showErr(tr("File '%1' not found!").arg(imageFilePath));
}

void Imggmi::postUploadRequest()
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
    url.setPath("/upload");

    req.setUrl(url);
    req.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("multipart/form-data; boundary=" + boundary));

    QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);

    QHttpPart imgPart;
    imgPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"img\""));
    imgPart.setBody("file");

    QHttpPart filePart;
    filePart.setHeader(QNetworkRequest::ContentDispositionHeader, QString("form-data; name=\"file\"; filename=\"%1\"").arg(fileName));
    filePart.setHeader(QNetworkRequest::ContentTypeHeader,  contentType);
    QFile *imgFile = new QFile(filePath);
    imgFile->open(QIODevice::ReadOnly);
    filePart.setBodyDevice(imgFile);
    imgFile->setParent(multiPart); // we cannot delete the imgFile now, so delete it with the multiPart

    QHttpPart urlPart;
    urlPart.setHeader(QNetworkRequest::ContentDispositionHeader, QString("form-data; name=\"url\""));
    urlPart.setBody(QByteArray());

    QHttpPart selImpPart;
    selImpPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"selected_input\""));
    selImpPart.setBody("file");

    QHttpPart sizePart;
    sizePart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"size\""));
    sizePart.setBody("640");

    QHttpPart previewPart;
    previewPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"preview_size\""));
    previewPart.setBody("180");

    QHttpPart rotationPart;
    rotationPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"rotation_type\""));
    rotationPart.setBody("0");

    multiPart->append(imgPart);
    multiPart->append(filePart);
    multiPart->append(urlPart);
    multiPart->append(selImpPart);
    multiPart->append(sizePart);
    multiPart->append(previewPart);
    multiPart->append(rotationPart);

    multiPart->setBoundary(boundary);

    uploadReply = nm->post(req, multiPart);
    multiPart->setParent(uploadReply);    // delete the multiPart with the reply

    connect(uploadReply,   &QNetworkReply::finished, this, &Imggmi::imageUploaded);

    /*

    Request Body:
    =============
    -----------------------------10136583214395
    Content-Disposition: form-data; name="img"

    file
    -----------------------------10136583214395
    Content-Disposition: form-data; name="file"; filename="sample.jpg"
    Content-Type: image/jpeg

    ÿØÿàÿØÿàÿ - image binary data - ØÿàÿØÿà


    -----------------------------10136583214395
    Content-Disposition: form-data; name="url"


    -----------------------------10136583214395
    Content-Disposition: form-data; name="selected_input"

    file
    -----------------------------10136583214395
    Content-Disposition: form-data; name="size"

    640
    -----------------------------10136583214395
    Content-Disposition: form-data; name="preview_size"

    180
    -----------------------------10136583214395
    Content-Disposition: form-data; name="rotation_type"

    0
    -----------------------------10136583214395--


    */
}

void Imggmi::imageUploaded()
{
    QNetworkReply *reply = uploadReply;

    QByteArray data = reply->readAll();

    auto jsonDoc = QJsonDocument::fromJson(data);

    if(!jsonDoc.isNull())
    {
        if(jsonDoc.isObject())
        {
            auto o = jsonDoc.object();

            if(!o.isEmpty())
            {
                /*
                    LINKS
                    =====
                    main:       https://cdn1.imggmi.com/uploads/2019/9/20/dfc9085130685e5d4e03013aa9aa29b9-full.jpg
                    image:      https://imggmi.com/full/2019/9/20/dfc9085130685e5d4e03013aa9aa29b9-full.jpg.html
                    thumbnail:  https://cdn1.imggmi.com/uploads/2019/9/20/dfc9085130685e5d4e03013aa9aa29b9-prev.jpg
                */

                bool error_returned = o["error"].toBool(false);
                if(!error_returned)
                    showUploadPage(o["redirect_path"].toString());
                else
                    showErr(QString("%1\n%2: %3").arg(tr("Upload failed!")).arg(tr("Reason given")).arg(o["text"].toString()));
            }
            else
                showErr(tr("Empty JSON object!"));
        }
        else
            showErr(tr("Returned page does not contain JSON object!"));
    }
    else
        showErr(tr("Empty result!"));

    reply->deleteLater();
}

void Imggmi::showUploadPage(QString pagePath)
{
    QUrl resulPage;
    resulPage.setScheme("https");
    resulPage.setHost(serverName);
    resulPage.setPath(pagePath);

    QString resultLink = resulPage.toString();

#ifdef QT_WEBENGINEWIDGETS_LIB
    webEngine->load(resulPage);
#else
    auto htmltext = QString("<div align=center><img src=\":/icons/imggmi.png\"><br><h2>Image has been uploaded.</h2><br><a href=\"%1\">%1</a></div>").arg(resultLink);
    webBrowser->setHtml(htmltext);
#endif
    show();
}

void Imggmi::showErr(QString msg)
{
    QMessageBox::critical(this, tr("Error"), msg);
}
