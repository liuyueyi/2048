#pragma once
#include "cocos2d.h"

USING_NS_CC;

//std::string map[16]={"2", "4", "8", "16", "32", "64", "128", "256", "512", "1024", "2048", "4096", "8192", "16384", "32768", "65536"};

class DataConf:
	public Ref
{
public:
	static DataConf* _instance;
	static const char* map[];
	static const char* title[];

	CREATE_FUNC(DataConf);
	static DataConf* getInstance();
	virtual bool init();

	void dumpData(int type);
};