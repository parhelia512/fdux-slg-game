#pragma once
#include "guibillboard.h"

#include "squaddefine.h"

class GUISquadBillBoard :
	public GUIBillBoard
{
public:
	GUISquadBillBoard(Ogre::SceneNode* node);
	~GUISquadBillBoard(void);

	Ogre::Vector3 getPosition();

	void update(Ogre::Vector2 screen,unsigned int deltaTime);

	void setMarginalValue(float marginalTop,float marginalBottom,float marginalLeft,float marginalRight);

	void showScene(std::string arg);//��ʾ����

	void hideScene();//���س���

	void setName(std::string name,MyGUI::Colour c);

	void setIcon(SquadType type);

	void setVisible(bool v);

private:
	Ogre::SceneNode* mNode;

	MyGUI::TextBox* mBName;
	MyGUI::ImageBox* mBImage;
	MyGUI::ImageBox* mBIcon;
	
	bool mIsVisible;
};
