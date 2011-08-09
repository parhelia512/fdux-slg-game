; �ű��� Inno Setup �ű��� ���ɣ�
; �йش��� Inno Setup �ű��ļ�����ϸ��������İ����ĵ���

#define MyAppName "��ȴ��ս��-���԰�"
#define MyAppVersion "1.5"
#define MyAppPublisher "�þ�������"
#define MyAppURL "http://fdux.xoom.it/"
#define MyAppExeName "ForgottenBattlefield.exe"

[Setup]
; ע: AppId��ֵΪ������ʶ��Ӧ�ó���
; ��ҪΪ������װ����ʹ����ͬ��AppIdֵ��
; (�����µ�GUID����� ����|��IDE������GUID��)
AppId={{B1B68475-2D66-485E-BFE7-993B05E84AD5}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
;AppVerName={#MyAppName} {#MyAppVersion}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}
DefaultDirName={pf}\ForgottenBattlefield
DefaultGroupName={#MyAppName}
OutputDir=F:\out1
OutputBaseFilename=setup
Compression=lzma
SolidCompression=yes

[Languages]
Name: "chinesesimp"; MessagesFile: "compiler:Default.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked; OnlyBelowVersion: 0,6.1

[Files]
Source: "F:\C++\SLGGame\Demo\*"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs createallsubdirs
; ע��: ��Ҫ���κι���ϵͳ�ļ���ʹ�á�Flags: ignoreversion��

[Icons]
Name: {group}\{#MyAppName}������; Filename: {app}\Release\Launcher.exe; WorkingDir: {app}\Release; 
Name: {group}\{#MyAppName}; Filename: {app}\Release\{#MyAppExeName}; WorkingDir: {app}\Release; 
Name: {group}\{#MyAppName}(ȫ��ģʽ); Filename: "{app}\Release\{#MyAppExeName}";Parameters:"-FullScene"; WorkingDir: {app}\Release;
Name: {group}\��Ϸ�ֲ�; Filename: {app}\��Ϸ�ֲ�.doc; WorkingDir: {app}; 
Name: {group}\ReadMe; Filename: {app}\readme����ľСJJ.txt; WorkingDir: {app}\; 
Name: "{group}\{cm:UninstallProgram,{#MyAppName}}"; Filename: "{uninstallexe}"
Name: {userdesktop}\{#MyAppName}; Filename: {app}\Release\Launcher.exe; WorkingDir: {app}\Release; 

[Run]
Filename: {app}\readme����ľСJJ.txt; Description: "�鿴��ʾ�����ļ�"; Flags: postinstall skipifsilent shellexec;
Filename: {app}\Support\vcredist_x86.exe; Description: "��װVC���п�"; Flags: nowait postinstall skipifsilent; 
Filename: {app}\Support\DirectX\DXSETUP.exe; Description: "��װDirectX9���п�"; Flags: nowait postinstall skipifsilent;
