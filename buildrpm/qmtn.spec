Name:		qmtn	
Version: 	0.5
Release:	1%{?dist}
Summary:	Qt Movie thumbnailer

Group:		Amusements/Graphics
License:	GPLv3
URL:		http://gitlab.com/movie_thumbnailer/qmtn/wikis/
Source0:	https://gitlab.com/movie_thumbnailer/qmtn/-/archive/master/qmtn.tar.gz
#Source0:	https://gitlab.com/movie_thumbnailer/qmtn/repository/%{version}/archive.tar.gz
#Source0:	https://gitlab.com/movie_thumbnailer/qmtn/repository/archive.tar.gz?ref=%{version}

BuildRequires:	gcc-c++ make

%if 0%{?mageia}
BuildRequires:	lib64qt5webengine-devel
Requires:		lib64qt5webengine5 lib64qt5svg5
%else
BuildRequires:	qt5-qtbase-devel
Requires:		qt5-qtbase qt5-qtsvg
%endif

%{?fedora:BuildRequires: qt5-qtwebengine-devel}
%{?fedora:Requires:	 	 qt5-qtwebengine}

%if 0%{?fedora} || 0%{?mageia}
%define qmake_opts CONFIG+=use_webengine src
%else
%define qmake_opts src
%endif

%description
Movie thumbnail generator written in Qt5

%prep
rm -rf ./*
tar -xf %SOURCE0
mv qmtn*/* ./

%build
qmake-qt5 INSTALL_ROOT=%{buildroot} %{qmake_opts}

%make_build

%install
make install INSTALL_ROOT=%{buildroot}

%files
%{_bindir}/qmtn
%{_datadir}/applications/%{name}.desktop
%{_datadir}/pixmaps/%{name}.png

%clean
rm -rf %{buildroot}
rm -rf *

%changelog
* Fri Apr 03 2020 wahibre  <wahibre@gmx.com> - 0.5
- packaging Mageia
- packaging CentOS
