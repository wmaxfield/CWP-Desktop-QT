; Script generated by the Inno Script Studio Wizard.
; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!

#define MyAppName "CWP"
#define MyAppVersion "1.0.1"
#define MyAppPublisher "Crossword Project"
#define MyAppURL "http://www.crosswordproject.com/"
#define MyAppExeName "CWP.exe"

[Setup]
; NOTE: The value of AppId uniquely identifies this application.
; Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
AppId={{720945AB-E5CB-4B5C-96F2-59D182BED640}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
;AppVerName={#MyAppName} {#MyAppVersion}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}
DefaultDirName={pf}\{#MyAppName}
DefaultGroupName={#MyAppName}
OutputDir=Z:\drop-folder\Dropbox\bible\CWP-QT1\innosetupScript
OutputBaseFilename=CWPsetup
Compression=lzma
SolidCompression=yes

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Files]
Source: "Z:\drop-folder\Dropbox\bible\CWP-QT1\build-CWP-Desktop_Qt_5_51_MinGW_32_bit-Release\CWP.app\CWP.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "Z:\drop-folder\Dropbox\bible\CWP-QT1\build-CWP-Desktop_Qt_5_51_MinGW_32_bit-Release\CWP.app\icudt54.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "Z:\drop-folder\Dropbox\bible\CWP-QT1\build-CWP-Desktop_Qt_5_51_MinGW_32_bit-Release\CWP.app\icuin54.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "Z:\drop-folder\Dropbox\bible\CWP-QT1\build-CWP-Desktop_Qt_5_51_MinGW_32_bit-Release\CWP.app\icuuc54.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "Z:\drop-folder\Dropbox\bible\CWP-QT1\build-CWP-Desktop_Qt_5_51_MinGW_32_bit-Release\CWP.app\libEGL.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "Z:\drop-folder\Dropbox\bible\CWP-QT1\build-CWP-Desktop_Qt_5_51_MinGW_32_bit-Release\CWP.app\libgcc_s_dw2-1.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "Z:\drop-folder\Dropbox\bible\CWP-QT1\build-CWP-Desktop_Qt_5_51_MinGW_32_bit-Release\CWP.app\libGLESV2.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "Z:\drop-folder\Dropbox\bible\CWP-QT1\build-CWP-Desktop_Qt_5_51_MinGW_32_bit-Release\CWP.app\libstdc++-6.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "Z:\drop-folder\Dropbox\bible\CWP-QT1\build-CWP-Desktop_Qt_5_51_MinGW_32_bit-Release\CWP.app\libwinpthread-1.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "Z:\drop-folder\Dropbox\bible\CWP-QT1\build-CWP-Desktop_Qt_5_51_MinGW_32_bit-Release\CWP.app\opengl32sw.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "Z:\drop-folder\Dropbox\bible\CWP-QT1\build-CWP-Desktop_Qt_5_51_MinGW_32_bit-Release\CWP.app\Qt5Core.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "Z:\drop-folder\Dropbox\bible\CWP-QT1\build-CWP-Desktop_Qt_5_51_MinGW_32_bit-Release\CWP.app\Qt5Gui.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "Z:\drop-folder\Dropbox\bible\CWP-QT1\build-CWP-Desktop_Qt_5_51_MinGW_32_bit-Release\CWP.app\Qt5Multimedia.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "Z:\drop-folder\Dropbox\bible\CWP-QT1\build-CWP-Desktop_Qt_5_51_MinGW_32_bit-Release\CWP.app\Qt5MultimediaWidgets.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "Z:\drop-folder\Dropbox\bible\CWP-QT1\build-CWP-Desktop_Qt_5_51_MinGW_32_bit-Release\CWP.app\Qt5Network.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "Z:\drop-folder\Dropbox\bible\CWP-QT1\build-CWP-Desktop_Qt_5_51_MinGW_32_bit-Release\CWP.app\Qt5OpenGL.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "Z:\drop-folder\Dropbox\bible\CWP-QT1\build-CWP-Desktop_Qt_5_51_MinGW_32_bit-Release\CWP.app\Qt5Positioning.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "Z:\drop-folder\Dropbox\bible\CWP-QT1\build-CWP-Desktop_Qt_5_51_MinGW_32_bit-Release\CWP.app\Qt5PrintSupport.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "Z:\drop-folder\Dropbox\bible\CWP-QT1\build-CWP-Desktop_Qt_5_51_MinGW_32_bit-Release\CWP.app\Qt5Qml.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "Z:\drop-folder\Dropbox\bible\CWP-QT1\build-CWP-Desktop_Qt_5_51_MinGW_32_bit-Release\CWP.app\Qt5Quick.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "Z:\drop-folder\Dropbox\bible\CWP-QT1\build-CWP-Desktop_Qt_5_51_MinGW_32_bit-Release\CWP.app\Qt5Sensors.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "Z:\drop-folder\Dropbox\bible\CWP-QT1\build-CWP-Desktop_Qt_5_51_MinGW_32_bit-Release\CWP.app\Qt5Sql.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "Z:\drop-folder\Dropbox\bible\CWP-QT1\build-CWP-Desktop_Qt_5_51_MinGW_32_bit-Release\CWP.app\Qt5Svg.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "Z:\drop-folder\Dropbox\bible\CWP-QT1\build-CWP-Desktop_Qt_5_51_MinGW_32_bit-Release\CWP.app\Qt5WebChannel.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "Z:\drop-folder\Dropbox\bible\CWP-QT1\build-CWP-Desktop_Qt_5_51_MinGW_32_bit-Release\CWP.app\Qt5WebKit.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "Z:\drop-folder\Dropbox\bible\CWP-QT1\build-CWP-Desktop_Qt_5_51_MinGW_32_bit-Release\CWP.app\Qt5WebKitWidgets.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "Z:\drop-folder\Dropbox\bible\CWP-QT1\build-CWP-Desktop_Qt_5_51_MinGW_32_bit-Release\CWP.app\Qt5Widgets.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "Z:\drop-folder\Dropbox\bible\CWP-QT1\build-CWP-Desktop_Qt_5_51_MinGW_32_bit-Release\CWP.app\audio\*"; DestDir: "{app}\audio"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "Z:\drop-folder\Dropbox\bible\CWP-QT1\build-CWP-Desktop_Qt_5_51_MinGW_32_bit-Release\CWP.app\bearer\*"; DestDir: "{app}\bearer"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "Z:\drop-folder\Dropbox\bible\CWP-QT1\build-CWP-Desktop_Qt_5_51_MinGW_32_bit-Release\CWP.app\CWPfiles\*"; DestDir: "{app}\CWPfiles"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "Z:\drop-folder\Dropbox\bible\CWP-QT1\build-CWP-Desktop_Qt_5_51_MinGW_32_bit-Release\CWP.app\iconengines\*"; DestDir: "{app}\iconengines"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "Z:\drop-folder\Dropbox\bible\CWP-QT1\build-CWP-Desktop_Qt_5_51_MinGW_32_bit-Release\CWP.app\imageformats\*"; DestDir: "{app}\imageformats"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "Z:\drop-folder\Dropbox\bible\CWP-QT1\build-CWP-Desktop_Qt_5_51_MinGW_32_bit-Release\CWP.app\mediaservice\*"; DestDir: "{app}\mediaservice"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "Z:\drop-folder\Dropbox\bible\CWP-QT1\build-CWP-Desktop_Qt_5_51_MinGW_32_bit-Release\CWP.app\platforms\*"; DestDir: "{app}\platforms"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "Z:\drop-folder\Dropbox\bible\CWP-QT1\build-CWP-Desktop_Qt_5_51_MinGW_32_bit-Release\CWP.app\playlistformats\*"; DestDir: "{app}\playlistformats"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "Z:\drop-folder\Dropbox\bible\CWP-QT1\build-CWP-Desktop_Qt_5_51_MinGW_32_bit-Release\CWP.app\position\*"; DestDir: "{app}\position"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "Z:\drop-folder\Dropbox\bible\CWP-QT1\build-CWP-Desktop_Qt_5_51_MinGW_32_bit-Release\CWP.app\printsupport\*"; DestDir: "{app}\printsupport"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "Z:\drop-folder\Dropbox\bible\CWP-QT1\build-CWP-Desktop_Qt_5_51_MinGW_32_bit-Release\CWP.app\sensorgestures\*"; DestDir: "{app}\sensorguestures"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "Z:\drop-folder\Dropbox\bible\CWP-QT1\build-CWP-Desktop_Qt_5_51_MinGW_32_bit-Release\CWP.app\sensors\*"; DestDir: "{app}\sensors"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "Z:\drop-folder\Dropbox\bible\CWP-QT1\build-CWP-Desktop_Qt_5_51_MinGW_32_bit-Release\CWP.app\sqldrivers\*"; DestDir: "{app}\sqldrivers"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "Z:\drop-folder\Dropbox\bible\CWP-QT1\build-CWP-Desktop_Qt_5_51_MinGW_32_bit-Release\CWP.app\translations\*"; DestDir: "{app}\translations"; Flags: ignoreversion recursesubdirs createallsubdirs
; NOTE: Don't use "Flags: ignoreversion" on any shared system files

[Icons]
Name: "{group}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"
Name: "{commondesktop}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: desktopicon

[Run]
Filename: "{app}\{#MyAppExeName}"; Description: "{cm:LaunchProgram,{#StringChange(MyAppName, '&', '&&')}}"; Flags: nowait postinstall skipifsilent

[Dirs]
Name: "{app}\images"
