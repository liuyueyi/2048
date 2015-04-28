#include "SetMenu.h"
#include "Grid.h"
#include "GameTool.h"
#include "GameLayer.h"
#include "DataConf.h"

bool SetMenu::init()
{
	if(!Layer::init())
		return false;

	this->setContentSize(Size(120, 180));
	this->setPosition(Vec2(195, 185));

	auto bg = LayerColor::create(Color4B(30, 30, 30, 200), 120, 180);
	this->addChild(bg);

	auto draw = DrawNode::create();
	this->addChild(draw);
	//draw->drawLine(Vec2(10, 20), Vec2(50, 20), Color4F(0, 0, 0, 1));

	auto classic = Label::createWithSystemFont(Grid::G2U("经典模式"), "Arial", 25);
	auto soldier = Label::createWithSystemFont(Grid::G2U("小兵模式"), "Arial", 25);
	auto color = Label::createWithSystemFont(Grid::G2U("纯色模式"), "Arial", 25);
	auto sound = Label::createWithSystemFont(Grid::G2U("声音"), "Arial", 25);

	auto item01 = MenuItemLabel::create(classic, CC_CALLBACK_1(SetMenu::classicCallFunc, this));
	item01->setPosition(60, 135 + 22.5f);
	draw->drawLine(Vec2(5, 135), Vec2(115, 135), Color4F(1,1,1,1));
	auto item02 = MenuItemLabel::create(soldier, CC_CALLBACK_1(SetMenu::soldierCallFunc, this));
	item02->setPosition(60, 90 + 22.5f);
	draw->drawLine(Vec2(5, 90), Vec2(115, 90), Color4F(1, 1, 1,1));
	auto item03 = MenuItemLabel::create(color, CC_CALLBACK_1(SetMenu::colorCallFunc, this));
	item03->setPosition(60, 45 + 22.5f);
	draw->drawLine(Vec2(5, 45), Vec2(115, 45), Color4F(1, 1, 1,1));
	auto item04 = MenuItemLabel::create(sound, CC_CALLBACK_1(SetMenu::soundCallFunc, this));
	item04->setPosition(60, 22.5f);
	//draw->drawLine(Vec2(5, 22.5f), Vec2(115, 157.5f), Color4F(0,0,0,1));

	auto menu = Menu::create(item01, item02, item03, item04, nullptr);
	//menu->alignItemsVertically();
	menu->setPosition(0, 0);
	this->addChild(menu);
	return true;
}

void SetMenu::classicCallFunc(Ref* ref)
{
	changeType(1);
}

void SetMenu::soldierCallFunc(Ref* ref)
{
	changeType(0);
}

void SetMenu::colorCallFunc(Ref* ref)
{
	changeType(2);
}

void SetMenu::soundCallFunc(Ref* ref)
{

}

void SetMenu::changeType(int newType)
{
	auto type = Grid::getType();
	if(type == newType) // do not change the game model
		return;
	// do change, then save the game stat and restart new game model
	DataConf::getInstance()->dumpData(type); 
	GameLayer::getInstance()->clearGrids();
	Grid::changeType(newType);
	this->setVisible(false);
}


