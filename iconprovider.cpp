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

/*
 * http://standards.freedesktop.org/icon-theme-spec/icon-theme-spec-latest.html
 * http://standards.freedesktop.org/icon-naming-spec/icon-naming-spec-latest.html
*/

IconProvider::IconProvider() {}

QIcon IconProvider::qt()       {return QIcon(":/icons/qt.png");                      }

#ifdef Q_OS_LINUX

QIcon IconProvider::folder()   {return QIcon::fromTheme("folder",                   QIcon(":/icons/zlozka.svg"));   }

QIcon IconProvider::loading()  {return QIcon::fromTheme("image-loading",            QIcon(":/icons/loading.svg"));  }

QIcon IconProvider::video()    {return QIcon::fromTheme("video-x-generic",          QIcon(":/icons/video.svg"));    }

QIcon IconProvider::image()    {return QIcon::fromTheme("image-x-generic",          QIcon(":/icons/image.svg"));    }

QIcon IconProvider::error()    {return QIcon::fromTheme("dialog-error",             QIcon(":/icons/failed.svg"));   }

QIcon IconProvider::refresh()  {return QIcon::fromTheme("view-refresh",             QIcon(":/icons/refresh.svg"));  }

QIcon IconProvider::help()     {return QIcon::fromTheme("help-about",               QIcon(":/icons/help.svg"));     }

QIcon IconProvider::settings() {return QIcon::fromTheme("applications-system",      QIcon(":/icons/settings.svg")); }

QIcon IconProvider::exit()     {return QIcon::fromTheme("application-exit",         QIcon(":/icons/off.svg"));      }

QIcon IconProvider::text()     {return QIcon::fromTheme("accessories-text-editor",  QIcon(":/icons/log.svg"));      }

QIcon IconProvider::zoomIn()   {return QIcon::fromTheme("zoom-in",                  QIcon(":/icons/zoomin.svg"));   }

QIcon IconProvider::zoomOut()  {return QIcon::fromTheme("zoom-out",                 QIcon(":/icons/zoomout.svg"));  }

QIcon IconProvider::zoomOrig() {return QIcon::fromTheme("zoom-original",            QIcon(":/icons/zoomorig.svg")); }

QIcon IconProvider::zoomFit()  {return QIcon::fromTheme("zoom-fit-best",            QIcon(":/icons/zoomfit.svg"));  }

#else

QIcon IconProvider::folder()   {return QIcon(":/icons/zlozka.svg");                  }

QIcon IconProvider::loading()  {return QIcon(":/icons/loading.svg");                 }

QIcon IconProvider::video()    {return QIcon(":/icons/video.svg");                   }

QIcon IconProvider::image()    {return QIcon(":/icons/image.svg");                   }

QIcon IconProvider::error()    {return QIcon(":/icons/failed.svg");                  }

QIcon IconProvider::refresh()  {return QIcon(":/icons/refresh.svg");                 }

QIcon IconProvider::help()     {return QIcon(":/icons/help.svg");                    }

QIcon IconProvider::settings() {return QIcon(":/icons/settings.svg");                }

QIcon IconProvider::exit()     {return QIcon(":/icons/off.svg");                     }

QIcon IconProvider::text()     {return QIcon(":/icons/log.svg");                     }

QIcon IconProvider::zoomIn()   {return QIcon(":/icons/zoomin.svg");                  }

QIcon IconProvider::zoomOut()  {return QIcon(":/icons/zoomout.svg");                 }

QIcon IconProvider::zoomOrig() {return QIcon(":/icons/zoomorig.svg");                }

QIcon IconProvider::zoomFit()  {return QIcon(":/icons/zoomfit.svg");                 }

#endif
