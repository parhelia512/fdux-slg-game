#include "EntityTreeItem.h"

EntityTreeItem::EntityTreeItem(EntityData* data,EntityTreeItem* parent,EntityItemType type)
{
	parentItem=parent;
	entityData=data;
	entityItemType=type;
}

EntityTreeItem::~EntityTreeItem(void)
{
	delete entityData;
	qDeleteAll(childItems);
}

void EntityTreeItem::appendChild(EntityTreeItem* item)//����ӽڵ�
{
	childItems.append(item);
}

void EntityTreeItem::removeChild(EntityTreeItem* item)
{
	childItems.removeOne(item);
}

EntityTreeItem* EntityTreeItem::child(int row)//��ȡ�ӽڵ�
{
	if(childItems.size()!=0)
	{
		return childItems.at(row);
	}
	else
	{
		return NULL;
	}
}

int EntityTreeItem::childCount() const//�ӽڵ�����
{
	return childItems.size();
}

int EntityTreeItem::columnCount() const//�е�����
{
	return 1;
}

EntityData* EntityTreeItem::data(int column)//ȡ������
{
	return entityData;
}

int EntityTreeItem::row() const//��ȡ�ýڵ��ڵڼ���
{
	if (parentItem)
	{
         return parentItem->childItems.indexOf(const_cast<EntityTreeItem*>(this));
	}
	else
	{
		return 0;
	}
}
    
EntityTreeItem* EntityTreeItem::parent()//��ȡ���ڵ�
{
	return parentItem;
}

void EntityTreeItem::setParent(EntityTreeItem* newParent)
{
	parentItem=newParent;
}

EntityTreeItem* EntityTreeItem::findItem(QString name)//�ݹ��������
{
	QList<EntityTreeItem*>::Iterator it;
	for(it=childItems.begin();it!=childItems.end();it++)
	{
		if((*it)->entityData->name==name)
		{
			return (*it);
		}

		if((*it)->entityItemType==Group)
		{
			EntityTreeItem* resultItem=(*it)->findItem(name);
			if(resultItem!=NULL)
			{
				return resultItem;
			}
		}
	}
	return NULL;
}

void EntityTreeItem::findItemAsSimpleName( QString name,QVector<EntityTreeItem*>* result )
{
	QList<EntityTreeItem*>::Iterator it;
	for(it=childItems.begin();it!=childItems.end();it++)
	{
		if((*it)->entityItemType==Group)
		{
			(*it)->findItemAsSimpleName(name,result);
		}
		else if ((*it)->entityItemType==Entity)
		{
			if((*it)->entityData->name.indexOf("_")!=-1)
			{
				QStringList sl=(*it)->entityData->name.split("_");
				if (sl.at(0)==name)
				{
					result->append((*it));
				}
			}
		}
	}
}