#ifndef IMGUP_H
#define IMGUP_H

#include <QObject>
#include <QDialog>
#include <QFile>
#include <QNetworkRequest>
#include <QNetworkReply>
#ifdef QT_WEBENGINEWIDGETS_LIB
#include <QWebEngineView>
#else
#include <QTextEdit>
#include <QFile>
#include <QDir>
#endif
#include <QHBoxLayout>

namespace imguploader {
class ImgUp;
}

class ImgUp : public QDialog
{
private:
    QWidget *mainWindow;
protected:
    QString imageFilePath;
    QString serverName;
    QString serverScheme;
    QNetworkAccessManager *nm;
    QNetworkReply *uploadReply;
#ifdef QT_WEBENGINEWIDGETS_LIB
    QWebEngineView *webEngine;
#else
    QTextEdit *webBrowser;
#endif
    QHBoxLayout *layout;

    virtual void postUploadRequest(){}
    void showErr(QString msg);
public:
    ImgUp(QWidget *parent, QString filePath=QString());
    void setImagePath(QString imagePath);
    QString hostName();
    void upload();
};

#endif // IMGUP_H
