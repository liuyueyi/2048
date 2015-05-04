#pragma once
#include "cocos2d.h"

USING_NS_CC;

class Grid:public cocos2d::Layer
{
public:
	static char* G2U(const char* gb2312);
	static void changeType(int type);
	static int getType();
	CREATE_FUNC(Grid);
	virtual bool init();

	CC_SYNTHESIZE(int, _value, ScoreValue);
	//void updateValue();
	void initValue(int value);
	void initValue(int value, int row, int column);
	bool compareTo(Grid* grid);

	void setLocalPosition(int row, int column);

	void moveAndClear(int targetRow, int targetColumn);
	void moveAndUpdate();

private:
	void loadMap();
	void updateBg();
	Label* _label;
	LayerColor* _bg;
};