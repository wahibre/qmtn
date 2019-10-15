#include <QMessageBox>
#include "imgup.h"
/*****************************************************************************/
ImgUp::ImgUp(QWidget *parent, QString filePath):
    QDialog(parent),
    mainWindow(parent),
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
/*****************************************************************************/
void ImgUp::setImagePath(QString imagePath)
{
    imageFilePath = imagePath;
}
/*****************************************************************************/
QString ImgUp::hostName()
{
    return serverName;
}
/*****************************************************************************/
void ImgUp::upload()
{
    if(QFile::exists(imageFilePath))
        postUploadRequest();
    else
        showErr(tr("File '%1' not found!").arg(imageFilePath));
}
/*****************************************************************************/
void ImgUp::showErr(QString msg)
{
    QMessageBox::critical(mainWindow, tr("Error"), msg);
}
/*****************************************************************************/
