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

	/*auto cl = LayerColor::create(Color4B(120, 120, 120, 255), 100, 100);
	cl->setPosition(100, 100);
	auto a1 = FadeOut::create(1);
	auto a2 = FadeIn::create(1);
	auto a3 = RepeatForever::create(Sequence::create(a1, a2, nullptr));
	cl->runAction(a3);
	this->addChild(cl);
	
	auto label = Label::createWithSystemFont("click me", "Arial", 30);
	label->setPosition(160, 200);
	this->addChild(label);
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [label](Touch* t, Event* s){
		auto rec = label->getBoundingBox();
		log("clicked");
		if(label->getBoundingBox().containsPoint(t->getLocation())){
			log("rec is: %f %f %f %f\n", rec.getMaxX(), rec.getMaxY(), rec.getMinX(), rec.getMinY());
			return true;
		}
		return false;
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, label);*/
	return true;
}