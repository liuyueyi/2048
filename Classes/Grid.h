#pragma once
#include "cocos2d.h"

USING_NS_CC;

class Grid:public cocos2d::Layer
{
public:
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

	void initAction();
	void moveOnly(int targetRow, int targetColumn);
	void moveAndClear(int targetRow, int targetColumn);
	void moveAndUpdate();

	static const char* guan[];

private:
	void loadSoldier();
	void loadMap();
	void updateBg();
	Label* _label;
	LayerColor* _bg;
};

enum StateType{
	SOLDIER, // 0
	CLASSIC, // 1
	COLOR // 2
};