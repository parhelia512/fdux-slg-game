#pragma once

#include <vector>

enum BasicAttr
{
	ATTR_ATTACK = 0, //��ս����ֵ
	ATTR_RANGEDATTACK =1, //Զ�̹���ֵ
	ATTR_DEFENSE = 2, //����ֵ
	ATTR_FORMATION = 3, //����ֵ
	ATTR_INITIATIVE = 4, //�ȹ�ֵ
	ATTR_ACTIONPOINT = 5, //�ж�ֵ
	ATTR_DETECTION = 6, //���ֵ
	ATTR_COVERT = 7,  //����ֵ
	ATTR_INJURY = 8,  //��������ϵ��
	ATTR_COUNTER = 9,	//����ϵ��
	ATTR_MAX = 10
};

typedef std::vector<int> AttrVector[ATTR_MAX];

enum ModifyType
{
	MODIFIER_BASIC = 0, //����������ֱ�ӵ���
	MODIFIER_RESISTANCE = 1, //�ֿ�����������������ֵ��������������ᳬ����ֵ����
};

struct AttrModifier
{
	AttrVector attr;
	ModifyType type;
};
