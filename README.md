Description
============

GUI Thumbnail Generator Using CLI [mtn](https://gitlab.com/movie_thumbnailer/mtn/wikis/home/).

Aplication focuses on quick thumbnails-grid creating using drag&drop.

Features:
- Drag&drop files and folders
- Recursive search for movie files
- Instant image making in background
- Image and output log preview
- Open image in external image viewer
- Recreate image with new settings
- Settings for managing mtn switches


Dependencies
============
Ubuntu 17.4  

    sudo apt-get install qt5-default qtbase5-dev
Fedora 25

    dnf install qt5-qtbase-devel

Get source
==========

    git clone https://gitlab.com/movie_thumbnailer/qmtn.git QtMtn


Build
=====
Fedora 25    

    qmake-qt5 QtMtn/mtngui.pro
Ubuntu 17.4 

    qmake QtMtn/mtngui.pro
    make


Install
=======
Fedora 25   

    su -c 'make install'
Ubuntu 17.4  

    sudo make install

