#pragma once
#include "cocos2d.h"
#include "Grid.h"

USING_NS_CC;

class GameLayer :
	public cocos2d::Layer
{
public:
	static GameLayer* getInstance();
	virtual bool init();
	virtual void initBg();
	void initGrids();
	void loadGrids(int type);
	void clearGrids();

	void restartGame();
	void undoGame();
	void randGenGrid();

	virtual void onEnter() override;
	virtual void onExit() override;
	bool onTouchBegan(Touch *touch, Event* event);
	void onTouchMoved(Touch* touch, Event* event);
	void onTouchEnded(Touch* touch, Event* event);

	Grid* _grids[4][4];
	int _lastGrids[4][4];

private:
	CREATE_FUNC(GameLayer);
	void clearLastGrids();
	void saveLastGrids();
	void recoverLastGrids();

	// move function
	bool moveToTop();
	bool moveToBottom();
	bool moveToRight();
	bool moveToLeft();

	int getPreGridIndex(const int direction, int row, int column);
	int moveGrid(const int direction, int row, int column, int targetRow, int targetColumn);
	void moveAndUpdate(int value, int targetRow, int targetColumn);
	void moveOnly(int row, int column, int targetRow, int targetColumn);
	void moveAndClear(int row, int column, int targetRow, int targetColumn);

	bool ifOver();

	Vec2 _begin;
	Vec2 _end;
	bool _isOver;
	static GameLayer* _instance;
};

enum Direction
{
	LEFT,
	RIGHT,
	UP,
	DOWN
};