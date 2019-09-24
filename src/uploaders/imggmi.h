#ifndef IMGGMI_H
#define IMGGMI_H

#include <QObject>
#include <QDialog>
#include <QFile>
#include <QNetworkRequest>
#include <QNetworkReply>
#ifdef QT_WEBENGINEWIDGETS_LIB
#include <QWebEngineView>
#else
#include <QTextEdit>
#endif
#include <QHBoxLayout>

namespace imguploader {
class Imggmi;
}

class Imggmi : public QDialog
{
    Q_OBJECT

    const QString serverName="imggmi.com";
    const QString serverScheme="https";

    QString imageFilePath;
    QNetworkAccessManager *nm;
    QNetworkReply *uploadReply;
#ifdef QT_WEBENGINEWIDGETS_LIB
    QWebEngineView *webEngine;
#else
    QTextEdit *webBrowser;
#endif
    QHBoxLayout *layout;

    void postUploadRequest();
    void showUploadPage(QString pagePath);
    void showErr(QString msg);

private slots:
    void imageUploaded();

public:
    Imggmi(QWidget *parent, QString filePath);
    void upload();
};

#endif // IMGGMI_H
