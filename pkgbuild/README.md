# Packaging for Arch Linux

## Dependencies
Dependencies for building in docker image

```
pacman -S grep git binutils fakeroot gcc make awk file qt5-base qt5-webengine
```

## Build
Build package

```
cd pkgbuild
makepkg
```

## Install
Install package

```
pacman -U ./qmtn-git*.tar.xz
```
