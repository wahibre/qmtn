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

#ifndef ICONPROVIDER_H
#define ICONPROVIDER_H

#include <QIcon>

class IconProvider
{
public:
    IconProvider();
    static QIcon folder();
    static QIcon loading();
    static QIcon video();
    static QIcon image();
    static QIcon error();
    static QIcon remove();
    static QIcon warning();
    static QIcon refresh();
    static QIcon help();
    static QIcon settings();
    static QIcon exit();
    static QIcon text();
    static QIcon zoomIn();
    static QIcon zoomOut();
    static QIcon zoomOrig();
    static QIcon zoomFit();
    static QIcon fullScreen();
    static QIcon qt();
};

#endif // ICONPROVIDER_H
