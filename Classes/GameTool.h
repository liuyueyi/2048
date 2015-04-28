#pragma once
#include "cocos2d.h"

USING_NS_CC;

class GameTool :
	public cocos2d::Layer
{
public:
	static GameTool* getInstance();

	virtual bool init();
	virtual bool initScore();
	void loadScore(int type);
	void updateScore(int addScore);
	void resetScore();

	void setScore(int score);
	int getScore();
	void setBestScore(int bestScore);
	int getBestScore();
private:
	CREATE_FUNC(GameTool);
	void updateBestScore();

	Label* scoreLabel;
	Label* bestLabel;

	int _score;
	int _bestScore;
};