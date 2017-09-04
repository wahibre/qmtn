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

#include "iconprovider.h"

IconProvider::IconProvider() {}

QIcon IconProvider::qt()       {return QIcon(":/rsc/qt.png");                      }

#ifdef Q_OS_LINUX

QIcon IconProvider::folder()   {return QIcon::fromTheme("folder");                 }

QIcon IconProvider::loading()  {return QIcon::fromTheme("image-loading");          }

QIcon IconProvider::video()    {return QIcon::fromTheme("video-x-generic");        }

QIcon IconProvider::image()    {return QIcon::fromTheme("image-x-generic");        }

QIcon IconProvider::error()    {return QIcon::fromTheme("dialog-error");           }

QIcon IconProvider::refresh()  {return QIcon::fromTheme("view-refresh");           }

QIcon IconProvider::help()     {return QIcon::fromTheme("help-about");             }

QIcon IconProvider::settings() {return QIcon::fromTheme("applications-system");    }

QIcon IconProvider::exit()     {return QIcon::fromTheme("application-exit");       }

QIcon IconProvider::text()     {return QIcon::fromTheme("accessories-text-editor");}

QIcon IconProvider::zoomIn()   {return QIcon::fromTheme("zoom-in");                }

QIcon IconProvider::zoomOut()  {return QIcon::fromTheme("zoom-out");               }

QIcon IconProvider::zoomOrig() {return QIcon::fromTheme("zoom-original");          }

QIcon IconProvider::zoomFit()  {return QIcon::fromTheme("zoom-fit-best");          }

#else

QIcon IconProvider::folder()   {return QIcon(":/rsc/zlozka.svg");                  }

QIcon IconProvider::loading()  {return QIcon(":/rsc/loading.svg");                 }

QIcon IconProvider::video()    {return QIcon(":/rsc/video.svg");                   }

QIcon IconProvider::image()    {return QIcon(":/rsc/image.svg");                   }

QIcon IconProvider::error()    {return QIcon(":/rsc/failed.svg");                  }

QIcon IconProvider::refresh()  {return QIcon(":/rsc/refresh.svg");                 }

QIcon IconProvider::help()     {return QIcon(":/rsc/help.svg");                    }

QIcon IconProvider::settings() {return QIcon(":/rsc/settings.svg");                }

QIcon IconProvider::exit()     {return QIcon(":/rsc/off.svg");                     }

QIcon IconProvider::text()     {return QIcon(":/rsc/log.svg");                     }

QIcon IconProvider::zoomIn()   {return QIcon(":/rsc/zoomin.svg");                  }

QIcon IconProvider::zoomOut()  {return QIcon(":/rsc/zoomout.svg");                 }

QIcon IconProvider::zoomOrig() {return QIcon(":/rsc/zoomorig.svg");                }

QIcon IconProvider::zoomFit()  {return QIcon(":/rsc/zoomfit.svg");                 }

#endif
