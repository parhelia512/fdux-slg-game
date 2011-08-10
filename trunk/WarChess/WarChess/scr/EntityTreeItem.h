#pragma once

#include <QList>
#include <QString>
#include <QStringList>
#include <QVector>

#include "EntityData.h"

enum EntityItemType
{
	Entity,
	Group
};

class EntityTreeItem
{
public:
	EntityTreeItem(EntityData* data,EntityTreeItem* parent,EntityItemType type);
	~EntityTreeItem(void);

	void appendChild(EntityTreeItem* item);
	void removeChild(EntityTreeItem* item);

	EntityTreeItem* child(int row);
    int childCount() const;
    int columnCount() const;
    EntityData* data(int column);
    int row() const;
    EntityTreeItem* parent();
	void setParent(EntityTreeItem* newParent);

	EntityTreeItem* findItem(QString name);
	void findItemAsSimpleName(QString name,QVector<EntityTreeItem*>* result);//������������"_"�ַ�����,��ƥ��"_"��ǰ���ַ���

	EntityItemType entityItemType;

private:
	EntityTreeItem* parentItem;
	QList<EntityTreeItem*> childItems;
	EntityData* entityData;
};
