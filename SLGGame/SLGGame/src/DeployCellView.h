/*!
@file
@author		Albert Semenov
@date		07/2008
*/
#ifndef __DeployCELL_VIEW_H__
#define __DeployCELL_VIEW_H__

#include <MyGUI.h>
#include "ItemBox/BaseCellView.h"
#include "DeployItemData.h"

class DeployCellView :
	public wraps::BaseCellView<DeployItemData*>
{
public:
	DeployCellView(MyGUI::Widget* _parent);

	void update(const MyGUI::IBDrawItemInfo& _info, DeployItemData* _data);
	static void getCellDimension(MyGUI::Widget* _sender, MyGUI::IntCoord& _coord, bool _drop);

private:
	MyGUI::ImageBox* mImageBack;
	MyGUI::ImageBox* mImageBorder;
	MyGUI::ImageBox* mImageItem;
	MyGUI::TextBox* mTextBack;
	MyGUI::TextBox* mTextFront;
};

#endif // __CELL_VIEW_H__
