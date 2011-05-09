#include "scripteditor.h"

#include "GUISystem.h"
#include "GUIStage.h"

#include <QColorDialog>
#include <QMessageBox>
#include <QSettings>

ScriptEditor::ScriptEditor(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);
	this->connect(ui.pushButton,SIGNAL(clicked(bool)),this,SLOT(showImageButton()));
	this->connect(ui.SetTextColorButton,SIGNAL(clicked(bool)),this,SLOT(SetTextColorButton()));
	this->connect(ui.SetRoleNameColorButton,SIGNAL(clicked(bool)),this,SLOT(SetRoleNameColorButton()));
	this->connect(ui.SetDialogButton,SIGNAL(clicked(bool)),this,SLOT(SetDialogButton()));
	this->connect(ui.ShowTextButton,SIGNAL(clicked(bool)),this,SLOT(ShowTextButton()));
	this->connect(ui.ShowRoleNameButton,SIGNAL(clicked(bool)),this,SLOT(ShowRoleNameButton()));
	this->connect(ui.ClearTextButton,SIGNAL(clicked(bool)),this,SLOT(ClearTextButton()));
	this->connect(ui.RunButton,SIGNAL(clicked(bool)),this,SLOT(RunLuaScript()));
	this->connect(ui.ParseButton,SIGNAL(clicked(bool)),this,SLOT(ParseScript()));

	QSettings settings("config.ini", QSettings::IniFormat);
	ui.DialogXEdit->setText(settings.value("DialogLeft").toString());
	ui.DialogYEdit->setText(settings.value("DialogTop").toString());
	ui.DialogWidthEdit->setText(settings.value("DialogWidth").toString());
	ui.DialogHeightEdit->setText(settings.value("DialogHeight").toString());
	ui.MarginLeftEdit->setText(settings.value("TextLeft").toString());
	ui.MarginTopEdit->setText(settings.value("TextTop").toString());
	ui.TextWidthEdit->setText(settings.value("TextWidth").toString());
	ui.TextHeightEdit->setText(settings.value("TextHeight").toString());
	ui.FontEdit->setText(settings.value("TextFont").toString());

	ui.RoleNameXEdit->setText(settings.value("RoleNameLeft").toString());
	ui.RoleNameYEdit->setText(settings.value("RoleNameTop").toString());
	ui.RoleNameWidthEdit->setText(settings.value("RoleNameWidth").toString());
	ui.RoleNameHeightEdit->setText(settings.value("RoleNameHeight").toString());
	ui.RoleNameFontEdit->setText(settings.value("RoleFont").toString());

	mDialogAttribute.TextRed=settings.value("TextRed").toFloat();
	mDialogAttribute.TextGreen=settings.value("TextGreen").toFloat();
	mDialogAttribute.TextBlue=settings.value("TextBlue").toFloat();

	mDialogAttribute.RoleNameRed=settings.value("RoleNameRed").toFloat();
	mDialogAttribute.RoleNameGreen=settings.value("RoleNameGreen").toFloat();
	mDialogAttribute.RoleNameBlue=settings.value("RoleNameBlue").toFloat();
}

ScriptEditor::~ScriptEditor()
{
	QSettings settings("config.ini", QSettings::IniFormat);
	settings.setValue("DialogLeft",ui.DialogXEdit->text());
	settings.setValue("DialogTop",ui.DialogYEdit->text());
	settings.setValue("DialogWidth",ui.DialogWidthEdit->text());
	settings.setValue("DialogHeight",ui.DialogHeightEdit->text());
	settings.setValue("TextLeft",ui.MarginLeftEdit->text());
	settings.setValue("TextTop",ui.MarginTopEdit->text());
	settings.setValue("TextWidth",ui.TextWidthEdit->text());
	settings.setValue("TextHeight",ui.TextHeightEdit->text());
	settings.setValue("TextFont",ui.FontEdit->text());

	settings.setValue("RoleNameLeft",ui.RoleNameXEdit->text());
	settings.setValue("RoleNameTop",ui.RoleNameYEdit->text());
	settings.setValue("RoleNameWidth",ui.RoleNameWidthEdit->text());
	settings.setValue("RoleNameHeight",ui.RoleNameHeightEdit->text());
	settings.setValue("RoleFont",ui.RoleNameFontEdit->text());

	settings.setValue("TextRed",mDialogAttribute.TextRed);
	settings.setValue("TextGreen",mDialogAttribute.TextGreen);
	settings.setValue("TextBlue",mDialogAttribute.TextBlue);

	settings.setValue("RoleNameRed",mDialogAttribute.RoleNameRed);
	settings.setValue("RoleNameGreen",mDialogAttribute.RoleNameGreen);
	settings.setValue("RoleNameBlue",mDialogAttribute.RoleNameBlue);
}


void ScriptEditor::showImageButton()
{
	GUIImageLayer layer;
	
	if (ui.EffectsLayerButton->isChecked())
	{
		layer=EffectsLayer;
	}
	else if (ui.LeftLayerButton->isChecked())
	{
		layer=LeftLayer;
	}
	else if (ui.MidLayerButton->isChecked())
	{
		layer=MidLayer;
	}
	else if (ui.RightLayerButton->isChecked())
	{
		layer=RightLayer;
	}
	else if (ui.BGLayerButton->isChecked())
	{
		layer=BackGroundLayer;
	}

	static_cast<GUIStage*>(OgreCore::getSingletonPtr()->mGUISystem->getScene(StageScene))->showImage(ui.ImageFileNameEdit->text().toStdString(),layer,ui.universalTimeEdit->text().toFloat(),ui.XEdit->text().toInt(),ui.YEdit->text().toInt());
}


void ScriptEditor::SetTextColorButton()
{
	QColor c=QColorDialog::getColor();
	mDialogAttribute.TextRed=c.redF();
	mDialogAttribute.TextGreen=c.greenF();
	mDialogAttribute.TextBlue=c.blueF();
}


void ScriptEditor::SetRoleNameColorButton()
{
	QColor c=QColorDialog::getColor();
	mDialogAttribute.RoleNameRed=c.redF();
	mDialogAttribute.RoleNameGreen=c.greenF();
	mDialogAttribute.RoleNameBlue=c.blueF();
}


void ScriptEditor::SetDialogButton()
{
	mDialogAttribute.TextLeft=ui.MarginLeftEdit->text().toInt();
	mDialogAttribute.TextTop=ui.MarginTopEdit->text().toInt();
	mDialogAttribute.TextWidth=ui.TextWidthEdit->text().toInt();
	mDialogAttribute.TextHeight=ui.TextHeightEdit->text().toInt();
	mDialogAttribute.TextFont=ui.FontEdit->text().toStdString();

	mDialogAttribute.RoleNameLeft=ui.RoleNameXEdit->text().toInt();
	mDialogAttribute.RoleNameTop=ui.RoleNameYEdit->text().toInt();
	mDialogAttribute.RoleNameWidth=ui.RoleNameWidthEdit->text().toInt();
	mDialogAttribute.RoleNameHeight=ui.RoleNameHeightEdit->text().toInt();
	mDialogAttribute.RoleFont=ui.RoleNameFontEdit->text().toStdString();

	//static_cast<GUIStage*>(OgreCore::getSingletonPtr()->mGUISystem->getScene(StageScene))->setTextDialog(mDialogAttribute);
	//static_cast<GUIStage*>(OgreCore::getSingletonPtr()->mGUISystem->getScene(StageScene))->setTextDialogVisible(true);
}


void ScriptEditor::ClearTextButton()
{
	//static_cast<GUIStage*>(OgreCore::getSingletonPtr()->mGUISystem->getScene(StageScene))->clearText();
}

void ScriptEditor::ShowTextButton()
{
	//std::string a=ui.TextEdit->toPlainText().toStdString();
	static_cast<GUIStage*>(OgreCore::getSingletonPtr()->mGUISystem->getScene(StageScene))->showScene("");
}

void ScriptEditor::ShowRoleNameButton()
{
	//static_cast<UIStage*>(OgreCore::getSingletonPtr()->mUIControl->getScene(StageScene))->showRoleName(ui.RoleNameEdit->text().toStdWString());
}

void ScriptEditor::timerEvent(QTimerEvent * event)
{
	OgreCore::getSingletonPtr()->RenderingFrame();
}

bool ScriptEditor::windowClosing( Ogre::RenderWindow *rw )
{
	if(QMessageBox::question(this,"�˳�?","�Ƿ�Ҫ�˳�����?",QMessageBox::Yes|QMessageBox::No )==QMessageBox::Yes)
	{
		close();
		return true;
	}
	else
	{
		return false;
	}
}

void ScriptEditor::RunLuaScript()
{
	OgreCore::getSingletonPtr()->mLuaSystem->runScript(ui.luaEdit->toPlainText().toStdString());
}

void ScriptEditor::onError( std::string error )
{
	ui.retEdit->append(error.c_str());
}

void ScriptEditor::onComplete()
{
	ui.retEdit->append("Luaִ�����");
}

void ScriptEditor::ParseScript()
{
	QString lua=mScriptParse.parse(ui.scrEdit->toPlainText());
	if (mScriptParse.hasError())
	{
		ui.retEdit->append(lua);
	}
	else
	{
		ui.luaEdit->append(lua);
	}
}