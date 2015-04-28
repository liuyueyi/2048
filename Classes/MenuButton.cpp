#include "MenuButton.h"

bool MenuButton::init()
{
	if(!Node::init())
		return false;
	else
		return true;
}

MenuItem* MenuButton::getMenuItem(const std::string& text, const Size& size)
{
	auto label = Label::createWithSystemFont(text, "Airea", 26);
	//label->setTextColor(Color4B(120,120,0, 255));

	auto item = MenuItemLabel::create(label);
	item->setContentSize(size);

	return item;
}