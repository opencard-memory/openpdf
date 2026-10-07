#define MyAppName "OpenPDF Office"
#define MyAppVersion "0.3.0"
#define MyAppPublisher "OpenPDF Office Project"
#define MyAppExeName "OpenPDFOffice.exe"

[Setup]
AppId={{8C59A139-04E5-4AB8-96E0-E488EC69AEF8}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
AppPublisher={#MyAppPublisher}
DefaultDirName={autopf}\OpenPDF Office
DefaultGroupName=OpenPDF Office
ArchitecturesAllowed=x64compatible
ArchitecturesInstallIn64BitMode=x64compatible
PrivilegesRequired=admin
OutputDir=output
OutputBaseFilename=OpenPDFOffice-Setup-x64
Compression=lzma2/ultra64
SolidCompression=yes
WizardStyle=modern
LicenseFile=..\licenses\사용한_오픈소스.txt
InfoAfterFile=..\licenses\사용한_오픈소스.txt
UninstallDisplayIcon={app}\{#MyAppExeName}

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"
Name: "korean"; MessagesFile: "compiler:Languages\Korean.isl"
Name: "spanish"; MessagesFile: "compiler:Languages\Spanish.isl"

[Tasks]
Name: "desktopicon"; Description: "바탕 화면에 바로 가기 만들기"; GroupDescription: "추가 작업:"
Name: "fontalllanguages"; Description: "Install the all-language offline font pack"; GroupDescription: "Offline fonts:"; Flags: checkedonce

[Files]
Source: "..\dist\*"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "..\licenses\사용한_오픈소스.txt"; DestDir: "{app}\licenses"; Flags: ignoreversion
Source: "..\config\google_fonts_api.dev.json"; DestDir: "{app}\config"; Flags: ignoreversion
Source: "..\fontpacks\all\*"; DestDir: "{app}\fonts"; Flags: ignoreversion recursesubdirs createallsubdirs skipifsourcedoesntexist; Tasks: fontalllanguages

[Icons]
Name: "{group}\OpenPDF Office"; Filename: "{app}\{#MyAppExeName}"
Name: "{group}\사용한 오픈소스"; Filename: "{app}\licenses\사용한_오픈소스.txt"
Name: "{autodesktop}\OpenPDF Office"; Filename: "{app}\{#MyAppExeName}"; Tasks: desktopicon

[Run]
Filename: "{app}\{#MyAppExeName}"; Description: "OpenPDF Office 실행"; Flags: nowait postinstall skipifsilent
