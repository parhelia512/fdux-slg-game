/*
 * �ļ���:Common.h
 * ����ʱ��:2010:10:14   16:09
 * ��д��:winsock
 * ����:  ȫ�ֹ��������ļ�
 */

#pragma once

const int MAX_MAPSIZE=64;//��ͼ���64*64

enum EditorID//�༭��ID
{
	NoneID,
	BlockID,
	GroundID
};

enum GridID
{
	MainGrid,
	SecGrid
};

enum EditorMode//�༭���༭ģʽ
{
	PutMode,//����ģʽ
	RemoveMode//�Ƴ�ģʽ
};

enum MouseButton//��갴��
{
	LeftButton,
	MidButton,
	RightButton
};

enum ConerType//����ͼ��λ��
{
	TopRight = 0,
	TopLeft = 1,
	BottomLeft = 2,
	BottomRight = 3,
	ConerTypeCount=4
};

enum GroundType//��ͼ
{
	GroundDisable=-1,
	GreenLand = 0,
	Desert = 1,
	Swamp = 2,
	Snow = 3
};

enum BlockType
{
	Water,
	LowGround,
	HighGround,
	HighGroundPlane,
	Ramp,
	Planish
};

enum QueryMark//��ѯ����
{
	DEFAULT_QUERY_MARK=1<<1,
	ARROW_QUERY_MARK=1<<2
};

static const int DefaultHeight=0;//Ĭ�ϸ߶�
static const int VERTEX_POS_BINDING=0;//����λ�ð�λ��
static const int VERTEX_UV_BINDING=1;//��ͼ�����λ��
static const int TEXTURE_COUNT=4;//��ͼ������
static const int VERTEX_QUAD = 4;//ÿһ�����ο�ӵ�еĶ�����
static const int VERTEX_PREQUAD = 6;//ÿһ�����ο��������

static const float TEXTURE_SIZE=512.0f;//��ͼ��С
static const float TEXTURE_BLOCK_SIZE=128.0f;//��ͼ���С
