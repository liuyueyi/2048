#include "DataConf.h"
#include "GameTool.h"
#include "GameLayer.h"

static DataConf* instance;
DataConf* DataConf::getInstance()
{
	if(instance == nullptr)
		instance = create();

	return instance;
}

bool DataConf::init()
{
	return true;
}

//void DataConf::loadMap()
//{
//	extern std::string map[16];
//	auto all = FileUtils::getInstance()->getValueMapFromFile("data.plist");
//	auto type = all.at("type").asInt();
//	log("the type is %d\n", type);
//
//	for(int i = 0; i < 16; i++)
//	{
//		if(type == 0)
//			map[i] = "";
//		else
//			map[i] = all.at(Value(type+i).asString()).asString();
//	}
//}

void DataConf::dumpData(int type)
{
	auto f = UserDefault::getInstance();
	f->setIntegerForKey(Value(type).asString().append("score").c_str(), GameTool::getInstance()->getScore());
	f->setIntegerForKey(Value(type).asString().append("best_score").c_str(), GameTool::getInstance()->getBestScore());
	f->setIntegerForKey("type", type);
	f->setBoolForKey(Value(type).asString().append("exits").c_str(), true);
	f->flush();

	Grid* temp;
	int value;
	for(int i = 0; i < 4; i++)
	{
		for(int j = 0; j < 4; j++)
		{
			temp = GameLayer::getInstance()->_grids[i][j];
			if(temp == nullptr)
				value = -1;
			else
				value = temp->getScoreValue();
			f->setIntegerForKey(Value(type*3).asString().append(Value(100+i*4+j).asString()).c_str(), value);
			f->setIntegerForKey(Value(type*3).asString().append(Value(i*4+j).asString()).c_str(), GameLayer::getInstance()->_lastGrids[i][j]);
		}
	}
	f->flush();
}
