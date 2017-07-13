#include "iconprovider.h"

IconProvider::IconProvider() {}

QIcon IconProvider::folder()   {return QIcon::fromTheme("folder");                 }

QIcon IconProvider::loading()  {return QIcon::fromTheme("image-loading");          }

QIcon IconProvider::video()    {return QIcon::fromTheme("video-x-generic");        }

QIcon IconProvider::image()    {return QIcon::fromTheme("image-x-generic");        }

QIcon IconProvider::error()    {return QIcon::fromTheme("sync_error");             }

QIcon IconProvider::refresh()  {return QIcon::fromTheme("view-refresh");           }

QIcon IconProvider::help()     {return QIcon::fromTheme("help-about");             }

QIcon IconProvider::system()   {return QIcon::fromTheme("applications-system");    }

QIcon IconProvider::exit()     {return QIcon::fromTheme("application-exit");       }

QIcon IconProvider::text()     {return QIcon::fromTheme("accessories-text-editor");}

QIcon IconProvider::zoomIn()   {return QIcon::fromTheme("zoom-in");                }

QIcon IconProvider::zoomOut()  {return QIcon::fromTheme("zoom-out");               }

QIcon IconProvider::zoomOrig() {return QIcon::fromTheme("zoom-original");          }

QIcon IconProvider::zoomFit()  {return QIcon::fromTheme("zoom-fit-best");          }
