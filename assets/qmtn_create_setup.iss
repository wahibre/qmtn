#define MyAppName "Qt Movie Thumbnailer"
#define MyAppVersion "0.3"
#define MyAppPublisher "Rusty pipe"
#define MyAppURL "https://gitlab.com/movie_thumbnailer/qmtn/wikis"
#define MyAppExeName "qmtn.exe"

[Setup]
AppId={{057D07D7-86B1-999F-AA92-2BE99092A9E4}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
;AppVerName={#MyAppName} {#MyAppVersion}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}
DefaultDirName={pf}\{#MyAppPublisher}\{#MyAppName}
DefaultGroupName={#MyAppName}
AllowNoIcons=true
OutputDir=.
OutputBaseFilename=qmtn_setup_{#MyAppVersion}
SourceDir=release
SetupIconFile=qmtn.ico
Compression=lzma
SolidCompression=true

[Languages]
Name: english; MessagesFile: compiler:Default.isl
;Name: slovak; MessagesFile: compiler:Languages\Slovak.isl

[Tasks]
Name: desktopicon; Description: {cm:CreateDesktopIcon}; GroupDescription: {cm:AdditionalIcons}; Flags: unchecked
Name: quicklaunchicon; Description: {cm:CreateQuickLaunchIcon}; GroupDescription: {cm:AdditionalIcons}; Flags: unchecked; OnlyBelowVersion: 0,6.1

[Files]
; NOTE: Don't use "Flags: ignoreversion" on any shared system files
Source: D3Dcompiler_47.dll; DestDir: {app}
Source: libEGL.dll; DestDir: {app}
Source: libgcc_s_dw2-1.dll; DestDir: {app}
Source: libGLESV2.dll; DestDir: {app}
Source: libstdc++-6.dll; DestDir: {app}
Source: libwinpthread-1.dll; DestDir: {app}
Source: opengl32sw.dll; DestDir: {app}
Source: qmtn.exe; DestDir: {app}
Source: Qt5Core.dll; DestDir: {app}
Source: Qt5Gui.dll; DestDir: {app}
Source: Qt5Svg.dll; DestDir: {app}
Source: Qt5Widgets.dll; DestDir: {app}
Source: platforms\qwindows.dll; DestDir: {app}\platforms\
Source: imageformats\qico.dll; DestDir: {app}\imageformats\
Source: imageformats\qjpeg.dll; DestDir: {app}\imageformats\
Source: imageformats\qsvg.dll; DestDir: {app}\imageformats\
Source: translations\qt_ca.qm; DestDir: {app}\translations\
Source: translations\qt_cs.qm; DestDir: {app}\translations\
Source: translations\qt_de.qm; DestDir: {app}\translations\
Source: translations\qt_en.qm; DestDir: {app}\translations\
Source: translations\qt_fi.qm; DestDir: {app}\translations\
Source: translations\qt_fr.qm; DestDir: {app}\translations\
Source: translations\qt_he.qm; DestDir: {app}\translations\
Source: translations\qt_hu.qm; DestDir: {app}\translations\
Source: translations\qt_it.qm; DestDir: {app}\translations\
Source: translations\qt_ja.qm; DestDir: {app}\translations\
Source: translations\qt_ko.qm; DestDir: {app}\translations\
Source: translations\qt_lv.qm; DestDir: {app}\translations\
Source: translations\qt_pl.qm; DestDir: {app}\translations\
Source: translations\qt_ru.qm; DestDir: {app}\translations\
Source: translations\qt_sk.qm; DestDir: {app}\translations\
Source: translations\qt_uk.qm; DestDir: {app}\translations\

[Icons]
Name: {group}\{#MyAppName}; Filename: {app}\{#MyAppExeName}
Name: {group}\{cm:UninstallProgram,{#MyAppName}}; Filename: {uninstallexe}
Name: {commondesktop}\{#MyAppName}; Filename: {app}\{#MyAppExeName}; Tasks: desktopicon
Name: {userappdata}\Microsoft\Internet Explorer\Quick Launch\{#MyAppName}; Filename: {app}\{#MyAppExeName}; Tasks: quicklaunchicon

[Run]
Filename: {app}\{#MyAppExeName}; Description: {cm:LaunchProgram,{#MyAppName}}; Flags: nowait postinstall skipifsilent

[Dirs]
Name: {app}\platforms
Name: {app}\imageformats
Name: {app}\translations
