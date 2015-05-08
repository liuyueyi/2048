#pragma once
#include "cocos2d.h"

class GameScene : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();
	static GameScene* getInstance();
	virtual bool init();

private:
	CREATE_FUNC(GameScene)
};

