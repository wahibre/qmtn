/* Qt Movie Thumbnailer - movie screencast generator

Copyright (C) 2017 wahibre <wahibre@gmx.com>

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

#ifndef SETTINGSDATA_H
#define SETTINGSDATA_H

#include <QColor>

enum columnItemNames {
    filename    =0,     /* file & folder */
    path        =1,     /* file only     */
    logtext     =2,     /* file only     */
    output      =3      /* file only     */
};

struct SettingsData
{
    QString output_directory, suffix, title, fontInfotext, fontTimestamp;
    int columns,rows,width, edge_detect, quality, gap, fontInfoSize, fontTimeSize,
        fontInfoLocation, fontTimeLocation, step, minHeight;
    double blank_skip, skip_begin, skip_end;
    bool overwrite, infotext, timestamp, verbose;
    QColor foreground, background, timecolor, timeshadow;

    QString executable;
    int max_dir_depth;
};

#endif // SETTINGSDATA_H
