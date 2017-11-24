Name:		qmtn	
Version: 	0.2
Release:	1%{?dist}
Summary:	Qt Movie thumbnailer

Group:		Amusements/Graphics
License:	GPLv2
URL:		http://gitlab.com/movie_thumbnailer/qmtn/
Source0:	%{name}.%{version}.tar.gz                

BuildRequires:	qt5-qtbase-devel
Requires:	mtn

%description
Movie thumbnail generator written in Qt5

%prep
#wget -O %{name}.%{version}.tar.gz https://gitlab.com/movie_thumbnailer/qmtn/repository/archive.tar.gz?ref=%{version}
tar -xzf %SOURCE0

%build
cd qmtn*
qmake-qt5 INSTALL_ROOT=%{buildroot} mtngui.pro
%make_build

%install
cd qmtn*
make install INSTALL_ROOT=%{buildroot}

%files
%{_bindir}/qmtn
%{_datadir}/applications/%{name}.desktop
%{_datadir}/pixmaps/%{name}.png

%clean
rm -rf %{buildroot}
rm -rf *

%changelog
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
