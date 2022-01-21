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

#ifndef IMGAA_H
#define IMGAA_H

#include <QObject>
#include <QJsonObject>
#include "imgup.h"

namespace imguploader {
class Imggmi;
}

class ImgAa : public ImgUp
{
    Q_OBJECT
    QString createHTML(QString filename, QString link, QString thumb_url, QString imageurl, QString deleteurl);
public:
    ImgAa(QWidget *parent, QString filePath=QString());

protected:
    virtual void postUploadRequest() Q_DECL_OVERRIDE;
    void showUploadPage(QJsonObject data);

private slots:
    void imageUploaded();
};

#endif // IMGAA_H
