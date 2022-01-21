/* Qt Movie Thumbnailer - movie screencast generator

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

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
