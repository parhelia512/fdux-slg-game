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

	void setIcon(int type);

	void setBar(float v,float v1);

	void setVisible(bool v);

	void changeColor(MyGUI::Colour c);

	void setNode(Ogre::SceneNode* node){mNode=node;}

	MyGUI::Colour mHasApColor;
	MyGUI::Colour mNoneApColor;

private:
	Ogre::SceneNode* mNode;

	MyGUI::TextBox* mBName;
	MyGUI::ImageBox* mBImage;
	MyGUI::ImageBox* mBIcon;
	MyGUI::ImageBox* mP1;
	MyGUI::ImageBox* mP2;
	
	bool mIsVisible;
};
