#pragma once
#include "cocos2d.h"

USING_NS_CC;

class MenuButton: public Node
{
public:
	CREATE_FUNC(MenuButton);
	virtual bool init();

	MenuItem* getMenuItem(const std::string& text, const Size& size);
};