#ifndef OBJCETCONTROL_H
#define OBJCETCONTROL_H

#include <QObject>
#include <QTreeView>
#include <QMouseEvent>
#include <QAction>
#include <QInputDialog>

#include <Ogre.h>

#include "EntityTreeModel.h"
#include "CollisionTools.h"

class ObjectData;

class AttributeControl;

class ObjcetControl : public QObject
{
	Q_OBJECT

public:
	ObjcetControl(QTreeView* treeView,AttributeControl* attribute);
	~ObjcetControl();

	void addObject(QString name,QString groupName,QString entityType,Ogre::SceneNode* objcetNode,Ogre::Entity* objectEntity, const QMap<QString, QString>& attrMap);//���һ���������󵽳�����,�������ʵ���д���

	QAction* mMoveAction;
	QAction* mRotateAction;
	QAction* mScaleAction;

public slots:
	void removeObject();//�Ƴ�һ������ʵ��

	void addObjectGroup();//���һ��������
	void removeObjectGroup();//�Ƴ�һ��������

	void mouseMove(QMouseEvent * event);//����������Ⱦ���ڵ������Ϣ
	void mousePress(QMouseEvent * event);
	void mouseRelease(QMouseEvent * event);
	void cameraMove(float x,float y,float z);

	void moveAction(bool checked);//�����϶�Ӧ��3����ť
	void rotateAction(bool checked);
	void scaleAction(bool checked);
	void moveLockAction(bool checked);
	void scaleLockAction(bool checked);

	void attributeChangle(QString name,QString value);

private:
	AttributeControl* mAttribute;//���������ƶ���

	EntityTreeModel* mObjectModel;//����ģ��
	QTreeView* mTreeView;//������ͼ

	float mLastX,mLastY;//��һ�����λ��

	ObjectData* mSelectObject;//��ǰѡ��Ķ���

	enum ObjectEditMode//����ı༭ģʽ
	{
		NoneEdit,
		Move,
		Rotate,
		Scale
	};

	enum ArrowType//����ı༭ģʽ
	{
		NoneArrow,
		ArrowX,
		ArrowY,
		ArrowZ
	};
	
	ObjectEditMode mEditMode;
	
	Ogre::SceneManager* mSceneManager;
	Ogre::RenderWindow* mWindow;
	Ogre::Camera* mCamera;
	Ogre::RaySceneQuery* mRaySceneQuery;
	MOC::CollisionTools* mCollisionTools;
	Ogre::SceneNode* mArrowNode;//��ͷ�ڵ�
	Ogre::SceneNode* mArrowXNode;//��ͷX�ڵ�
	Ogre::SceneNode* mArrowYNode;//��ͷY�ڵ�
	Ogre::SceneNode* mArrowZNode;//��ͷZ�ڵ�
	Ogre::Entity* mArrowXEntity;//X��ͷ
	Ogre::Entity* mArrowYEntity;//Y��ͷ
	Ogre::Entity* mArrowZEntity;//Z��ͷ

	
	void moveObject(Ogre::Vector3 tagetPos);//�ƶ���ǰѡ�����
	void rotateObject(int mouseY);//��ת��ǰѡ�����
	void scaleObject(int mouseY);//���ŵ�ǰѡ�����
	void createAssistPlane(ArrowType type,Ogre::Vector3 pos);//�����ƶ�����ƽ��
	void setAttribute();//��ѡ�еĶ�����������õ����������;
	Ogre::Plane* mAssistPlane;//���ƶ���������
	Ogre::Vector3 mLastPosition;//��һ�ε�λ��
	bool mMoveLocked;//�Ƿ��ƶ���ʱ����뵽����
	bool mScaleLocked;//�Ƿ�ʹ��ȫ������
	int mLastMouseY;//��һ�����Y��
	ArrowType mPressArrow;//��ǰ���µ���

	void setArrowStyle();//���ü�ͷ����ʽ

	EntityTreeItem* findEntity(const QModelIndex& index,const Ogre::Entity* entity);//�ݹ�ƥ��ʵ��

};

#endif // OBJCETCONTROL_H
