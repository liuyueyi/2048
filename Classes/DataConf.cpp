#include "DataConf.h"
#include "GameTool.h"
#include "GameLayer.h"
#include "json/document.h"
#include "json/stringbuffer.h"
#include "json/rapidjson.h"
#include "json/writer.h"

const char* DataConf::map[] = { "10", "11", "!2", "13", "14", "15", "16" ,"17", "18", "19", "20", "21", "22", "23", "24", "25"};
const char* DataConf::title[] = {"sound_on", "sound_off", "classic", "color", "soldier"};

DataConf* DataConf::_instance = nullptr;
DataConf* DataConf::getInstance()
{
	if(_instance == nullptr)
		_instance = create();

	return _instance;
}

bool DataConf::init()
{
	return true;
}


void DataConf::dumpData(int type)
{
	auto f = UserDefault::getInstance();
	f->setIntegerForKey(Value(type).asString().append("score").c_str(), GameTool::getInstance()->getScore());
	f->setIntegerForKey(Value(type).asString().append("best_score").c_str(), GameTool::getInstance()->getBestScore());
	f->setIntegerForKey("type", type);
	f->setBoolForKey(Value(type).asString().append("exits").c_str(), true);
	log("save type exits %d %d", type, f->getBoolForKey(Value(type).asString().append("exits").c_str(), false));
	//	f->flush();

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
	log("dump the data for type: %d\n", type);
}
