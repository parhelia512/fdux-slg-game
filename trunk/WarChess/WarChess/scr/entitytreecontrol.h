/*
 * �ļ���:entitytreecontrol.h
 * ����ʱ��:2010:10:28   16:57
 * ��д��:winsock
 * ����: EntityTree������,�����ⲿ����
 */

#ifndef ENTITYTREECONTROL_H
#define ENTITYTREECONTROL_H

#include <QObject>
#include <QTreeView>
#include <Ogre.h>

#include "EntityTreeModel.h"

class ObjcetControl;

class TerrainSystem;

class EntityDataImp;

class EntityTreeControl : public QObject
{
	Q_OBJECT

public:
	EntityTreeControl(QTreeView* treeView,ObjcetControl* objectControl);
	~EntityTreeControl();

	void open(QString filename);
	void save(QString filename);
	bool addGroup(QString name);
	bool removeGroup(QString name);

public slots:
	void addEntity();
	void removeEntity();

	void mouseMove(QMouseEvent * event);//����������Ⱦ���ڵ������Ϣ
	void mousePress(QMouseEvent * event);
	void mouseRelease(QMouseEvent * event);

	void selectEntity(const QModelIndex & index );//���û�ѡ��ĳEntityItemʱ
private:
	void LoadXML(QDomDocument doc);//��xml���뵽����
	QDomDocument SaveXML();//���������xml
	void cancelSelect();//ȡ��ѡ��

	QTreeView* mTreeView;
	EntityTreeModel* mEntityTreeModel;
	ObjcetControl* mObjectControl;
	Ogre::SceneNode* mSelectNode;//�Ѿ���ѡ�е���
	Ogre::Entity* mSelectEntity;//�ڵ�ʹ�õ�ʵ��
	EntityDataImp* mEntityItemData;//ѡ�е������������
	Ogre::SceneManager* mSceneManager;
	TerrainSystem* mTerrain;
	
};

#endif // ENTITYTREECONTROL_H
