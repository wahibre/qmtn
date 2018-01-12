Name:		qmtn	
Version: 	0.3
Release:	1%{?dist}
Summary:	Qt Movie thumbnailer

Group:		Amusements/Graphics
License:	GPLv3
URL:		http://gitlab.com/movie_thumbnailer/qmtn/wikis/
Source0:	https://gitlab.com/movie_thumbnailer/qmtn/repository/%{version}/archive.tar.gz

BuildRequires:	gcc-c++ qt5-qtbase-devel
Requires:	mtn qt5-qtbase

%description
Movie thumbnail generator written in Qt5

%prep
#wget -O %{name}.%{version}.tar.gz https://gitlab.com/movie_thumbnailer/qmtn/repository/archive.tar.gz?ref=%{version}
rm -rf ./*
tar -xf %SOURCE0
mv qmtn*/* ./

%build
qmake-qt5 INSTALL_ROOT=%{buildroot} mtngui.pro

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
