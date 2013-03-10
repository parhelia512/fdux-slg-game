; �ű��� Inno Setup �ű��� ���ɣ�
; �йش��� Inno Setup �ű��ļ�����ϸ��������İ����ĵ���

#define MyAppName "��ȴ��ս��-������"
#define MyAppVersion "609"
#define MyAppPublisher "�þ�������"
#define MyAppURL "http://fdux.sinaapp.com/"
#define MyAppExeName "ForgottenBattlefield.exe"

[Setup]
; ע: AppId��ֵΪ������ʶ��Ӧ�ó���
; ��ҪΪ������װ����ʹ����ͬ��AppIdֵ��
; (�����µ�GUID����� ����|��IDE������GUID��)
AppID={{B1B68475-2D66-485E-BFE7-993B05E84AD5}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
;AppVerName={#MyAppName} {#MyAppVersion}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}
DefaultDirName={pf}\ForgottenBattlefield
DefaultGroupName={#MyAppName}
OutputDir=F:\C++\SLGGame\Deploy
OutputBaseFilename=setup
Compression=lzma/Max
SolidCompression=true
WizardImageFile=F:\C++\SLGGame\Deploy\Title.bmp
AppVerName=v1.0

[Languages]
Name: "chinesesimp"; MessagesFile: "compiler:Default.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked; OnlyBelowVersion: 0,6.1

[Files]
Source: "F:\C++\SLGGame\Deploy\CD11\*"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs createallsubdirs
; ע��: ��Ҫ���κι���ϵͳ�ļ���ʹ�á�Flags: ignoreversion��

[Icons]
Name: {group}\{#MyAppName}������; Filename: {app}\Release\Launcher.exe; WorkingDir: {app}\Release; 
Name: {group}\{#MyAppName}; Filename: {app}\Release\{#MyAppExeName}; WorkingDir: {app}\Release; 
Name: {group}\{#MyAppName}(ȫ��ģʽ); Filename: "{app}\Release\{#MyAppExeName}";Parameters:"-FullScene"; WorkingDir: {app}\Release;
Name: {group}\ReadMe; Filename: {app}\readme����ľ��СJJ.txt; WorkingDir: {app}\; 
Name: "{group}\{cm:UninstallProgram,{#MyAppName}}"; Filename: "{uninstallexe}"
Name: {userdesktop}\{#MyAppName}; Filename: {app}\Release\Launcher.exe; WorkingDir: {app}\Release; 

[Run]
Filename: {app}\readme����ľ��СJJ.txt; Description: �鿴��ʾ�����ļ�; Flags: postinstall skipifsilent shellexec; 
Filename: {app}\Support\vcredist_x86.exe; Description: ��װVC++2008���п�(���鰲װ); Flags: nowait postinstall skipifsilent; 
Filename: {app}\Support\DirectX\DXSETUP.exe; Description: "��װDirectX9���п�"; Flags: nowait postinstall skipifsilent;
Filename: {app}\Support\vcredist2010_x86.exe; Description: ��װVC++2010���п�(���鰲װ); Flags: PostInstall NoWait SkipIfSilent; 
Filename: {app}\Support\K-Lite_Codec_Pack_975_Full.exe; Description: ��װ��Ƶ������(XP���밲װ); Flags: PostInstall NoWait SkipIfSilent;
