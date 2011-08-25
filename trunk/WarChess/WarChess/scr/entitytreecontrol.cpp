#include "entitytreecontrol.h"

#include <QFile>
#include <QTextStream>
#include <QMessageBox>

#include "entityitemdelegate.h"
#include "entitydialog.h"
#include "IIRoot.h"

EntityTreeControl::EntityTreeControl(QTreeView* treeView,ObjcetControl* objectControl):mTreeView(treeView),mSelectEntity(NULL),mTerrain(IIRoot::getSingletonPtr()->mTerrain),mObjectControl(objectControl)
{
	//����ģ����,��ģ�����õ���ͼ��,���������Լ��ı༭�����������ͼ��
	mEntityTreeModel=new EntityTreeModel();
	mTreeView->setModel(mEntityTreeModel);
	EntityItemDelegate* entitiyItemDelegate=new EntityItemDelegate(NULL);
	entitiyItemDelegate->entityTreeModel=mEntityTreeModel;
	entitiyItemDelegate->treeView=mTreeView;
	mTreeView->setItemDelegate(entitiyItemDelegate);

	open("Entity.xml");//����Ĭ���б�	
	
	mSceneManager=IIRoot::getSingletonPtr()->mSceneMgr;
	mSelectNode=mSceneManager->getRootSceneNode()->createChildSceneNode();//��������ģ��ʹ�õĽڵ�
	mSelectNode->setPosition(0,-1000,0);
}

EntityTreeControl::~EntityTreeControl()
{
	//�˳�ʱ,�����б�;
	save("Entity.xml");
}

bool EntityTreeControl::addGroup(QString name)
{
	EntityTreeItem* rootItem=mEntityTreeModel->getRootItem();

	if(rootItem->findItem(name)!=NULL)
	{
		return false;
	}

	rootItem->appendChild(new EntityTreeItem(new EntityDataImp(name),rootItem,EntityItemType::Group));

	mTreeView->reset();//�ı����ݺ�,�ǵ�֪ͨviewˢ��

	return true;
}

bool EntityTreeControl::removeGroup(QString name)
{
	EntityTreeItem* rootItem=mEntityTreeModel->getRootItem();

	if(rootItem->findItem(name)!=NULL)
	{
		EntityTreeItem* childItem=rootItem->findItem(name);
		if(childItem->entityItemType==EntityItemType::Group)
		{
			mEntityTreeModel->removeRow(childItem->row());
			childItem->parent()->removeChild(childItem);		
			mTreeView->reset();//�ı����ݺ�,�ǵ�֪ͨviewˢ��
			delete childItem;//�м�!����Ҫ��viewˢ�º�,����delete������
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}


}

void EntityTreeControl::addEntity()
{

	QStringList groupList;

	for(int i=0;i<mEntityTreeModel->getRootItem()->childCount();i++)//�������б�
	{
		groupList.append(mEntityTreeModel->getRootItem()->child(i)->data(0)->name);//��root��ȡ������ӽڵ�(������)
	}

	if(groupList.isEmpty())
	{
		QMessageBox::information(NULL,"����","û����,���Ƚ���һ����");
		return ;
	}

	EntityDialog dialog(groupList,mEntityTreeModel);
	dialog.exec();
	int result=dialog.result();

	if(result==QDialog::Accepted)
	{
		EntityTreeItem* groupItem=mEntityTreeModel->getRootItem()->findItem(dialog.entityData->GroupName);//�õ���ڵ�
		groupItem->appendChild(new EntityTreeItem(new EntityDataImp(*dialog.entityData),groupItem,EntityItemType::Entity));
		mTreeView->reset();//�ı����ݺ�,�ǵ�֪ͨviewˢ��
		mTreeView->expand(mEntityTreeModel->index(groupItem->row(),0));//չ����Ӧ����
	}

}

void EntityTreeControl::removeEntity()
{
	QItemSelectionModel* selectionModel =mTreeView->selectionModel();
	QModelIndex selectionIndex=selectionModel->currentIndex();

	if(selectionIndex.isValid())
	{
		EntityTreeItem* childItem=static_cast<EntityTreeItem*>(selectionIndex.internalPointer());
		if(childItem->entityItemType==EntityItemType::Entity)
		{
			QMessageBox::StandardButton button=QMessageBox::information(NULL,"��ʾ","���Ҫɾ����ʵ��?",(QMessageBox::Yes | QMessageBox::No));
			if(button==QMessageBox::Yes)
			{
				if (childItem->data(0)==mEntityItemData)//�����ǰѡ�е���Ҫ��ɾ��,���������
				{
					cancelSelect();
				}

				mEntityTreeModel->removeRow(selectionIndex.row());
				childItem->parent()->removeChild(childItem);		
				mTreeView->reset();//�ı����ݺ�,�ǵ�֪ͨviewˢ��
				delete childItem;//�м�!����Ҫ��viewˢ�º�,����delete������
			}
		}
	}
}

void EntityTreeControl::open(QString filename)
{
	if(filename!="")
	{
		QFile file(filename);
		if (!file.open(QIODevice::ReadOnly))
		{
			QMessageBox::critical(NULL,"����","�޷����ļ�!");
			return ;
		}
		QString XMLStr=QTextStream(&file).readAll();
		file.close();

		QDomDocument doc;
		QString errorStr;
		int errorLine;
		int errorCol;
		if(!doc.setContent(XMLStr,false,&errorStr,&errorLine,&errorCol))
		{
			QMessageBox::critical(NULL,"����",QString("XML����ʧ��!errorLine=%1,errorCol=%2,errorStr=%3").arg(errorLine).arg(errorCol).arg(errorStr));
			return;
		}
		LoadXML(doc);
	}
}

void EntityTreeControl::save(QString filename)
{
	if(filename!="")
	{
		QDomDocument doc=SaveXML();
		QString XMLStr=doc.toString();
		QFile file(filename);
		if (!file.open(QIODevice::WriteOnly))
		{
			QMessageBox::critical(NULL,"����","�޷����ļ�!");
			return ;
		}
		QTextStream stream(&file);
		stream<<"<?xml version=\"1.0\" encoding=\"GB2312\"?>";
		stream<<"\n";
		stream<<XMLStr;
		file.close();
	}
}

void EntityTreeControl::LoadXML(QDomDocument doc)//��xml���뵽����
{
	QList<EntityTreeItem*> childItems;
	EntityTreeItem* rootItem=mEntityTreeModel->getRootItem();
	for(int i=0;i<rootItem->childCount();i++)
	{
		childItems.append(rootItem->child(i));
		rootItem->removeChild(rootItem->child(i));
	}
	mTreeView->reset();
	qDeleteAll(childItems);

	QDomElement root=doc.documentElement();

	QDomElement groupList=root.firstChildElement("GroupList");

	QDomElement entityList=root.firstChildElement("EntityList");

	QDomElement group=groupList.firstChildElement();

	while(!group.isNull())
	{
		rootItem->appendChild(new EntityTreeItem(new EntityData(group.attribute("name")),NULL,EntityItemType::Group));
		group=group.nextSiblingElement();
	}

	QDomElement entity=entityList.firstChildElement();
	while(!entity.isNull())
	{
		EntityTreeItem* groupItem=rootItem->findItem(entity.attribute("group"));
		EntityDataImp* entityData=new EntityDataImp(entity.attribute("name"));
		entityData->GroupName=entity.attribute("group");
		entityData->EntityName=entity.attribute("entity");
		entityData->MatName=entity.attribute("mat");
		entityData->EntityType = entity.attribute("type");
		groupItem->appendChild(new EntityTreeItem(static_cast<EntityData*>(entityData),groupItem,EntityItemType::Entity));
		entity=entity.nextSiblingElement();
	}

}

QDomDocument EntityTreeControl::SaveXML()
{
	QDomDocument doc;

	QDomElement root=doc.createElement("Root");

	QDomElement groupList=doc.createElement("GroupList");

	QDomElement entityList=doc.createElement("EntityList");

	EntityTreeItem* rootItem=mEntityTreeModel->getRootItem();

	doc.appendChild(root);
	root.appendChild(groupList);
	root.appendChild(entityList);

	for(int i=0;i<rootItem->childCount();i++)
	{
		QDomElement group=doc.createElement("Group");
		group.setAttribute("name",rootItem->child(i)->data(0)->name);
		groupList.appendChild(group);

		for(int j=0;j<rootItem->child(i)->childCount();j++)
		{
			QDomElement entity=doc.createElement("Entity");
			EntityDataImp* entityData=static_cast<EntityDataImp*>(rootItem->child(i)->child(j)->data(0));
			entity.setAttribute("name",entityData->name);
			entity.setAttribute("group",entityData->GroupName);
			entity.setAttribute("entity",entityData->EntityName);
			entity.setAttribute("mat",entityData->MatName);
			entity.setAttribute("type",entityData->EntityType);
			entityList.appendChild(entity);
		}
	}

	return doc;
}

void EntityTreeControl::selectEntity( const QModelIndex & index )
{
	if (index.isValid())
	{
		EntityTreeItem* item = static_cast<EntityTreeItem*>(index.internalPointer());

		if (item->entityItemType!=Group)
		{
			EntityDataImp* entityData=static_cast<EntityDataImp*>(item->data(0));
			mEntityItemData=entityData;

			if (Ogre::ResourceGroupManager::getSingletonPtr()->resourceExistsInAnyGroup(entityData->EntityName.toLocal8Bit().data()))
			{		
				mEntityTreeModel->setSelectItem(index);
				if (mSelectEntity!=NULL)
				{
					mSelectNode->detachObject(mSelectEntity);
					mSceneManager->destroyEntity(mSelectEntity);
				}
				mSelectEntity=mSceneManager->createEntity(entityData->EntityName.toLocal8Bit().data());
				mSelectEntity->setMaterialName(entityData->MatName.toLocal8Bit().data());
				mSelectNode->attachObject(mSelectEntity);
				mSelectNode->setPosition(0,-1000,0);//��ʼʱ�ƶ���һ���������ĵط�
			}
			else//���mesh������
			{
				QMessageBox::warning(NULL,"����",QString("%1 ������!").arg(entityData->EntityName));
				mTreeView->openPersistentEditor(index);
			}
		}
	}
	else//��ʾ���ѡ��
	{
		mEntityTreeModel->setSelectItem(index);
	}
	
}

void EntityTreeControl::mouseMove( QMouseEvent * event )
{
	if (mSelectEntity!=NULL && mTerrain->isInit())
	{
		int GX,GY;
		if (mTerrain->coordinateToGrid(event->x(),event->y(),&GX,&GY))
		{
			Ogre::Vector2 v=mTerrain->getRealXY(GX,GY,SecGrid);
			float height=mTerrain->getHeight(GX,GY);
			mSelectNode->setPosition(v.x,height,v.y);
		}
	}
}

void EntityTreeControl::mousePress( QMouseEvent * event )
{
	if (mSelectEntity!=NULL && mTerrain->isInit())
	{
		if (event->button()==Qt::MouseButton::LeftButton)
		{
			//����Ҫ����������
			//1.����һ���ڵ�
			Ogre::SceneNode* node=mSceneManager->getRootSceneNode()->createChildSceneNode();
			node->setPosition(mSelectNode->getPosition());
			//2.��ʵ��Ҳ����һ��
			Ogre::Entity* entity=mSceneManager->createEntity(mEntityItemData->EntityName.toLocal8Bit().data());
			entity->setQueryFlags(DEFAULT_QUERY_MARK);
			entity->setMaterialName(mEntityItemData->MatName.toLocal8Bit().data());
			node->attachObject(entity);
			//3.����Ϣ�ύ��ObjctControl
			QMap<QString, QString> map;
			mObjectControl->addObject(mEntityItemData->name,mEntityItemData->GroupName,mEntityItemData->EntityType,node,entity,map);

		}
		else if (event->button()==Qt::MouseButton::RightButton)
		{
			cancelSelect();//�Ҽ�ȡ��ѡ��
		}

	}
}

void EntityTreeControl::mouseRelease( QMouseEvent * event )
{

}

void EntityTreeControl::cancelSelect()
{
	mSelectNode->detachObject(mSelectEntity);
	mSceneManager->destroyEntity(mSelectEntity);
	mSelectEntity=NULL;
	mEntityItemData=NULL;
	selectEntity(QModelIndex());
}
