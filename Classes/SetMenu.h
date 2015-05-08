#pragma once

#include "cocos2d.h"

USING_NS_CC;

class SetMenu:public Layer
{
public:
	static SetMenu* getInstance();
	CREATE_FUNC(SetMenu);
	virtual bool init();

private:
	void classicCallFunc(Ref* ref);
	void colorCallFunc(Ref* ref);
	void soldierCallFunc(Ref* ref);
	void soundCallFunc(Ref* ref);
	void changeType(int newType);

	void loadTitle();

	Label* _modes[4];
};