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

BuildRequires:	gcc-c++ qt5-qtbase-devel
Requires:		qt5-qtbase qt5-qtsvg

%{?fedora:BuildRequires: qt5-qtwebengine-devel}
%{?fedora:Requires:	 	 qt5-qtwebengine}

%if 0%{?fedora}
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
* Wed Jan 22 2020 wahibre  <wahibre@gmx.com> - 0.5
- packaging CentOS
- upload image to Imagevenue.com and Imgaa.com
- add profile name to status bar
- display images even if mtn's result is Warning

* Thu Mar 21 2019 wahibre  <wahibre@gmx.com> - 0.4
- import/export settings
- recent files
- supporting mtn's --transparent switch
- supporting mtn's --shadow switch
- font dialog changed to ComboBox (windows only)

* Tue Jan 9 2018 wahibre  <wahibre@gmx.com> - 0.3
- merge toplevel directory of dropped files in treeview
- show image in fullscreen
- inserting/removing profiles
- filesize only in human readable format (MiB, GiB)
- changed log icon
- removed group separator in numbers (affect -B, -E mtn's switches)

* Fri Nov 24 2017 wahibre  <wahibre@gmx.com> - 0.2
- location of mtn
- max. recursion depth
- settings profile
- recreating thumbnail
- context menu item "Open Movie"
- collapseAll/expandAll context menu items
- fix: autorefresh created image/log
- fix: floating numbers respects locale settings

* Mon Aug 21 2017 wahibre  <wahibre@gmx.com> - 0.1.9
- option to locate mtn binary
- option to specify recursion depth
- icons for Windows version
