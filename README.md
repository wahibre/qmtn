Description
============

GUI Thumbnail Generator Using CLI [mtn](https://gitlab.com/movie_thumbnailer/mtn/wikis/home/).

Aplication focuses on quick thumbnails-grid creating using drag&drop.

Features:
- Drag&drop files and folders
- Recursive search for movie files
- Instant image making in background
- Image and output log preview
- Open image in Fullscreen or and external image viewer
- Recreate image with new settings
- Managing mtn switches
- Settings profiles


Dependencies
============
Ubuntu

    sudo apt-get install qt5-default qtbase5-dev
Fedora

    dnf install qt5-qtbase-devel

Get source
==========

    git clone https://gitlab.com/movie_thumbnailer/qmtn.git


Build
=====
    cd qmtn
    qmake-qt5 src
    make


Install
=======
    sudo make install

