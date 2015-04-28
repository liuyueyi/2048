#include "GameMenuLayer.h"
#include "MenuButton.h"
#include "GameLayer.h"
#include "GameScene.h"
#include "SetMenu.h"

USING_NS_CC;

static GameMenuLayer* _instance;
GameMenuLayer* GameMenuLayer::getInstance()
{
	if(_instance == nullptr)
		_instance = create();
	return _instance;
}

bool GameMenuLayer::init()
{
	if(!Layer::init())
		return false;

	this->setContentSize(Size(300, 30));
	this->setPosition(Vec2(10, 370));

	auto menuButton = MenuButton::create();
	auto resetBg = LayerColor::create(Color4B(143, 122, 101, 255), 100, 30);
	this->addChild(resetBg);
	auto resetmenu = menuButton->getMenuItem("Restart", Size(100, 30));
	resetmenu->setPosition(55, 17);
	resetmenu->setCallback(CC_CALLBACK_1(GameMenuLayer::resetGameFun, this));
	
	auto setBg = LayerColor::create(Color4B(143, 122, 101, 255), 50, 30);
	setBg->setPosition(250, 0);
	this->addChild(setBg);
	auto setmenu = menuButton->getMenuItem("Set", Size(40, 30));
	setmenu->setPosition(275, 17);
	setmenu->setCallback(CC_CALLBACK_1(GameMenuLayer::setGameFun, this));
	
	auto undoBg = LayerColor::create(Color4B(143, 122, 101, 255), 60, 30);
	undoBg->setPosition(180, 0);
	this->addChild(undoBg);
	auto undomenu = menuButton->getMenuItem("Undo", Size(60, 30));
	undomenu->setPosition(213, 17);
	undomenu->setCallback(CC_CALLBACK_1(GameMenuLayer::undoGameFun, this));

	auto menu = Menu::create(resetmenu, undomenu, setmenu, NULL);
	//menu->alignItemsHorizontally();
	menu->setPosition(0,0);

	this->addChild(menu);
	return true;
}

void GameMenuLayer::resetGameFun(Ref* ref)
{
	GameLayer::getInstance()->restartGame();
	log("reset game");
}

void GameMenuLayer::setGameFun(Ref* ref)
{
	auto layer = static_cast<GameScene*>(this->getParent());
	auto setmenu = layer->getChildByName("setlayer");
	if(setmenu->isVisible())
		setmenu->setVisible(false);
	else
		setmenu->setVisible(true);
	log("start game");
}

void GameMenuLayer::undoGameFun(Ref* ref)
{
	GameLayer::getInstance()->undoGame();
	log("back to last stat");
}