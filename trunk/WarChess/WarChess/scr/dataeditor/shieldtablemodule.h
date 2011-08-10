#ifndef SHIELDTABLEMODULE_H
#define SHIELDTABLEMODULE_H

#include <QAbstractTableModel>

class ShieldTableModule : public QAbstractTableModel
{
	Q_OBJECT

public:
	ShieldTableModule(QObject *parent = 0);
	~ShieldTableModule();

	virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
	virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
	virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
	virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
	virtual Qt::ItemFlags flags(const QModelIndex &index) const;
	virtual bool setData(const QModelIndex &index, const QVariant &qvalue,int role = Qt::EditRole);

	bool insertRow(int row = 0, const QModelIndex & parent = QModelIndex());
	bool removeRow (int row, const QModelIndex & parent = QModelIndex());

	void ModChanged() {reset();}
private:

};

#endif // SHIELDTABLEMODULE_H
