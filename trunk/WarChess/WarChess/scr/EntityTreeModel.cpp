#include "EntityTreeModel.h"

#include <QIcon>

EntityTreeModel::EntityTreeModel(QObject *parent):QAbstractItemModel(parent),mSelectItem(QModelIndex())
{
	rootItem=new EntityTreeItem(new EntityData("Root"),NULL,EntityItemType::Group);
}

EntityTreeModel::~EntityTreeModel(void)
{
	delete rootItem;
}

QVariant EntityTreeModel::data(const QModelIndex &index, int role) const//ȡ��QModelIndexָ�������
{
     if (!index.isValid())
	 {
         return QVariant();
	 }
	 else
	 {
		 EntityTreeItem* item = static_cast<EntityTreeItem*>(index.internalPointer());
		 if (role == Qt::DisplayRole)//���Ҫ��ȡ��Item�ı���
		 {
			 return item->data(index.column())->name;
		 }
		 else if(role==Qt::DecorationRole)//���Ҫȥȡ��Item��ͼ��
		 {
			 if(item->entityItemType==EntityItemType::Group)
			 {
				 return QIcon(":/WarChess/Resources/fileopen.png");
			 }
			 else
			 {
				 if (mSelectItem.isValid() && mSelectItem==index)
				 {
					return QIcon(":/WarChess/Resources/ok.png");
				 }
				 else
				 {
					return QIcon(":/WarChess/Resources/drag.png");
				 }
			 }
		 }
		 else
		 {
			 return QVariant();
		 }
	 }
}

Qt::ItemFlags EntityTreeModel::flags(const QModelIndex &index) const//ȡ��QModelIndex��ָ���������,ָ�Ƿ���Ա༭֮���
{
	if (!index.isValid())
	{
        return 0;
	}
	else
	{
		EntityTreeItem* item=static_cast<EntityTreeItem*>(index.internalPointer());
		if(item->parent()==NULL)
		{
			return (Qt::ItemFlag::ItemIsEnabled | Qt::ItemFlag::ItemIsSelectable);
		}
		else
		{
			return (Qt::ItemFlag::ItemIsEnabled |Qt::ItemFlag::ItemIsEditable| Qt::ItemFlag::ItemIsSelectable | Qt::ItemFlag::ItemIsDropEnabled | Qt::ItemFlag::ItemIsDragEnabled);
		}

	}
}

QVariant EntityTreeModel::headerData(int section, Qt::Orientation orientation,int role) const//���ر�ͷ��Ϣ
{
	 if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
	 {
         return "ʵ���б�";
	 }
	 else
	 {
		 return QVariant();
	 }
}

QModelIndex EntityTreeModel::index(int row, int column,const QModelIndex &parent) const
//���������View��˵������Ҫ�����������Ҫ���������ϵ�index.�������item��û��parent��view�������һ��parent,��Ȼ�����û��parent,�����parent��index������Ч�ġ�
{
	EntityTreeItem* parentItem;

	if(!parent.isValid())
	{
		parentItem=rootItem;
	}
	else
	{
		parentItem=static_cast<EntityTreeItem*>(parent.internalPointer());
	}

	EntityTreeItem *childItem = parentItem->child(row);

	if(childItem)
	{
		return createIndex(row,column,childItem);
	}
	else
	{
		return QModelIndex();
	}
}

QModelIndex EntityTreeModel::parent(const QModelIndex &index) const
//�����������view����Ҫһ��ָ��index��parent��index
{
	if(index.isValid())
	{
		EntityTreeItem* childItem=static_cast<EntityTreeItem*>(index.internalPointer());
		EntityTreeItem* parentItem=childItem->parent();
		
		if (parentItem==rootItem)
		{
			return QModelIndex();
		}
		else if(parentItem==NULL)
		{
			return QModelIndex();
		}
		else
		{
			return createIndex(parentItem->row(),0,parentItem);
		}
	} 
	else
	{
		return QModelIndex();
	}
}

int EntityTreeModel::rowCount(const QModelIndex &parent) const//����ĳ�����ڵ��µ��ӽڵ�ĸ���
{
	EntityTreeItem *parentItem;
	if(parent.isValid())
	{
		parentItem=static_cast<EntityTreeItem*>(parent.internalPointer());
	}
	else
	{
		parentItem=rootItem;
	}

	return parentItem->childCount();
}

int EntityTreeModel::columnCount(const QModelIndex &parent) const//һ���ڵ��м�������
{
	 if (parent.isValid())
	 {
         return static_cast<EntityTreeItem*>(parent.internalPointer())->columnCount();
	 }
     else
	 {
         return rootItem->columnCount();
	 }
}

EntityTreeItem* EntityTreeModel::getRootItem()
{
	return rootItem;
}

void EntityTreeModel::setSelectItem( const QModelIndex &item )
{	
	QModelIndex temp=mSelectItem;

	mSelectItem=item;

	if (temp.isValid())
	{
		emit dataChanged(temp,temp);//Ҫ��view�ػ���ǰ����,��֤ͼ�걻����
	}
}

