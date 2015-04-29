#pragma once
#include "cocos2d.h"

class GameMenuLayer :
	public cocos2d::Layer
{
public:
	virtual bool init();
	static GameMenuLayer* getInstance();
private:
	CREATE_FUNC(GameMenuLayer);
	void resetGameFun(cocos2d::Ref* ref);
	void undoGameFun(cocos2d::Ref* ref);
	void setGameFun(cocos2d::Ref* ref);

	static GameMenuLayer* _instance;
};