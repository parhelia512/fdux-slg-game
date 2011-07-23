#pragma once

#include <string>

enum GUIImageLayer
{
	EffectsLayer,
	LeftLayer,
	MidLayer,
	RightLayer,
	BackGroundLayer,
	AllLayer
};

struct GUIDialogAttribute
{
	unsigned int TextLeft;
	unsigned int TextTop;
	unsigned int TextWidth;
	unsigned int TextHeight;

	std::string TextBgImage;
	unsigned int TextBgLeft;
	unsigned int TextBgTop;
	unsigned int TextBgWidth;
	unsigned int TextBgHeight;

	unsigned int RoleNameLeft;
	unsigned int RoleNameTop;
	unsigned int RoleNameWidth;
	unsigned int RoleNameHeight;

	float TextRed;
	float TextGreen;
	float TextBlue;

	float RoleNameRed;
	float RoleNameGreen;
	float RoleNameBlue;

	std::string TextFont;
	std::string RoleFont;
};

struct GUIImageAttribute
{
	unsigned int LeftLayerX;
	unsigned int LeftLayerY;
	unsigned int MidLayerX;
	unsigned int MidLayerY;
	unsigned int RightLayerX;
	unsigned int RightLayerY;
};

enum GUISceneType
{
	NoneScene,
	StageScene,
	MenuScene,
	PUDebugScene,
	LoadingScene,
	BattleScene,
	SLScene,
	MenuWindowsScene,
	MessageBoxScene
};

#define DefaultDialogVisibleTime 0.5//Ĭ�϶Ի��򽥳�ʱ��
#define DefaultRoleNameVisibleTime 0.1//Ĭ�Ͻ�ɫ������ʱ��
#define DefaultShowTextTime 0.1//Ĭ�ϴ���Ч���ٶ�
#define DefaultCursorTime 0.01//Ĭ�Ϲ����˸Ƶ��
#define LineCursorFileName "LineCursor.png"//Ĭ���й���ļ���
#define PageCursorFileName "PageCursor.png"//Ĭ�ϻ�ҳ����ļ���
