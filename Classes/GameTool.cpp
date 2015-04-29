#include "GameTool.h"
#include "DataConf.h"

GameTool* GameTool::_instance=nullptr;
GameTool* GameTool::getInstance()
{
	if(_instance==nullptr)
		_instance = create();
	return _instance;
}

bool GameTool::init()
{
	do{
		CC_BREAK_IF(!Layer::init());
		// set tool layer's size and position
		this->setContentSize(Size(300, 60));
		this->setPosition(10, 410);

		auto layerIcon = LayerColor::create(Color4B(230, 230, 0, 0), 100, 60);
		auto label = Label::createWithSystemFont("2048+", "Arial", 36);
		//auto label = Label::createWithTTF("小兵传奇", "ClearSansBold.ttf", 36);
		label->setTextColor(Color4B(134,134,134, 255));
		label->setPosition(60, 30);
		layerIcon->addChild(label);
		this->addChild(layerIcon);

		initScore();
	}while(0);
	return true;
}

bool GameTool::initScore()
{
	auto scoreIcon = LayerColor::create(Color4B(186, 172, 159, 255), 80, 50);
	auto scoreTitleLabel = Label::createWithSystemFont("SCORE", "Arial", 16);
	scoreTitleLabel->setPosition(40,35);
	scoreIcon->setPosition(Vec2(130, 5));
	scoreIcon->addChild(scoreTitleLabel);

	scoreLabel = Label::createWithSystemFont(Value(_score).asString(), "Arial", 20);
	scoreLabel->setPosition(40, 10);
	scoreIcon->addChild(scoreLabel);
	this->addChild(scoreIcon);


	auto bestIcon = LayerColor::create(Color4B(186, 172, 159, 255), 80, 50);
	auto bestTitleLabel = Label::createWithSystemFont("BEST", "Arial", 16);
	bestTitleLabel->setPosition(40, 35);
	bestIcon->setPosition(Vec2(220, 5));
	bestIcon->addChild(bestTitleLabel);

	bestLabel = Label::createWithSystemFont(Value(_bestScore).asString(), "Arial", 20);
	bestLabel->setPosition(40, 10);
	bestIcon->addChild(bestLabel);
	this->addChild(bestIcon);

	// 首次从文件中读取最高分
	loadScore(UserDefault::getInstance()->getIntegerForKey("type", 1));
	return true;
}

void GameTool::loadScore(int type)
{
	setScore(UserDefault::getInstance()->getIntegerForKey(Value(type).asString().append("score").c_str(), 0));
	setBestScore(UserDefault::getInstance()->getIntegerForKey(Value(type).asString().append("best_score").c_str(), 0));
}

int GameTool::getScore()
{
	return _score;
}

void GameTool::setScore(int score)
{
	_score = score;
	scoreLabel->setString(Value(_score).asString());
}

int GameTool::getBestScore()
{
	return _bestScore;
}
void GameTool::setBestScore(int bestScore)
{
	_bestScore = bestScore;
	bestLabel->setString(Value(_bestScore).asString());
}

void GameTool::updateScore(int addScore)
{
	setScore(_score + addScore);
	updateBestScore(); // update the bset score if necessary
}

void GameTool::resetScore()
{
	setScore(0);
}

void GameTool::updateBestScore()
{
	if(_score < _bestScore)
		return ;

	setBestScore(_score);
}