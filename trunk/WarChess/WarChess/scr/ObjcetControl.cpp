#include "ObjcetControl.h"

#include "IIRoot.h"

#include "ObjectData.h"

#include "AttributeControl.h"

#include "ObjcetItemDelegate.h"

#include <QMessageBox>

#include <QHash>

ObjcetControl::ObjcetControl(QTreeView* treeView,AttributeControl* attribute)
:	mLastX(0.0),
	mLastY(0.0),
	mSelectObject(NULL),
	mEditMode(NoneEdit),
	mTreeView(treeView),
	mPressArrow(NoneArrow),
	mAssistPlane(NULL),
	mMoveLocked(false),
	mAttribute(attribute)
{
	//����ģ����,��ģ�����õ���ͼ��
	mObjectModel=new EntityTreeModel();
	mTreeView->setModel(mObjectModel);
	ObjcetItemDelegate* entitiyItemDelegate=new ObjcetItemDelegate(NULL);
	entitiyItemDelegate->entityTreeModel=mObjectModel;
	entitiyItemDelegate->treeView=mTreeView;
	mTreeView->setItemDelegate(entitiyItemDelegate);
	
	mObjectModel->getRootItem()->appendChild(new EntityTreeItem(new EntityData("�ر����"),mObjectModel->getRootItem(),EntityItemType::Group));//�������Ĭ�ϵ���
	mObjectModel->getRootItem()->appendChild(new EntityTreeItem(new EntityData("�ر���Ч"),mObjectModel->getRootItem(),EntityItemType::Group));
	mObjectModel->getRootItem()->appendChild(new EntityTreeItem(new EntityData("����1��λ"),mObjectModel->getRootItem(),EntityItemType::Group));
	mObjectModel->getRootItem()->appendChild(new EntityTreeItem(new EntityData("����2��λ"),mObjectModel->getRootItem(),EntityItemType::Group));
	mObjectModel->getRootItem()->appendChild(new EntityTreeItem(new EntityData("����3��λ"),mObjectModel->getRootItem(),EntityItemType::Group));
	mObjectModel->getRootItem()->appendChild(new EntityTreeItem(new EntityData("����4��λ"),mObjectModel->getRootItem(),EntityItemType::Group));
	mSceneManager=IIRoot::getSingletonPtr()->mSceneMgr;
	mCamera=IIRoot::getSingletonPtr()->mCamera;
	mWindow=IIRoot::getSingletonPtr()->mWindow;
	mRaySceneQuery=new Ogre::DefaultRaySceneQuery(mSceneManager);
	mCollisionTools=new MOC::CollisionTools(mSceneManager);

	//������ͷ�ڵ�,��ʼ��XYZ��
	mArrowNode=mSceneManager->getRootSceneNode()->createChildSceneNode();

	mArrowXNode=mArrowNode->createChildSceneNode();
	mArrowXEntity=mSceneManager->createEntity("ArrowX","scbArrowEx.mesh");
	mArrowXNode->attachObject(mArrowXEntity);
	mArrowXEntity->setMaterialName("ArrowX");
	mArrowXEntity->setQueryFlags(ARROW_QUERY_MARK);//���ò�ѯ����
	mArrowXNode->yaw(Ogre::Radian(Ogre::Degree(-90)));
	mArrowXEntity->setRenderQueueGroupAndPriority(mSceneManager->getRenderQueue()->getDefaultQueueGroup()+4,mSceneManager->getRenderQueue()->getDefaultRenderablePriority()+4);//�����������Ⱦ���к���,������ͻ������е�ģ����ǰ��

	mArrowYNode=mArrowNode->createChildSceneNode();
	mArrowYEntity=mSceneManager->createEntity("ArrowY","scbArrowEx.mesh");
	mArrowYNode->attachObject(mArrowYEntity);
	mArrowYEntity->setMaterialName("ArrowY");
	mArrowYEntity->setQueryFlags(ARROW_QUERY_MARK);
	mArrowYNode->pitch(Ogre::Radian(Ogre::Degree(90)));
	mArrowYEntity->setRenderQueueGroupAndPriority(mSceneManager->getRenderQueue()->getDefaultQueueGroup()+5,mSceneManager->getRenderQueue()->getDefaultRenderablePriority()+5);

	mArrowZNode=mArrowNode->createChildSceneNode();
	mArrowZEntity=mSceneManager->createEntity("ArrowZ","scbArrowEx.mesh");
	mArrowZNode->attachObject(mArrowZEntity);
	mArrowZEntity->setMaterialName("ArrowZ");
	mArrowZEntity->setQueryFlags(ARROW_QUERY_MARK);
	mArrowZEntity->setRenderQueueGroupAndPriority(mSceneManager->getRenderQueue()->getDefaultQueueGroup()+4,mSceneManager->getRenderQueue()->getDefaultRenderablePriority()+4);

	mArrowNode->setVisible(false);

	this->connect(mAttribute,SIGNAL(attributeChangle(QString,QString)),this,SLOT(attributeChangle(QString,QString)));//�����������ƶ�������
}

ObjcetControl::~ObjcetControl()
{
	delete mObjectModel;
}

void ObjcetControl::addObject( QString name,QString groupName,QString entityType,Ogre::SceneNode* objcetNode,Ogre::Entity* objectEntity,const QMap<QString, QString>& attrMap)
{
	EntityTreeItem* groupItem=mObjectModel->getRootItem()->findItem(groupName);//�õ���ڵ�
	if (groupItem!=NULL)
	{
		//���ﴫ�����ʵ�������,���ڳ��������ж��������ͬһ��ʵ��Ķ���,��������Ҫ���⴦��
		QVector<EntityTreeItem*> result;
		QVector<EntityTreeItem*>::Iterator it;
		QString objectName = name.indexOf('_') == -1 ? name : name.split('_').at(0);
		mObjectModel->getRootItem()->findItemAsSimpleName(objectName,&result);//�ȴ��б��в��Ҿ�����ͬ���ֵ���
		
		if (result.size()!=0)
		{
			int max=0;
			QStringList sl;
			for (it=result.begin();it!=result.end();it++)//�������,���Һ�׺����ֵ
			{
				sl=(*it)->data(0)->name.split("_");
				int value=sl.at(1).toInt();
				if (value>max)
				{
					max=value;
				}
				sl.clear();
			}
			objectName+=QString("_")+QString().setNum(max+1);
		}
		else
		{
			objectName+="_0";//���û�Ļ�,��0��ʼ����
		}
			
		ObjectData* objectData=new ObjectData(objectName);
		objectData->mNode=objcetNode;
		objectData->mEntity=objectEntity;
		objectData->EntityType = entityType;
		objectData->GroupName = groupName;
		if (groupName.startsWith(QString::fromStdString("����")) && attrMap.isEmpty())
		{
			objectData->map["direction"] = "1";
			objectData->map["numunit"] = "0";
			objectData->map["morale"] = "0";
		}
		if (groupName.startsWith(QString::fromStdString("����")) && !attrMap.isEmpty())
		{
			objectData->map["direction"] = attrMap.value("direction");
			objectData->map["numunit"] = attrMap.value("numunit");
			objectData->map["morale"] = attrMap.value("morale");
		}

		int GX,GY;
		IIRoot::getSingletonPtr()->mTerrain->calculateGrid(objcetNode->getPosition().x,objcetNode->getPosition().z,&GX,&GY);
		objectData->GridX=GX;
		objectData->GridY=GY;

		QString value=attrMap.value("ObjectDirection");
		Ogre::Quaternion q;

		if(value=="North")
		{
			q.FromAngleAxis(Ogre::Degree(180),Ogre::Vector3(0,1,0));
		}
		else if(value=="South")
		{
			q.FromAngleAxis(Ogre::Degree(360),Ogre::Vector3(0,1,0));
		}
		else if(value=="West")
		{
			q.FromAngleAxis(Ogre::Degree(270),Ogre::Vector3(0,1,0));
		}
		else if(value=="East")
		{
			q.FromAngleAxis(Ogre::Degree(90),Ogre::Vector3(0,1,0));
		}

		objectData->mNode->setOrientation(q);

		objectData->map["ObjectDirection"]=attrMap.value("ObjectDirection");

		groupItem->appendChild(new EntityTreeItem(objectData,groupItem,EntityItemType::Entity));
		mTreeView->reset();//�ı����ݺ�,�ǵ�֪ͨviewˢ��
		mTreeView->expand(mObjectModel->index(groupItem->row(),0));//չ����Ӧ����
	}
}

void ObjcetControl::removeObject()
{
	QItemSelectionModel* selectionModel =mTreeView->selectionModel();
	QModelIndex selectionIndex=selectionModel->currentIndex();

	if(selectionIndex.isValid())
	{
		EntityTreeItem* childItem=static_cast<EntityTreeItem*>(selectionIndex.internalPointer());
		if(childItem->entityItemType==EntityItemType::Entity)
		{
			QMessageBox::StandardButton button=QMessageBox::information(NULL,"��ʾ","���Ҫɾ���ö���?",(QMessageBox::Yes | QMessageBox::No));
			if(button==QMessageBox::Yes)
			{
				mObjectModel->removeRow(selectionIndex.row());
				childItem->parent()->removeChild(childItem);		
				mTreeView->reset();//�ı����ݺ�,�ǵ�֪ͨviewˢ��
				delete childItem;//�м�!����Ҫ��viewˢ�º�,����delete������
			}
		}
	}
}

void ObjcetControl::addObjectGroup()
{
	bool isOk;
	QString groupName=QInputDialog::getText(NULL,"���","�����µ�����",QLineEdit::Normal,QString(),&isOk);

	if(isOk)
	{

		EntityTreeItem* rootItem=mObjectModel->getRootItem();

		if(rootItem->findItem(groupName)!=NULL)
		{
			QMessageBox::critical(NULL,"����","�Ѿ��и�����");
			return;
		}

		rootItem->appendChild(new EntityTreeItem(new EntityData(groupName),rootItem,EntityItemType::Group));

		mTreeView->reset();//�ı����ݺ�,�ǵ�֪ͨviewˢ��
	}
}

void ObjcetControl::removeObjectGroup()
{
	QItemSelectionModel* selectionModel = mTreeView->selectionModel();
	QModelIndex selectionIndex=selectionModel->currentIndex();

	if(selectionIndex.isValid())
	{
		EntityTreeItem* childItem=static_cast<EntityTreeItem*>(selectionIndex.internalPointer());
		if(childItem->entityItemType==EntityItemType::Group)
		{
			QMessageBox::StandardButton button=QMessageBox::information(NULL,"��ʾ","���Ҫɾ������?���µ�ȫ��ʵ�嶼����ɾ��.",(QMessageBox::Yes | QMessageBox::No));
			if(button==QMessageBox::Yes)
			{
				mObjectModel->removeRow(selectionIndex.row());
				childItem->parent()->removeChild(childItem);		
				mTreeView->reset();//�ı����ݺ�,�ǵ�֪ͨviewˢ��
				delete childItem;//�м�!����Ҫ��viewˢ�º�,����delete������
			}
		}
	}
}

void ObjcetControl::mouseMove( QMouseEvent * event )
{
	if (mPressArrow!=NoneArrow && mEditMode!=NoneEdit)//����Ѿ���ĳ���ᰴ����,���ҿ�ʼĳ���༭����
	{

		switch(mEditMode)//���ݱ༭ģʽ���в���
		{
		case Move:
			{
				Ogre::Ray mouseRay = mCamera->getCameraToViewportRay(float(event->x())/float(mWindow->getWidth()),float(event->y())/float(mWindow->getHeight()));
				std::pair<bool,Ogre::Real> result;

				result = mouseRay.intersects(*mAssistPlane);//�븨��ƽ��Ľ���

				if (result.first)
				{
					moveObject(mouseRay.getPoint(result.second));
					mLastPosition=mouseRay.getPoint(result.second);
				}
				break;
			}
		case Rotate:
			{
				rotateObject(event->y());
				mLastMouseY=event->y();
				break;
			}
		case Scale:
			{
				scaleObject(event->y());
				mLastMouseY=event->y();
				break;
			}

		}

		setAttribute();
	}
}

void ObjcetControl::mousePress( QMouseEvent * event )
{
	if (event->button()==Qt::MouseButton::LeftButton)
	{
		//�����ѯ����
		Ogre::Ray mouseRay = mCamera->getCameraToViewportRay(float(event->x())/float(mWindow->getWidth()),float(event->y())/float(mWindow->getHeight()));
		
		Ogre::Entity* entity = NULL;
		Ogre::Vector3 result = Ogre::Vector3::ZERO;
		float distToColl;

		if (mSelectObject!=NULL)//��ǰ�Ѿ�ѡ����ĳ����
		{
			if (mCollisionTools->raycast(mouseRay,result,(Ogre::ulong&)entity,distToColl,ARROW_QUERY_MARK))//���ȼ���Ƿ�����
			{
				//QMessageBox::information(NULL,QString(entity->getName().c_str()),QString().setNum(entity->getQueryFlags()));
				if (entity->getName()=="ArrowX")
				{
					mPressArrow=ArrowX;
					createAssistPlane(ArrowX,entity->getParentNode()->getPosition());
				}
				else if (entity->getName()=="ArrowY")
				{
					mPressArrow=ArrowY;
					createAssistPlane(ArrowY,entity->getParentNode()->getPosition());
				}
				else if (entity->getName()=="ArrowZ")
				{
					mPressArrow=ArrowZ;
					createAssistPlane(ArrowZ,entity->getParentNode()->getPosition());
				}
				mLastPosition=entity->getParentSceneNode()->getPosition();
				mLastMouseY=event->y();
			}
			else//���û�е����,��ô���ѡ��
			{
				mSelectObject->mNode->showBoundingBox(false);//���ذ�Χ��
				mSelectObject=NULL;
				mPressArrow=NoneArrow;
				mArrowNode->setVisible(false);
				setAttribute();
			}
		}
		else//�����ǰû��ѡ����ĳ����
		{
			if (mCollisionTools->raycast(mouseRay,result,(Ogre::ulong&)entity,distToColl))//ʹ�þ�ȷ���ִ�в�ѯ
			{
				EntityTreeItem* item=NULL;
				//��ȡroot����Ľڵ�,���в���
				for (int i=0;i<mObjectModel->rowCount();i++)
				{
					item=findEntity(mObjectModel->index(i,0),entity);
					if (item!=NULL)
					{
						break;
					}
				}

				if(item!=NULL)//�ж���ѡ��
				{
					mSelectObject=static_cast<ObjectData*>(item->data(0));
					mSelectObject->mNode->showBoundingBox(true);//����ѡ�еĶ���,��ʾ���İ�Χ��
					if (mEditMode!=NoneEdit)//��������˱༭ģʽ
					{
						mArrowNode->setVisible(true);
						mArrowNode->setPosition(mSelectObject->mNode->getPosition());
					}
				}
				setAttribute();
			}
		}
	}

}

void ObjcetControl::mouseRelease( QMouseEvent * event )
{
	if (mSelectObject!=NULL)
	{
		Ogre::Vector3 v=mSelectObject->mNode->getPosition();
		int GX,GY;
		IIRoot::getSingletonPtr()->mTerrain->calculateGrid(v.x,v.z,&GX,&GY); 
		Ogre::Vector2 v1=IIRoot::getSingletonPtr()->mTerrain->getRealXY(GX,GY,SecGrid);
		float height=IIRoot::getSingletonPtr()->mTerrain->getHeight(GX,GY);
		mSelectObject->GridX=GX;
		mSelectObject->GridY=GY;

		if (mMoveLocked)//��������Ƶ�������
		{
			switch(mPressArrow)
			{
			case ArrowX:
				{
					v.x=v1.x;
					break;
				}
			case ArrowY:
				{
					v.y=height;
					break;
				}
			case ArrowZ:
				{
					v.z=v1.y;
					break;
				}
			}
			mSelectObject->mNode->setPosition(v);
			mArrowNode->setPosition(v);
		}

		mPressArrow=NoneArrow;

		setAttribute();
	}
}

void ObjcetControl::moveAction( bool checked )
{
	if (checked)
	{
		mEditMode=Move;
		mMoveAction->setChecked(true);
		mRotateAction->setChecked(false);
		mScaleAction->setChecked(false);
		setArrowStyle();

		if (mSelectObject!=NULL)//����Ѿ�ѡ����ĳ��������,��ʱ���°�ť��Ҫ��ʾ����
		{
			mArrowNode->setVisible(true);
			mArrowNode->setPosition(mSelectObject->mNode->getPosition());
		}
	}
	else
	{
		mEditMode=NoneEdit;
		mMoveAction->setChecked(false);
		mArrowNode->setVisible(false);
	}
}

void ObjcetControl::rotateAction( bool checked )
{
	if (checked)
	{
		mEditMode=Rotate;
		mMoveAction->setChecked(false);
		mRotateAction->setChecked(true);
		mScaleAction->setChecked(false);
		setArrowStyle();

		if (mSelectObject!=NULL)//����Ѿ�ѡ����ĳ��������,��ʱ���°�ť��Ҫ��ʾ����
		{
			mArrowNode->setVisible(true);
			mArrowNode->setPosition(mSelectObject->mNode->getPosition());
		}
	}
	else
	{
		mEditMode=NoneEdit;
		mRotateAction->setChecked(false);
		mArrowNode->setVisible(false);
	}
}

void ObjcetControl::scaleAction( bool checked )
{
	if (checked)
	{
		mEditMode=Scale;
		mMoveAction->setChecked(false);
		mRotateAction->setChecked(false);
		mScaleAction->setChecked(true);
		setArrowStyle();

		if (mSelectObject!=NULL)//����Ѿ�ѡ����ĳ��������,��ʱ���°�ť��Ҫ��ʾ����
		{
			mArrowNode->setVisible(true);
			mArrowNode->setPosition(mSelectObject->mNode->getPosition());
		}
	}
	else
	{
		mEditMode=NoneEdit;
		mScaleAction->setChecked(false);
		mArrowNode->setVisible(false);
	}
}

void ObjcetControl::moveObject(Ogre::Vector3 tagetPos)
{
	Ogre::Vector3 v=mArrowNode->getPosition();
	switch(mPressArrow)
	{
	case ArrowX:
		{
			v.x=tagetPos.x;
			break;
		}
	case ArrowY:
		{
			v.y=tagetPos.y;
			break;
		}
	case ArrowZ:
		{
			v.z=tagetPos.z;
			break;
		}
	}

	mArrowNode->setPosition(v);
	mSelectObject->mNode->setPosition(v);
	int GX,GY;
	IIRoot::getSingletonPtr()->mTerrain->calculateGrid(v.x,v.z,&GX,&GY); 
	mSelectObject->GridX=GX;
	mSelectObject->GridY=GY;
}

void ObjcetControl::rotateObject(int mouseY)
{
	int y=mLastMouseY-mouseY;
	switch(mPressArrow)
	{
	case ArrowX:
		{
			mSelectObject->mNode->pitch(Ogre::Radian(Ogre::Degree(y)));
			break;
		}
	case ArrowY:
		{
			mSelectObject->mNode->yaw(Ogre::Radian(Ogre::Degree(y)));
			break;
		}
	case ArrowZ:
		{
			mSelectObject->mNode->roll(Ogre::Radian(Ogre::Degree(y)));
			break;
		}
	}
}

void ObjcetControl::scaleObject(int mouseY)
{
	int y=mLastMouseY-mouseY;
	Ogre::Vector3 s=mSelectObject->mNode->getScale();
	if (!mScaleLocked)//�����ȫ������
	{
		switch(mPressArrow)
		{
		case ArrowX:
			{
				s.x+=y*0.1;
				break;
			}
		case ArrowY:
			{
				s.y+=y*0.1;
				break;
			}
		case ArrowZ:
			{
				s.z+=y*0.1;
				break;
			}
		}
	}
	else
	{
		s.x+=y*0.1;
		s.y+=y*0.1;
		s.z+=y*0.1;
	}
	mSelectObject->mNode->setScale(s);
}

void ObjcetControl::setArrowStyle()
{
	switch(mEditMode)
	{
	case Move:
		{
			mArrowXEntity->getSubEntity(1)->setVisible(true);
			mArrowXEntity->getSubEntity(2)->setVisible(false);
			mArrowXEntity->getSubEntity(3)->setVisible(false);

			mArrowYEntity->getSubEntity(1)->setVisible(true);
			mArrowYEntity->getSubEntity(2)->setVisible(false);
			mArrowYEntity->getSubEntity(3)->setVisible(false);

			mArrowZEntity->getSubEntity(1)->setVisible(true);
			mArrowZEntity->getSubEntity(2)->setVisible(false);
			mArrowZEntity->getSubEntity(3)->setVisible(false);
			break;
		}
	case Rotate:
		{
			mArrowXEntity->getSubEntity(1)->setVisible(false);
			mArrowXEntity->getSubEntity(2)->setVisible(true);
			mArrowXEntity->getSubEntity(3)->setVisible(false);

			mArrowYEntity->getSubEntity(1)->setVisible(false);
			mArrowYEntity->getSubEntity(2)->setVisible(true);
			mArrowYEntity->getSubEntity(3)->setVisible(false);

			mArrowZEntity->getSubEntity(1)->setVisible(false);
			mArrowZEntity->getSubEntity(2)->setVisible(true);
			mArrowZEntity->getSubEntity(3)->setVisible(false);
			break;
		}
	case Scale:
		{
			mArrowXEntity->getSubEntity(1)->setVisible(false);
			mArrowXEntity->getSubEntity(2)->setVisible(false);
			mArrowXEntity->getSubEntity(3)->setVisible(true);

			mArrowYEntity->getSubEntity(1)->setVisible(false);
			mArrowYEntity->getSubEntity(2)->setVisible(false);
			mArrowYEntity->getSubEntity(3)->setVisible(true);

			mArrowZEntity->getSubEntity(1)->setVisible(false);
			mArrowZEntity->getSubEntity(2)->setVisible(false);
			mArrowZEntity->getSubEntity(3)->setVisible(true);
			break;
		}
	}
}

void ObjcetControl::cameraMove( float x,float y,float z )
{
	//���ݼ�ͷ��������ľ���,��̬������ͷ�Ĵ�С
	Ogre::Vector3 camPos(x,y,z);
	camPos-=mArrowNode->getPosition();
	float camDistance=camPos.squaredLength();
	camDistance/=5000;
	if (camDistance<=1)
	{
		camDistance=1;
	}

	mArrowNode->setScale(camDistance,camDistance,camDistance);
}

EntityTreeItem* ObjcetControl::findEntity(const QModelIndex& index,const Ogre::Entity* entity )
{
	if (index.isValid())
	{
		EntityTreeItem* parentItem=static_cast<EntityTreeItem*>(index.internalPointer());//ȡ��index��Ӧ��Item��
		int childCount=parentItem->childCount();
		if (childCount!=0)//����������ӽڵ�
		{
			for (int i=0;i<childCount;i++)//�����ӽڵ�
			{
				EntityTreeItem* childItem=parentItem->child(i);
				if (childItem->entityItemType==Group)//����ӽڵ㻹��һ����
				{
					EntityTreeItem* result=findEntity(mObjectModel->index(i,0,index),entity);//�ݹ���в���

					if(result!=NULL)
					{
						return result;
					}
				}
				else//�ӽڵ���Item
				{
					ObjectData* data=static_cast<ObjectData*>(childItem->data(0));
					if (data->mEntity==entity)//ƥ���Ƿ�ΪҪ���ҵ�Entity
					{
						return childItem;
					}
				}
			}
		}

		return NULL;
	}
	else
	{
		return NULL;
	}

}

void ObjcetControl::createAssistPlane(ArrowType type,Ogre::Vector3 pos)
{
	switch(type)
	{
	case ArrowX://X��Ļ�,Ӧ�ù���һ����x��ƽ�е���(X-1000,Y+1000,Z),(X+1000,Y+1000,Z),(X-1000,Y-1000,Z)
		{
			Ogre::Vector3 v1(pos.x-1000,pos.y+1000,pos.z);
			Ogre::Vector3 v2(pos.x+1000,pos.y+1000,pos.z);
			Ogre::Vector3 v3(pos.x-1000,pos.y-1000,pos.z);
			mAssistPlane=new Ogre::Plane(v1,v2,v3);
			break;
		}
	case ArrowY://Y���鷳
		{
			Ogre::Vector3 v1(pos.x-1000,pos.y+1000,pos.z);
			Ogre::Vector3 v2(pos.x+1000,pos.y+1000,pos.z);
			Ogre::Vector3 v3(pos.x-1000,pos.y-1000,pos.z);
			mAssistPlane=new Ogre::Plane(v1,v2,v3);
			break;
		}
	case ArrowZ://��X����
		{
			Ogre::Vector3 v1(pos.x,pos.y+1000,pos.z-1000);
			Ogre::Vector3 v2(pos.x,pos.y+1000,pos.z+1000);
			Ogre::Vector3 v3(pos.x,pos.y-1000,pos.z-1000);
			mAssistPlane=new Ogre::Plane(v1,v2,v3);
			break;
		}

	}

}

void ObjcetControl::moveLockAction( bool checked )
{
	mMoveLocked=checked;
}

void ObjcetControl::scaleLockAction( bool checked )
{
	mScaleLocked=checked;
}

void ObjcetControl::setAttribute()
{
	if (mSelectObject!=NULL)
	{
		QMap<QString,QString> map;
		Ogre::Vector3 p=mSelectObject->mNode->getPosition();
		Ogre::Quaternion q=mSelectObject->mNode->getOrientation();
		Ogre::Vector3 s=mSelectObject->mNode->getScale();
		map["Name#string"]=mSelectObject->name;
		map["Type#string"]=mSelectObject->EntityType;

		Ogre::Vector2 gridPosition=IIRoot::getSingletonPtr()->mTerrain->getRealXY(mSelectObject->GridX,mSelectObject->GridY,SecGrid); 
		float height=IIRoot::getSingletonPtr()->mTerrain->getHeight(mSelectObject->GridX,mSelectObject->GridY);
		map["OffsetX#float"]=QString().setNum(p.x-gridPosition.x);
		map["OffsetY#float"]=QString().setNum(p.y-height);
		map["OffsetZ#float"]=QString().setNum(p.z-gridPosition.y);

		QString gXString=QString("GridX#integer|0|%1").arg(IIRoot::getSingletonPtr()->mTerrain->getGridSize()-1);
		QString gYString=QString("GridY#integer|0|%1").arg(IIRoot::getSingletonPtr()->mTerrain->getGridSize()-1);
		map[gXString]=QString().setNum(mSelectObject->GridX);
		map[gYString]=QString().setNum(mSelectObject->GridY);

		map["Roll#"]=QString().setNum(q.getRoll().valueDegrees());
		map["Pitch#"]=QString().setNum(q.getPitch().valueDegrees());
		map["Yaw#"]=QString().setNum(q.getYaw().valueDegrees());
		map["ScaleX#float"]=QString().setNum(s.x);
		map["ScaleY#float"]=QString().setNum(s.y);
		map["ScaleZ#float"]=QString().setNum(s.z);

		if (mSelectObject->map.contains("ObjectDirection"))
		{
			map["ObjectDirection#enum|North|South|West|East"]=mSelectObject->map.value("ObjectDirection");
		}

		if (mSelectObject->map.contains("direction"))
		{
			map["Direction#string"] = mSelectObject->map.value("direction");
		}
		if (mSelectObject->map.contains("numunit"))
		{
			map["NumUnit#string"] = mSelectObject->map.value("numunit");
		}
		if (mSelectObject->map.contains("morale"))
		{
			map["Morale#string"] = mSelectObject->map.value("morale");
		}

		mAttribute->setAttribute(map);
	}
	else
	{
		mAttribute->clearAttribute();
	}
}

void ObjcetControl::attributeChangle( QString name,QString value )
{
	if (mSelectObject!=NULL)
	{
		Ogre::Vector3 p=mSelectObject->mNode->getPosition();
		Ogre::Vector3 s=mSelectObject->mNode->getScale();
		Ogre::Quaternion q=mSelectObject->mNode->getOrientation();
		if (name=="X")
		{






			p.x=value.toFloat();
			mSelectObject->mNode->setPosition(p);

		}
		else if (name=="Y")
		{




			p.y=value.toFloat();
			mSelectObject->mNode->setPosition(p);

		}
		else if (name=="Z")
		{




			p.z=value.toFloat();
			mSelectObject->mNode->setPosition(p);

		}
		else if (name=="OffsetX")
		{
			Ogre::Vector2 gridPosition=IIRoot::getSingletonPtr()->mTerrain->getRealXY(mSelectObject->GridX,mSelectObject->GridY,SecGrid); 

			p.x=gridPosition.x+value.toFloat();
			mSelectObject->mNode->setPosition(p);
		}
		else if (name=="OffsetY")
		{
			float height=IIRoot::getSingletonPtr()->mTerrain->getHeight(mSelectObject->GridX,mSelectObject->GridY);

			p.y=height+value.toFloat();
			mSelectObject->mNode->setPosition(p);
		}
		else if (name=="OffsetZ")
		{
			Ogre::Vector2 gridPosition=IIRoot::getSingletonPtr()->mTerrain->getRealXY(mSelectObject->GridX,mSelectObject->GridY,SecGrid); 

			p.z=gridPosition.y+value.toFloat();
			mSelectObject->mNode->setPosition(p);
		}
		else if (name=="GridX")
		{
			Ogre::Vector2 v1=IIRoot::getSingletonPtr()->mTerrain->getRealXY(value.toInt(),mSelectObject->GridY,SecGrid); 
			float height=IIRoot::getSingletonPtr()->mTerrain->getHeight(value.toInt(),mSelectObject->GridY);
			mSelectObject->mNode->setPosition(Ogre::Vector3(v1.x,height,v1.y));
		}
		else if (name=="GridY")
		{
			Ogre::Vector2 v1=IIRoot::getSingletonPtr()->mTerrain->getRealXY(mSelectObject->GridX,value.toInt(),SecGrid); 
			float height=IIRoot::getSingletonPtr()->mTerrain->getHeight(mSelectObject->GridX,value.toInt());
			mSelectObject->mNode->setPosition(Ogre::Vector3(v1.x,height,v1.y));
		}
		//else if (name=="Roll")
		//{
		//	q.FromAngleAxis(Ogre::Radian(value.toFloat()))  ;
		//	mSelectObject->mNode->setPosition(p);
		//}
		//else if (name=="Pitch")
		//{
		//	p.z=value.toFloat();
		//	mSelectObject->mNode->setPosition(p);
		//}
		//else if (name=="Yaw")
		//{
		//	p.z=value.toFloat();
		//	mSelectObject->mNode->setPosition(p);
		//}
		else if (name=="ScaleX")
		{
			s.x=value.toFloat();
			mSelectObject->mNode->setScale(s);
		}
		else if (name=="ScaleY")
		{
			s.y=value.toFloat();
			mSelectObject->mNode->setScale(s);
		}
		else if (name=="ScaleZ")
		{
			s.z=value.toFloat();
			mSelectObject->mNode->setScale(s);
		}
		else if (name == "Type")
		{
			mSelectObject->EntityType = value;
		}
		else if (name == "Direction")
		{
			mSelectObject->map["direction"] = value;
		}
		else if (name == "NumUnit")
		{
			mSelectObject->map["numunit"] = value;
		}
		else if (name == "Morale")
		{
			mSelectObject->map["morale"] = value;
		}
		else if (name == "ObjectDirection")
		{
			mSelectObject->map["ObjectDirection"] = value;
			Ogre::Quaternion q;

			if(value=="North")
			{
				q.FromAngleAxis(Ogre::Degree(180),Ogre::Vector3(0,1,0));
			}
			else if(value=="South")
			{
				q.FromAngleAxis(Ogre::Degree(360),Ogre::Vector3(0,1,0));
			}
			else if(value=="West")
			{
				q.FromAngleAxis(Ogre::Degree(270),Ogre::Vector3(0,1,0));
			}
			else if(value=="East")
			{
				q.FromAngleAxis(Ogre::Degree(90),Ogre::Vector3(0,1,0));
			}

			mSelectObject->mNode->setOrientation(q);
		}
	}
}

