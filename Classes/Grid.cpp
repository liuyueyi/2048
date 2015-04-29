#include "Grid.h"
#include "GameTool.h"
#include "GameLayer.h"

//static std::string map[] = {"2", "4", "8", "16", "32", "64", "128", "256", "512", "1024", "2048", "4096", "8192", "16384", "32768", "65536",  "131072", "262144", "524288"};
std::string map[16] = {"0"};
std::string guan[] = {"小兵", "下士", "中士", "上士", "少尉", "中尉", "上尉", "少校", "中校", "上校", "大校", "准将", "少将", "中将", "上将", "元帅"};
std::string num[] = {"2", "4", "8", "16", "32", "64", "128", "256", "512", "1024", "2048", "4096", "8192", "16384", "32768", "65536",  "131072", "262144", "524288"};

static int type = 0;
void Grid::changeType(int _type)
{
	type = _type;
	GameTool::getInstance()->loadScore(type);
	GameLayer::getInstance()->loadGrids(type);
}

int Grid::getType()
{
	return type;
}

char* Grid::G2U(const char* gb2312)
{
	int len = MultiByteToWideChar(CP_ACP, 0, gb2312, -1, NULL, 0);
	wchar_t* wstr = new wchar_t[len+1];
	memset(wstr, 0, len+1);
	MultiByteToWideChar(CP_ACP, 0, gb2312, -1, wstr, len);
	len = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, NULL, NULL);
	char* str = new char[len+1];
	memset(str, 0, len+1);
	WideCharToMultiByte(CP_UTF8, 0, wstr, -1, str, len, NULL, NULL);
	if(wstr) delete[] wstr;
	return str;
}

bool Grid::init()
{
	if(!Layer::init())
		return false;
	
	loadMap();

	this->setContentSize(Size(65, 65));
	_value = 0;
	_bg = LayerColor::create(Color4B(213, 205, 194, 255), 65, 65);
	//_bg->setAnchorPoint(Vec2(32.5f, 32.5f));
	_label = Label::createWithSystemFont(map[_value], "Arial", 30);
	//_label = Label::createWithTTF(map[_value], "MarkerFelt.ttf", 30);
	_label->setPosition(32.5f, 32.5f);
	this->addChild(_bg);
	this->addChild(_label);
	updateBg();

	return true;
}


void Grid::loadMap()
{
	
	if(map[0].compare("0") == 0) // 首次时，装载显示对象
	{
		type = UserDefault::getInstance()->getIntegerForKey("type", 0);
	}

	for(int i = 0; i<16; i++)
	{
		if(type == 0)
			map[i] = Grid::G2U(guan[i].c_str());
		else if(type == 1)
			map[i] = num[i];
		else
			map[i] = "";
	}
}

bool Grid::compareTo(Grid* grid)
{
	if(grid == nullptr)
		return false;

	return _value == grid->getScoreValue();
}

// init the first value of the grid, and the value should be 0 or 1
void Grid::initValue(int value)
{
	_value = value;
	updateBg();
}

void Grid::initValue(int value, int row, int column)
{
	setLocalPosition(row, column);
	initValue(value);
}

void Grid::setLocalPosition(int row, int column)
{
	this->setPosition(column*73 + 8, row*73 + 8);
}

void Grid::moveAndClear(int targetRow, int targetColumn)
{
	auto a1 = MoveTo::create(0.2f, Vec2(73 * targetColumn + 8, 73 * targetRow + 8));
	auto a2 = FadeOut::create(0.2f);
	auto a3 = Spawn::create(a1, a2, nullptr);
	auto a4 = CallFunc::create([&]{
		//log("remove child");
		this->removeFromParent();
	});
	this->runAction(Sequence::create(a3, a4, nullptr));
}

void Grid::moveAndUpdate()
{
	this->setVisible(false);
	auto action01 = ScaleTo::create(0.1f, 1.1f);
	auto action02 = ScaleTo::create(0.1f, 1);
	//auto action03 = DelayTime::create(0.1f);
	auto action03 = FadeIn::create(0.2f);
	auto action04 = Sequence::create(action03, CallFunc::create([&]{
		//log("add child");
		this->setVisible(true);
	}), action01, action02, nullptr);
	this->runAction(action04);
}


void Grid::updateValue()
{
	_value ++;
	if(_value >= 19)
		_value = 19;
	updateBg();
}

void Grid::updateBg()
{
	_label->setString(map[_value]);
	Color3B color, fcolor = Color3B(255, 255, 255);

	switch(_value)
	{
	case 0:
		color = Color3B(247,213,97);
		fcolor = Color3B(120, 120, 120);
		break;
	case 1:
		color = Color3B(166, 232, 103);
		fcolor = Color3B(90, 90, 90);
		break;
	case 2:
		color = Color3B(87, 212, 154);
		break;
	case 3:
		color = Color3B(19, 181, 177);
		break;
	case 4:
		color = Color3B(68, 138, 202);
		break;
	case 5:
		color = Color3B(200, 97, 234);
		break;
	case 6:
		color = Color3B(225,115,181);
		break;
	case 7:
		color = Color3B(238,100, 141);
		break;
	case 8:
		color = Color3B(243, 157, 79);
		break;
	case 9:
		color = Color3B(245, 124, 78);
		break;
	case 10:
		color = Color3B(246, 76, 20);
		break;
	case 11:
		color = Color3B(210, 210, 10); //Color3B(105, 84, 187);
		break;
	case 12:
		color = Color3B(190, 194, 22); // Color3B(50, 86, 204);
		break;
	case 13:
		color = Color3B(160, 160, 10);
		break;
	case 14:
		color =  Color3B(50, 86, 204);// Color3B(24, 66, 149);
		break;
	default:
		color =  Color3B(24, 66, 149);
		break;
	}
	_label->setColor(fcolor);
	_bg->setColor(color);
	
	if(type != 1)
		return;

	if(_value > 15)
		_label->setSystemFontSize(20);
	else if(_value > 12)
		_label->setSystemFontSize(24);
	else if(_value > 8)
		_label->setSystemFontSize(26);

}

