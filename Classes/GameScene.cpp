#include "GameScene.h"
#include "GameLayer.h"
#include "GameMenuLayer.h"
#include "GameTool.h"
#include "SetMenu.h"

USING_NS_CC;

Scene* GameScene::createScene()
{
	auto scene = Scene::create();
	auto layer = GameScene::getInstance();
	scene->addChild(layer);
	return scene;
}

static GameScene* _gameScene;
GameScene* GameScene::getInstance()
{
	if(!_gameScene)
		_gameScene = GameScene::create();
	return _gameScene;
}

bool GameScene::init()
{
	if(!Layer::init())
		return false;

	this->addChild(LayerColor::create(Color4B(242, 242, 242, 255))); // add background

	auto tool = GameTool::getInstance();
	this->addChild(tool);

	auto menuBtnLayer = GameMenuLayer::getInstance();
	this->addChild(menuBtnLayer);

	auto gameLayer = GameLayer::getInstance();
	this->addChild(gameLayer);

	auto setLayer = SetMenu::create();
	setLayer->setName("setlayer");
	setLayer->setVisible(false);
	this->addChild(setLayer);

	return true;
}