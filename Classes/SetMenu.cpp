#include "SetMenu.h"
#include "Grid.h"
#include "GameTool.h"
#include "GameLayer.h"
#include "DataConf.h"
#include "json/document.h"
#include "json/rapidjson.h"

std::string setTitle[5];
Color3B selColor = Color3B(247,213,97);
bool SetMenu::init()
{
	if(!Layer::init())
		return false;
	loadTitle();

	auto designSize = Director::getInstance()->getOpenGLView()->getDesignResolutionSize();
	this->setContentSize(Size(120, 180));
	this->setPosition(Vec2(195, 115 + 80*designSize.height/480.0));

	auto bg = LayerColor::create(Color4B(30, 30, 30, 200), 120, 180);
	this->addChild(bg);

	auto draw = DrawNode::create();
	this->addChild(draw);
	//draw->drawLine(Vec2(10, 20), Vec2(50, 20), Color4F(0, 0, 0, 1));

	_modes[1] = Label::createWithSystemFont(setTitle[2], "Arial", 25);
	_modes[0] = Label::createWithSystemFont(setTitle[4], "Arial", 25);
	_modes[2] = Label::createWithSystemFont(setTitle[3], "Arial", 25);
	_modes[3] = Label::createWithSystemFont(setTitle[Value(GameLayer::getInstance()->getMusic()).asInt()], "Arial", 25);
	_modes[Grid::getType()]->setColor(selColor);

	auto item01 = MenuItemLabel::create(_modes[1], CC_CALLBACK_1(SetMenu::classicCallFunc, this));
	item01->setPosition(60, 135 + 22.5f);
	draw->drawLine(Vec2(5, 135), Vec2(115, 135), Color4F(1,1,1,1));
	auto item02 = MenuItemLabel::create(_modes[0], CC_CALLBACK_1(SetMenu::soldierCallFunc, this));
	item02->setPosition(60, 90 + 22.5f);
	draw->drawLine(Vec2(5, 90), Vec2(115, 90), Color4F(1, 1, 1,1));
	auto item03 = MenuItemLabel::create(_modes[2], CC_CALLBACK_1(SetMenu::colorCallFunc, this));
	item03->setPosition(60, 45 + 22.5f);
	draw->drawLine(Vec2(5, 45), Vec2(115, 45), Color4F(1, 1, 1,1));
	auto item04 = MenuItemLabel::create(_modes[3], CC_CALLBACK_1(SetMenu::soundCallFunc, this));
	item04->setPosition(60, 22.5f);
	//draw->drawLine(Vec2(5, 22.5f), Vec2(115, 157.5f), Color4F(0,0,0,1));

	auto menu = Menu::create(item01, item02, item03, item04, nullptr);
	//menu->alignItemsVertically();
	menu->setPosition(0, 0);
	this->addChild(menu);
	//updateSoundValue(GameLayer::getInstance()->getMusic());
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
	auto isMusic = GameLayer::getInstance()->setMusic();
	UserDefault::getInstance()->setBoolForKey("isMusic", isMusic);
	auto sound = static_cast<MenuItemLabel *>(ref);
	if(sound!=nullptr)
		sound->setString(setTitle[Value(isMusic).asInt()]);
	this->setVisible(false);
}

void SetMenu::changeType(int newType)
{
	auto type = Grid::getType();
	if(type == newType) // do not change the game model
		return;

	_modes[type]->setColor(Color3B(255, 255, 255));
	_modes[newType]->setColor(selColor);
	// do change, then save the game stat and restart new game model
	DataConf::getInstance()->dumpData(type); 
	GameLayer::getInstance()->clearGrids();
	Grid::changeType(newType);
	this->setVisible(false);
}


void SetMenu::loadTitle()
{
	auto path = "data.json";
	rapidjson::Document document;
	bool bRect = false;
	ssize_t size = 0;
	std::string load_str;

	// read text from json file
	unsigned char* titlech = FileUtils::getInstance()->getFileData(path, "r", &size);
	load_str = std::string((const char*)titlech, size);

	document.Parse<0>(load_str.c_str());
	if(document.HasParseError()) // print the load error
		log("get parse error%s\n", document.GetParseError());

	if(!document.IsObject())
	{
		log("not object, so error\n");
		return ;
	}

	const char* titles[] = {"sound_off", "sound_on", "classic", "color", "soldier"};
	for(int i = 0; i < 5; i++)
	{
		rapidjson::Value& _classic = document[titles[i]];
		setTitle[i] = _classic.GetString();
	}
	for(auto t : setTitle)
	{
		log("classic is %s\n", t.c_str());
	}
}
