#include "GameLayer.h"
#include "GameTool.h"
#include "DataConf.h"

static GameLayer* _instance;
int isMove = 0; // 大于0时，表示移动了当前的数字布局，因此需要自动保存上一次的布局结果, 在移动完毕后在RandGenGrid中进行自动清0
GameLayer* GameLayer::getInstance()
{
	if(_instance == nullptr)
		_instance = create();
	return _instance;
}


bool GameLayer::init()
{
	do{
		CC_BREAK_IF(!Layer::init());

		this->setContentSize(Size(300, 300));
		this->setPosition(10, 65);

		initBg();
	}while(0);
	return true;
}

void GameLayer::initBg()
{
	// init the rand seed
	struct timeval now;
	gettimeofday(&now, NULL);
	srand((unsigned int)(now.tv_sec * 1000 + now.tv_usec / 1000));

	auto bg = LayerColor::create(Color4B(186,172,159,255), 300, 300);
	this->addChild(bg);
	for(int i = 0; i < 4; i++)
	{
		for(int j = 0; j < 4; j++)
		{
			auto cellBg = LayerColor::create(Color4B(213, 205, 194, 255), 65, 65);
			cellBg->setPosition(73 * j + 8, 73 * i + 8);
			this->addChild(cellBg);
		}
	}
	// 首次启动时，从文件中恢复上次的状态
	loadGrids(UserDefault::getInstance()->getIntegerForKey("type"));
}

void GameLayer::initGrids()
{
	for(int row = 0; row < 4; row++)
	{
		for(int column = 0; column < 4; column++)
		{
			_grids[row][column] = nullptr;
		}
	}
	clearLastGrids();
	randGenGrid();
	randGenGrid();
}

void GameLayer::loadGrids(int type)
{
	auto f = UserDefault::getInstance();
	if(!f->isXMLFileExist() || !f->getBoolForKey(Value(type).asString().append("exits").c_str(), false))
		return initGrids();
	
	int value = 0;
	for(int i = 0; i < 4; i++)
	{
		for(int j = 0; j < 4; j++)
		{
			value = f->getIntegerForKey(Value(type*3).asString().append(Value(100+i*4+j).asString()).c_str());
			if(value == -1)
				_grids[i][j] = nullptr;
			else
			{
				_grids[i][j] = Grid::create();
				_grids[i][j]->initValue(value, i, j);
				this->addChild(_grids[i][j]);
			}
			_lastGrids[i][j] = f->getIntegerForKey(Value(type*3).asString().append(Value(i*4+j).asString()).c_str());
		}
	}
}

void GameLayer::clearGrids()
{
	for(int row = 0; row < 4; row++)
	{
		for(int column = 0; column < 4; column++)
		{
			if(_grids[row][column] != nullptr)
				_grids[row][column]->removeFromParent();
			_grids[row][column] = nullptr;
		}
	}
}

#define EMPTY -1 // 表示lastGrids中保存的是上次的布局
#define DOUBLE_EMPTY -2 // 表示_lastGrids中保存的是初始化的结果
void GameLayer::clearLastGrids()
{
	for(int row = 0; row < 4; row++)
	{
		for(int column = 0; column < 4; column++)
		{
			_lastGrids[row][column] = DOUBLE_EMPTY;
		}
	}
}
void GameLayer::restartGame()
{
	GameTool::getInstance()->resetScore();
	clearGrids();
	clearLastGrids();
	randGenGrid();
	randGenGrid();
}

void GameLayer::undoGame()
{
	if(_lastGrids[0][0] != DOUBLE_EMPTY) // 非初始化时，才能调用undo
	{
		clearGrids();
		recoverLastGrids();
	}
}

void GameLayer::saveLastGrids()
{
	if(++isMove != 1)
		return;

	for(int row = 0; row < 4; row++)
	{
		for(int column = 0; column < 4; column++)
		{
			if(_grids[row][column] != nullptr)
				_lastGrids[row][column] = _grids[row][column]->getScoreValue();
			else
				_lastGrids[row][column] = EMPTY;
		}
	}
}

void GameLayer::recoverLastGrids()
{
	for(int row = 0; row < 4; row++)
	{
		for(int column = 0; column < 4; column++)
		{
			if(_lastGrids[row][column]  > EMPTY)
			{
				_grids[row][column] = Grid::create();
				_grids[row][column]->initValue(_lastGrids[row][column], row, column);
				this->addChild(_grids[row][column]);
			}
			else
				_grids[row][column] = nullptr;
		}
	}
}

void GameLayer::randGenGrid()
{
	isMove = 0; // 新增一个数字，标记isMove为0，即从新开始判断是否移动
	int value, row, column;
	do{
		value = CCRANDOM_0_1() * 16;
		row = value / 4;
		column = value % 4;
	}while(_grids[row][column] != nullptr);

	auto item = Grid::create();
	item->initValue((int)(CCRANDOM_0_1()*2), row, column);
	_grids[row][column] = item;
	item->setScale(0);
	item->runAction(ScaleTo::create(0.3f, 1));
	this->addChild(item);
}

void GameLayer::onEnter()
{
	Layer::onEnter();
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(GameLayer::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(GameLayer::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(GameLayer::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void GameLayer::onExit()
{
	Layer::onExit();
	/*log("eixt%d", DataConf::getInstance()->getModelType());*/
	DataConf::getInstance()->dumpData(Grid::getType());
}

bool GameLayer::onTouchBegan(Touch* touch, Event* event)
{
	_begin = touch->getLocation();
	auto rect = Rect(this->getPosition().x, this->getPosition().y, this->getContentSize().width, this->getContentSize().height);
	return rect.containsPoint(_begin); // means only touch the game box, then response
}

void GameLayer::onTouchMoved(Touch* touch, Event* event)
{
}
void GameLayer::onTouchEnded(Touch* touch, Event* event)
{
	_end = touch->getLocation();
	auto value = _end - _begin;

	if(abs(value.x) > abs(value.y)) // left or right move
	{
		if(value.x - 5 > 0) // to right
			moveToRight();
		//else if(value.x + 5 < 0) // to left
		else if(value.x + 5 < 0)
			moveToLeft();
		else
			return;
	}
	else
	{
		if(value.y - 5 > 0) // to up
			moveToTop();
		else if(value.y + 5 < 0)
			moveToBottom();
		else
			return;
	}

	if(isMove > 0) // if moved, then add a new Grid item
		randGenGrid();

	// judge if game over

}

#define ERRORINDEX -1
int GameLayer::getPreGridIndex(const int direction, int row, int column)
{
	switch(direction)
	{
	case Direction::LEFT:
		while(--column >= 0)
			if(_grids[row][column] != nullptr)
				return column;
		break;
	case Direction::RIGHT:
		while(++column <= 3)
			if(_grids[row][column] != nullptr)
				return column;
		break;
	case Direction::UP:
		while(++row <= 3)
			if(_grids[row][column] != nullptr)
				return row;
		break;
	case Direction::DOWN:
		while(--row >= 0)
			if(_grids[row][column] != nullptr)
				return row;
		break;
	}
	return ERRORINDEX;
}

// move the grid to target place and return the pre_grid index
int GameLayer::moveGrid(const int direction, int row, int column, int targetRow, int targetColumn)
{
	int addScore = 0;

	int preColumnOrRow = getPreGridIndex(direction, row, column);
	if(preColumnOrRow == ERRORINDEX)
	{
		moveOnly(row, column, targetRow, targetColumn);
		return ERRORINDEX;
	}

	int preRow, preCol;
	int tag = -1;
	switch(direction)
	{
	case Direction::LEFT:
		tag = 1;
	case Direction::RIGHT:
		preRow = row;
		preCol = preColumnOrRow;
		break;
	case Direction::DOWN:
		tag = 1;
	case Direction::UP:
		preRow = preColumnOrRow;
		preCol = column;
		break;
	}

	if(_grids[row][column]->compareTo(_grids[preRow][preCol])) // can merge two number
	{
		auto value = _grids[row][column]->getScoreValue();
		moveAndClear(row, column, targetRow, targetColumn);
		moveAndClear(preRow, preCol, targetRow, targetColumn);
		moveAndUpdate(++value, targetRow, targetColumn);
		GameTool::getInstance()->updateScore(pow(2, ++value));
		return preColumnOrRow;
	}

	moveOnly(row, column, targetRow, targetColumn);
	return preColumnOrRow + tag;
}

bool GameLayer::moveToRight()
{
	auto target = 3;
	auto preColumn = 0;
	for(int row = 0; row < 4; row ++)
	{
		target = 3;
		for(int column = 3; column >= 0; column --)
		{
			if(_grids[row][column] == nullptr)
				continue;
			preColumn = moveGrid(Direction::LEFT, row, column, row, target);
			if(preColumn == ERRORINDEX)
				break;
			target--;
		}
	}
	return isMove>0;
}

bool GameLayer::moveToLeft()
{
	auto target = 0;
	auto preColumn = 0;
	for(int row = 0; row < 4; row++)
	{
		target = 0;
		for(int column = 0; column < 4; column++)
		{
			if(_grids[row][column] == nullptr)
				continue;

			preColumn = moveGrid(Direction::RIGHT, row, column, row, target);
			if(preColumn == ERRORINDEX)
				break;
			target ++;
		}
	}
	return isMove>0;
}

bool GameLayer::moveToTop()
{
	int target = 3;
	int preRow = 0;
	for(int column = 0; column < 4; column++)
	{
		target = 3;
		for(int row=3; row >= 0; row--)
		{
			if(_grids[row][column] == nullptr)
				continue;
			preRow = moveGrid(Direction::DOWN, row, column, target, column);
			if(preRow == ERRORINDEX)
				break;
			target --;
		}
	}
	return isMove>0;
}

bool GameLayer::moveToBottom()
{
	int target = 0;
	int preRow = 0;
	for(int column = 0; column < 4; column++)
	{
		target = 0;
		for(int row=0; row < 4; row++)
		{
			if(_grids[row][column] == nullptr)
				continue;
			preRow = moveGrid(Direction::UP, row, column, target, column);
			if(preRow == ERRORINDEX)
				break;
			target ++;
		}
	}
	return isMove>0;
}

void GameLayer::moveOnly(int row, int column, int targetRow, int targetColumn)
{
	if(row == targetRow && column == targetColumn)
		return;

	saveLastGrids(); // 仅第一移动时，保存上一次的布局

	auto action = MoveTo::create(0.1f, Vec2(73*targetColumn + 8, 73*targetRow+ 8));
	_grids[row][column]->runAction(action);
	_grids[targetRow][targetColumn] = _grids[row][column];
	_grids[row][column] = nullptr;
}

void GameLayer::moveAndUpdate(int value, int targetRow, int targetColumn)
{
	auto temp = Grid::create();
	temp->initValue(value, targetRow, targetColumn);
	_grids[targetRow][targetColumn] = temp;
	this->addChild(_grids[targetRow][targetColumn]);
	temp->moveAndUpdate();
}

void GameLayer::moveAndClear(int row, int column, int targetRow, int targetColumn)
{
	saveLastGrids();

	_grids[row][column]->moveAndClear(targetRow, targetColumn);
	_grids[row][column] = nullptr;
}