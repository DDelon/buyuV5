#include "LuaCppAdapter.h"
#include "AppDelegate.h"
#include "FishFrameManager.h"
#include "BulletManager.h"
#include "Player/PlayerManager.h"

#include "NetsManager.h"
#include "StringUtil.h"
#include "DebugLog.h"
#include "CoinManager.h"
#include "scripting/lua-bindings/manual/CCLuaEngine.h"
#include "AudioControl.h"
#include "Collision/CollisionManager.h"
#include "SkillLockManager.h"



extern "C"
{
#include "lua.h"  
#include "lauxlib.h"  
#include "lualib.h"  
}
//#define LUA_DEBUG

LuaCppAdapter *LuaCppAdapter::_pInstance = nullptr;
LuaCppAdapter *LuaCppAdapter::getInstance()
{
	if (_pInstance == nullptr)
	{
		_pInstance = new LuaCppAdapter();
	}

	return _pInstance;
}

LuaCppAdapter::LuaCppAdapter()
{
	_pLuaState = LuaEngine::getInstance()->getLuaStack()->getLuaState();
	_playerId = 0;
	_gameType = 0;
}

LuaCppAdapter::~LuaCppAdapter()
{
}

void LuaCppAdapter::addDebugLayer(Layer *pLayer, std::string roomId)
{
#if LUA_DEBUG > 0
	Size winSize = Director::getInstance()->getWinSize();
	Layer *pDebugInfoLayer = LayerColor::create(Color4B(0, 0, 0, 255));
	pDebugInfoLayer->setContentSize(Size(100, 100));
	_pRoomIdLabel = LabelTTF::create(roomId, "Arial", 24);
	_pRoomIdLabel->setPosition(pDebugInfoLayer->getContentSize().width / 2, pDebugInfoLayer->getContentSize().height*0.8);
	pDebugInfoLayer->addChild(_pRoomIdLabel, 1, 6665);
	_pTimelineLabel = LabelTTF::create("timelineIndex: 1", "Arial", 24);
	_pTimelineLabel->setPosition(pDebugInfoLayer->getContentSize().width / 2, pDebugInfoLayer->getContentSize().height*0.5);
	pDebugInfoLayer->addChild(_pTimelineLabel, 1, 6666);
	_pFrameLabel = LabelTTF::create("frame: 1", "Arial", 24);
	_pFrameLabel->setPosition(pDebugInfoLayer->getContentSize().width / 2, pDebugInfoLayer->getContentSize().height*0.2);
	pDebugInfoLayer->addChild(_pFrameLabel, 1, 6667);
	pDebugInfoLayer->setPosition(Vec2(winSize.width / 2, winSize.height*0.85));
	pLayer->addChild(pDebugInfoLayer, 888);
#endif
}

void LuaCppAdapter::frameRunning(int frame)
{
#if LUA_DEBUG > 0
	Value frameStr(frame);
	_pFrameLabel->setString(frameStr.asString());
#endif
}

void LuaCppAdapter::changeTimeline(int timelineIndex)
{
#if LUA_DEBUG > 0
	Value timelineStr(timelineIndex);
	_pTimelineLabel->setString(timelineStr.asString());
#endif
}

void LuaCppAdapter::debugLog(std::string log)
{
	DebugLog::getInstance()->log(log);
}

void LuaCppAdapter::openRankWeb(std::string url)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	using namespace cocos2d::experimental::ui;
	Scene *pCurScene = Director::getInstance()->getRunningScene();
	WebView *webView = WebView::create();
	webView->setPosition(Vec2(pCurScene->getContentSize().width*0.5, pCurScene->getContentSize().height*0.5));
	webView->setContentSize(Size(500, 500));
	webView->loadURL(url);
	webView->setScalesPageToFit(false);
	webView->setVisible(true);
	DebugLog::getInstance()->log("web:"+url);
	webView->setOnShouldStartLoading([](WebView *sender, const std::string &url){
		return true;
	});
	webView->setOnDidFinishLoading([](WebView *sender, const std::string &url){
		DebugLog::getInstance()->log("111111111111111111");
	});
	webView->setOnDidFailLoading([](WebView *sender, const std::string &url){
		DebugLog::getInstance()->log("2222222222222222222");
	});
	pCurScene->addChild(webView, 8184);
#endif
}

void LuaCppAdapter::startGame(Layer *pManagerLayer, Layer *pFishLayer, ValueMap data, ValueVector killedFishes, ValueVector calledFishes)
{
	//BulletManager::getInstance()->delPlayerCannon(-1);
	//获取玩家id
	for (auto iter : data["playerTab"].asValueVector())
	{
		ValueMap info = iter.asValueMap();
		auto infoIter = info.find("playerInfo");
		if (infoIter == info.end())
			continue;
		if (infoIter->second.asValueMap()["isSelf"].asBool())
		{
			_playerId = infoIter->second.asValueMap()["playerId"].asInt();
		}
	}
	pManagerLayer->addChild(FishFrameManager::getInstance());
	pManagerLayer->addChild(CoinManager::getInstance());
	pManagerLayer->addChild(PlayerManager::getInstance());

	CoinManager::getInstance()->setWinScle(data["winScaleX"].asFloat(), data["winScaleY"].asFloat());
	Layer* coinLayer = Layer::create();
	pFishLayer->getParent()->addChild(coinLayer,10);
	CoinManager::getInstance()->setCoinLayer(coinLayer);

	addDebugLayer(pManagerLayer, data["roomId"].asString());
	FishFrameManager::getInstance()->setFishLayer(pFishLayer);
	FishFrameManager::getInstance()->setFilpVal(data["isFlip"].asBool());

	bool isInGroup = data["isInGroup"].asBool();
	int index = data["timelineIndex"].asInt();
	int frame = data["frameId"].asInt();
	int roomId = data["roomLv"].asInt();
	FishFrameManager::getInstance()->addKilledFishes(killedFishes);
	FishFrameManager::getInstance()->setRoomLv(roomId);
	log("-----------------%d", index);
	if (isInGroup)
	{
		//正在刷鱼潮
		FishFrameManager::getInstance()->addFishGroup(index, frame);
		//FishFrameManager::getInstance()->addFishGroup(4, 0);
	}
	else
	{
		//刷时间线
		FishFrameManager::getInstance()->addFishTimeline(index, frame);
		//300011127 300007778
// 		Fish *pFish = FishFrameManager::getInstance()->bornFish(100000157, 300007778, 0, 0, 0);
// 		pFish->setBornFrame(0);
// 		FishFrameManager::getInstance()->getFishLayer()->addChild(pFish);
// 		Fish *pFish1 = FishFrameManager::getInstance()->bornFish(100000158, 300011127, 0, 0, 0);
// 		pFish1->setBornFrame(0);
// 		FishFrameManager::getInstance()->getFishLayer()->addChild(pFish1);
	}

	//召唤鱼
	FishFrameManager::getInstance()->addCalledFish(calledFishes);
	
	FishFrameManager::getInstance()->start();
	FishFrameManager::getInstance()->clearKilledFishesVec();
	CollisionManager::getInstance()->init(FishFrameManager::getInstance(), BulletManager::getInstance());
}

void LuaCppAdapter::exitGame()
{
	CollisionManager::getInstance()->clearData();
	FishFrameManager::getInstance()->reset();
	FishFrameManager::getInstance()->removeFromParent();
	CoinManager::getInstance()->removeFromParent();
	PlayerManager::getInstance()->reset();
	PlayerManager::getInstance()->removeFromParent();
	BulletManager::getInstance()->reset();
	SkillLockManager::getInstance()->reset();
	NetsManager::getInstance()->reset();
	_gameType = 0;
}

void LuaCppAdapter::startBullet(Layer *pBulletLayer, ValueMap data)
{
	DebugLog::getInstance()->log("---------------00000----startBullet--------1------------0-----------");
	pBulletLayer->addChild(BulletManager::getInstance());
	BulletManager::getInstance()->start(pBulletLayer, data);
}

void LuaCppAdapter::startNets(Layer *pNetsLayer)
{
	pNetsLayer->addChild(NetsManager::getInstance());
	NetsManager::getInstance()->start(pNetsLayer);
}

void LuaCppAdapter::syncFrame(ValueMap data)
{
	FishFrameManager::getInstance()->syncFrame(data);
}

void LuaCppAdapter::setGunRate(int gunRate)
{

}

void LuaCppAdapter::fishSwithColor(ValueMap data)
{
	
}

void LuaCppAdapter::fishGroupCome(ValueMap data)
{
	int index = data["index"].asInt();
	

	FishFrameManager::getInstance()->runAction(Sequence::create(DelayTime::create(0.5f), CallFuncN::create([=](Ref *pSender){
		FishFrameManager::getInstance()->reset();
		FishFrameManager::getInstance()->addFishGroup(index, 10);
		FishFrameManager::getInstance()->start();
		this->callLuaFunc("FishGMF", "CppToLua", "enableUseSkill", ValueMap());
	}), NULL));

	//FishFrameManager::getInstance()->moveToOut();

}

void LuaCppAdapter::fishTimeLineCome(ValueMap data)
{
	//bool isContinue = data["isContinue"].asBool();
	int index = data["index"].asInt();
	FishFrameManager::getInstance()->runAction(Sequence::create(DelayTime::create(1.0f), CallFuncN::create([=](Ref *pSender){
		FishFrameManager::getInstance()->reset();
		FishFrameManager::getInstance()->addFishTimeline(index, 20);
		FishFrameManager::getInstance()->start();
	}), NULL));
	FishFrameManager::getInstance()->moveToOut();

}

void LuaCppAdapter::fishAccelerateOut()
{
	FishFrameManager::getInstance()->moveToOut();
}

Vec3 LuaCppAdapter::getPathPos(int pathId, int frame)
{
	std::vector<Vec3>* pathTab = FishFrameManager::getInstance()->getPath(pathId);
	Size winSize = Director::getInstance()->getWinSize();
	float widthPercent = winSize.width / BASE_RESOLUTION_WIDTH;
	float heightPercent = winSize.height / BASE_RESOLUTION_HEIGHT;
	Vec3 point = (*pathTab)[frame / PATH_INTERVAL];
	point.x = (point.x)*widthPercent;
	point.y = (point.y)*heightPercent;
	bool isFlip = FishFrameManager::getInstance()->getFilpVal();
	if (isFlip)
	{
		Size winSize = Director::getInstance()->getWinSize();
		point = Vec3(winSize.width - point.x, winSize.height - point.y, point.z - 180);
	}
	return point;
}

void LuaCppAdapter::callFish(ValueMap data)
{
	ValueVector dataVec;
	dataVec.push_back(Value(data));
	FishFrameManager::getInstance()->addCalledFish(dataVec);
}

void LuaCppAdapter::loadDataBin()
{
	std::string searchPath = FileUtils::getInstance()->getWritablePath();
	bool isExistBin = FileUtils::getInstance()->isFileExist(searchPath + "res/luaconfig/GameData.bin");
	if (isExistBin)
	{
		ParseGameData::getInstance()->loadGameData((searchPath + "res/luaconfig/GameData.bin").c_str());
	}
	else
	{
		ParseGameData::getInstance()->loadGameData("res/luaconfig/GameData.bin");
	}

	TableMap tabValue = ParseGameData::getInstance()->getTabMap("fishpathEx");
	for (auto iter : tabValue)
	{
		string key = iter.first;
		if (key == "nil")
		{
			continue;
		}
		FieldMap val = iter.second;
		DataValue dataVal = val["pointdata"];
		string strVal = dataVal.asString();

		vector<std::string> vec3Vect;
		vector<Vec3> onePath;
		StringUtil::_STR_SPLIT(strVal, ";", vec3Vect);
		for (auto vec3Val : vec3Vect)
		{
			vector<std::string> vec3ValVect;
			StringUtil::_STR_SPLIT(vec3Val, ",", vec3ValVect);
			if (vec3Val == "")
			{
				break;
			}
			Vec3 point(atof(vec3ValVect[0].c_str()), atof(vec3ValVect[1].c_str()), atof(vec3ValVect[2].c_str()));
			onePath.push_back(point);
		}

		FishFrameManager::getInstance()->addPath(atoi(key.c_str()), onePath);
	}
	tabValue.clear();

}

ValueVector LuaCppAdapter::getNBombKilledFishes(int bombLv, Vec2 pos)
{
	std::vector<Fish *> effectedFishes;
	ValueVector fishTab;
	FishFrameManager::getInstance()->getNBombFish(bombLv, pos, effectedFishes);

	for (auto iter : effectedFishes)
	{
		ValueMap fishData;
		fishData["timelineId"] = iter->getTimelineId();
		fishData["fishArrayId"] = iter->getFishArrayId();

		fishTab.push_back(Value(fishData));
	}
	return fishTab;
}

std::string LuaCppAdapter::getConfigData(std::string configName, std::string keyID, std::string keyName)
{
	TableMap temp = ParseGameData::getInstance()->getTabMap(configName);
	DataValue valMap = temp[keyID][keyName];
	std::string res = valMap.asString();

	return res;
}

int LuaCppAdapter::getCurFrame()
{
	return FishFrameManager::getInstance()->getCurrentFrame();
}

ValueMap LuaCppAdapter::getConfigDataByName(std::string keyName)
{
	TableMap temp = ParseGameData::getInstance()->getTabMap("config");
	FieldMap valMap = temp[keyName];

	ValueMap res;
	for (auto iter : valMap)
	{
		res[iter.first] = iter.second.asString();
	}

	return res;
}

void LuaCppAdapter::playerFire(ValueMap data)
{
	int playerId = data["playerId"].asInt();
	Player* player = PlayerManager::getInstance()->getPlayerById(playerId);
	if (player == NULL)
	{
		printf("-------playerFire----player == NULL--------");
		return;
	}

	int fireType = data["fireType"].asInt();
	int rate = data["bulletRate"].asInt();
	int propId = GOLD;
	int propCount = rate;
	if (data["createType"].asString() == "friend")
	{
		propId = BULLET;
		propCount = 1;
		if (!data["isCost"].asBool())
		{
			propCount = 0;
		}
	}
	if (data["effectId"].asInt() == 17)
		propCount *= 2;

	//log("--------------effectId:%d, %d", data["effectId"].asInt(), propCount);
	if (fireType == 0)	//其他人射击
	{
		float angle = data["degree"].asFloat();
		int lifeTime = data["lifeTime"].asInt();
		string bulletId = data["bulletId"].asString();
		int timelineId = data["timelineId"].asInt();
		int fishArrayId = data["fishArrayId"].asInt();
		int effectId = data["effectId"].asInt();
		Vec2 pos = Vec2(data["pos"].asValueMap()["x"].asFloat(), data["pos"].asValueMap()["y"].asFloat());
		player->shoot(propId, propCount);
		BulletManager::getInstance()->createBullet(bulletId, playerId, pos, angle, lifeTime, timelineId, fishArrayId, rate, effectId);
		
		//已用的子弹更新
		int newUsedBullet = player->getRoleData()->getPropData(USED_BULLET).realCount + propCount;
		player->getRoleData()->setPropRealData(USED_BULLET, newUsedBullet);

// 		if (timelineId != 0)
// 		{
// 			bool isChangeAimFish = false;
// 			Fish* aimFish = SkillLockManager::getInstance()->getAimFish(playerId);
// 			if (aimFish != nullptr)
// 			{
// 				int aimTimelineId = aimFish->getTimelineId();
// 				if (aimTimelineId != timelineId)
// 				{
// 					isChangeAimFish = true;
// 				}
// 			}
// 			else
// 			{
// 				isChangeAimFish = true;
// 			}
// 			if (isChangeAimFish)
// 			{
// 				printf("--------isChangeAimFish == true------------------");
// 				ValueMap lockFishData;
// 				lockFishData["setType"] = 3;
// 				lockFishData["playerId"] = playerId;
// 				lockFishData["timelineId"] = timelineId;
// 				lockFishData["fishArrayId"] = fishArrayId;
// 				SkillLockManager::getInstance()->setLockData(lockFishData);
// 			}
// 		}
	}
	else if (fireType == 1)	//自己提前发射子弹，假扣钱
	{
		float angle = data["degree"].asFloat();
		int lifeTime = data["lifeTime"].asInt();
		string bulletId = data["bulletId"].asString();
		int timelineId = data["timelineId"].asInt();
		int fishArrayId = data["fishArrayId"].asInt();
		int effectId = data["effectId"].asInt();
		Vec2 pos = Vec2(data["pos"].asValueMap()["x"].asFloat(), data["pos"].asValueMap()["y"].asFloat());
		player->getRoleData()->isSurePropData(false, propId, propCount, bulletId);
		BulletManager::getInstance()->createBullet(bulletId, playerId, pos, angle, lifeTime, timelineId, fishArrayId, rate, effectId);
		
		//已用的子弹计入缓存
		player->getRoleData()->isSurePropData(false, USED_BULLET, propCount);
	}
	else if (fireType == 2)	//自己确认已发射子弹，真扣钱
	{
		string bulletId = data["bulletId"].asString();
		int newGold = player->getRoleData()->getPropData(propId).realCount - propCount;
		player->getRoleData()->setPropRealData(propId, newGold);
		player->getRoleData()->isSurePropData(true, propId, propCount, bulletId);

		//已用的子弹更新
		int newUsedBullet = player->getRoleData()->getPropData(USED_BULLET).realCount + propCount;
		player->getRoleData()->setPropRealData(USED_BULLET, newUsedBullet);
		player->getRoleData()->isSurePropData(true, USED_BULLET, propCount);
		//player->shoot(rate);
	}

}

void LuaCppAdapter::bulletChangeTarget(ValueMap data)
{

}

void LuaCppAdapter::updateThunderRate(int rate)
{
	FishFrameManager::getInstance()->setThunderRate(rate);
}

void LuaCppAdapter::updateNBombRate(int rate)
{
	FishFrameManager::getInstance()->setNBombRate(rate);
}

void LuaCppAdapter::removePlayerBullet(ValueMap data)
{
	BulletManager::getInstance()->clearBullet(data["id"].asInt());
}

void LuaCppAdapter::otherPlayerBulletCollision(ValueMap data)
{
	std::string bulletId = data["bulletId"].asString();
	BulletManager::getInstance()->bulletCollision(bulletId);
}

void LuaCppAdapter::setCannon(Node *pCannon, int playerId)
{
	BulletManager::getInstance()->addPlayerCannon(playerId, pCannon);
}

//得到当前等级和下一级差多少
cocos2d::ValueMap LuaCppAdapter::getLVByExp(int curExp)
{
	ValueMap resultMap;
	int keyID = 980000001;
	while (true)
	{
		if (GAMEVALUE_STRING("level", IntToString(keyID+1), "id").empty())
		{
			resultMap["level"] = GAMEVALUE_INT("level", IntToString(keyID), "level");
			resultMap["expNext"] = 0;
			resultMap["expCur"] = 0;
			break;
		}
		int exp = GAMEVALUE_INT("level", IntToString(keyID), "exp");
		int expNext = GAMEVALUE_INT("level", IntToString(keyID +1), "exp");
		curExp = curExp - exp;
		if (curExp < expNext)
		{
			resultMap["level"] = GAMEVALUE_INT("level", IntToString(keyID), "level");
			resultMap["expNext"] = GAMEVALUE_INT("level", IntToString(keyID + 1), "exp");
			resultMap["expCur"] = curExp;
			break;
		}
		keyID = keyID + 1;
	}
	return resultMap;

}

ValueMap LuaCppAdapter::getGunRate(int type, int curRate, int minGunRate, int maxGunRate, float coin)
{
	printf("-----getGunRate---type=%d----curRate=%d---minGunRate=%d---maxGunRate=%d---coin=%d\n", type, curRate, minGunRate, maxGunRate, coin );
	TableMap temp = ParseGameData::getInstance()->getTabMap("cannon");
	ValueMap map;
	int allCount = 50;
	if (type == 1)
	{
		if (curRate < minGunRate)
		{
			map["Rate"] = curRate;
			return map;
		}
		//得到下一个炮倍
		for (int i = 0; i < allCount; ++i)
		{
			std::string keyID = StringUtil::format("%d", 920000001 + i);
			DataValue valMap1 = temp[keyID]["times"];
			int rate = valMap1.asInt();
			if (valMap1.asString().empty())
			{
				map["Rate"] = minGunRate;
				return map;
			}
			if (curRate == maxGunRate && rate >curRate)
			{
				map["Rate"] = rate;
				return map;
			}
			else if (curRate > maxGunRate)
			{
				map["Rate"] = minGunRate;
				return map;
			}
			else if (curRate < maxGunRate && rate > curRate)
			{
				map["Rate"] = rate;
				return map;
			}
		}
	}
	else if (type == 2)
	{
		if (curRate < minGunRate)
		{
			map["Rate"] = curRate;
			return map;
		}
		//得到上一个炮倍
		for (int i = allCount; i >= 0; --i)
		{
			std::string keyID = StringUtil::format("%d", 920000001 + i);
			DataValue valMap1 = temp[keyID]["times"];
			int rate = valMap1.asInt();
			if (valMap1.asString().empty())
			{
				continue;
			}
			if (curRate == minGunRate)
			{
				std::string keyID2 = StringUtil::format("%d", 920000001 + i - 1);
				DataValue valMap2 = temp[keyID2]["times"];
				int rate2 = valMap2.asInt();

				if ((rate > maxGunRate && rate2 == maxGunRate) || (rate >= maxGunRate && rate2 < maxGunRate))
				{
					map["Rate"] = rate;
					return map;
				}
			}
			else if (rate < curRate)
			{
				map["Rate"] = rate;
				return map;
			}
		}
	}
	else if (type == 3)
	{
		//得到上一个炮倍
		for (int i = allCount; i >= 0; --i)
		{
			std::string keyID = StringUtil::format("%d", 920000001 + i);
			DataValue valMap1 = temp[keyID]["times"];
			int rate = valMap1.asInt();
			if (valMap1.asString().empty())
			{
				continue;
			}
			if (rate <= coin && rate < curRate)
			{
				map["Rate"] = rate;
				return map;
			}
		}
	}
	else if (type == 4)
	{
		//得到下一个炮倍
		for (int i = 0; i < allCount; ++i)
		{
			std::string keyID = StringUtil::format("%d", 920000001 + i);
			DataValue valMap1 = temp[keyID]["times"];
			int rate = valMap1.asInt();
			if (valMap1.asString().empty())
			{
				map["Rate"] = 0;
				return map;
			}
			if (rate > maxGunRate)
			{
				map["Rate"] = rate;
				map["unlock_gem"] = temp[keyID]["unlock_gem"].asInt();
				map["unlock_award"] = temp[keyID]["unlock_award"].asInt();
				return map;
			}
		}
	}
	return map;
}

void LuaCppAdapter::callLuaFunc(std::string globalTabName, std::string funcName, std::string type, ValueMap val)
{
	ValueMap package;
	package["type"] = type;
	package["data"] = val;

	lua_getglobal(_pLuaState, globalTabName.c_str());
	lua_pushstring(_pLuaState, funcName.c_str());
	lua_gettable(_pLuaState, -2);
	pushValueMap(_pLuaState, package);
	lua_pcall(_pLuaState, 1, 0, 0);
}

void LuaCppAdapter::getLuaValue(std::string globalTabName, std::string valName, Value &val)
{
	lua_getglobal(_pLuaState, globalTabName.c_str());
	if (!lua_istable(_pLuaState, -1))
	{
		DebugLog::getInstance()->log(globalTabName + " is invalid table");
	}
	else
	{
		lua_getfield(_pLuaState, -1, valName.c_str());
		if (lua_isnumber(_pLuaState, -1))
		{
			val = Value((unsigned int)lua_tointeger(_pLuaState, -1));
		}
		else if (lua_isboolean(_pLuaState, -1))
		{
			val = Value(lua_toboolean(_pLuaState, -1));
		}

		lua_pop(_pLuaState, -1);
	}
}

std::string LuaCppAdapter::getVIPResId(int playerId)
{
	std::string temp = _vipResMap[playerId];
	return temp;
}

void LuaCppAdapter::pushValueMap(lua_State *pState, ValueMap val)
{
	//log("-----------------------------------------1111111111111111111111111111111111111111");
	//lua_getglobal(pState, "testFunc");
	lua_newtable(pState);                                              /* L: table */
	for (auto it = val.begin(); it != val.end(); ++it)
	{
		lua_pushstring(pState, it->first.c_str());                     /* L: table key */
		pushValue(pState, it->second);                                     /* L: table key value */
		lua_rawset(pState, -3);                     /* table.key = value, L: table */
		//lua_settable(_pState, -3);
	}
}

void LuaCppAdapter::pushInt(lua_State *pState, int val)
{
	lua_pushnumber(pState, val);
}

void LuaCppAdapter::pushFloat(lua_State *pState, float val)
{
	lua_pushnumber(pState, val);
}

void LuaCppAdapter::pushBoolean(lua_State *pState, bool boolValue)
{
	lua_pushboolean(pState, boolValue);
}

void LuaCppAdapter::pushString(lua_State *pState, const char* val)
{
	lua_pushstring(pState, val);
}

void LuaCppAdapter::pushDouble(lua_State *pState, int val)
{
	lua_pushnumber(pState, val);
}

void LuaCppAdapter::pushVector(lua_State *pState, ValueVector val)
{
	lua_newtable(pState);                                              /* L: table */
	int index = 1;
	for (auto it = val.begin(); it != val.end(); ++it)
	{
		pushInt(pState, index);
		pushValue(pState, *it);                                            /* L: table value */
		lua_rawset(pState, -3);          /* table[index] = value, L: table */
		++index;
	}
}

void LuaCppAdapter::pushValue(lua_State *pState, Value val)
{
	Value::Type type = val.getType();
	switch (type)
	{
	case cocos2d::Value::Type::INTEGER:
		pushInt(pState, val.asInt());
		break;
	case cocos2d::Value::Type::FLOAT:
		pushFloat(pState, val.asFloat());
		break;
	case cocos2d::Value::Type::DOUBLE:
		pushDouble(pState, val.asDouble());
		break;
	case cocos2d::Value::Type::BOOLEAN:
		pushBoolean(pState, val.asBool());
		break;
	case cocos2d::Value::Type::STRING:
		pushString(pState, val.asString().c_str());
		break;
	case cocos2d::Value::Type::VECTOR:
		pushVector(pState, val.asValueVector());
		break;
	case cocos2d::Value::Type::MAP:
		pushValueMap(pState, val.asValueMap());
		break;
	case cocos2d::Value::Type::INT_KEY_MAP:
		log("未定义int key map");
		break;
	default:
		break;
	}
}

cocos2d::ValueMapIntKey LuaCppAdapter::getValueTable(ValueMap& data)
{
	ValueMapIntKey vm;
	TableMap& tm = ParseGameData::getInstance()->getTabMap(data["name"].asString());

	TableMap::iterator it = tm.begin();
	int i = 0;
	while (it != tm.end()) {
		FieldMap fm = it->second;
		ValueMap vmField;
		FieldMap::iterator itField = fm.begin();

		bool isInvalid = false;
		while (itField != fm.end()) {
			if (itField->first ==  "id" 
				&& itField->second.asString() == "") {
				isInvalid = true;
				break;
			}

			vmField.insert(make_pair(itField->first, Value(itField->second.asString())));
			itField++;
		}

		if (isInvalid) {
			isInvalid = false; // databin 里面可能有nil的元素, 因为excel表格不完全空, 某一行有个空格之类的
			it++;
			continue;
		}

		vm.insert(make_pair(i, Value(vmField)));

		i++;
		it++;
	}
	
	return vm;
	//data["result"]
}

void LuaCppAdapter::setFishState(ValueMap data)
{
	int state = data["state"].asInt();
	if (state == 1)
	{
		FishFrameManager::getInstance()->resumeAllFish();
	}
	else if (state == 2)
	{

	}
	else if (state == 3)
	{

	}
	else if (state == 4)
	{
		FishFrameManager::getInstance()->pauseAllFish(4);
	}
	else if (state == 5)
	{
		FishFrameManager::getInstance()->pauseAllFish(5);
	}
}

cocos2d::ValueMap LuaCppAdapter::getRoomFish(int curRoomID)
{
	ValueMap resultMap;
	int keyID = 820000000;
	int count = 0;
	while (true)
	{
		if (GAMEVALUE_STRING("roomfish", IntToString(keyID), "room_id").empty())
		{
			break;
		}
		int room_id = GAMEVALUE_INT("roomfish", IntToString(keyID), "room_id");
		if (room_id == curRoomID)
		{
			count = count + 1;
			ValueMap map3;
			map3["fish_id"] = GAMEVALUE_INT("roomfish", IntToString(keyID), "fish_id");
			map3["show_score"] = GAMEVALUE_INT("roomfish", IntToString(keyID), "show_score");
			map3["fish_type"] = GAMEVALUE_INT("roomfish", IntToString(keyID), "fish_type");
			resultMap[IntToString(count)] = map3;
		}
		keyID = keyID + 1;
	}

	resultMap["count"] = count;
	return resultMap;
}

cocos2d::ValueMap LuaCppAdapter::getShopData()
{
	ValueMap resultMap;

	int keyID = 830000001;
	ValueMap countMap;
	while (true)
	{
		if (GAMEVALUE_STRING("recharge", IntToString(keyID), "id").empty())
		{
			break;
		}
		int recharge_type = GAMEVALUE_INT("recharge", IntToString(keyID), "recharge_type");
		ValueMap data;
		data["id"] = GAMEVALUE_INT("recharge", IntToString(keyID), "id");
		data["recharge_type"] = GAMEVALUE_INT("recharge", IntToString(keyID), "recharge_type");
		data["recharge"] = GAMEVALUE_INT("recharge", IntToString(keyID), "recharge");
		data["recharge_num"] = GAMEVALUE_INT("recharge", IntToString(keyID), "recharge_num");
		data["gift_num"] = GAMEVALUE_INT("recharge", IntToString(keyID), "gift_num");
		data["recharge_method"] = GAMEVALUE_INT("recharge", IntToString(keyID), "recharge_method");
		data["frist_change_enable"] = GAMEVALUE_INT("recharge", IntToString(keyID), "frist_change_enable");
		data["recharge_res"] = GAMEVALUE_STRING("recharge", IntToString(keyID), "recharge_res");
		data["recharge_name"] = GAMEVALUE_STRING("recharge", IntToString(keyID), "recharge_name");

		if (resultMap[IntToString(recharge_type)].isNull())
		{
			ValueMap data2;
			resultMap[IntToString(recharge_type)] = data2;
			countMap[IntToString(recharge_type)] = 0;
		}

		int newCount = countMap[IntToString(recharge_type)].asInt() + 1;
		countMap[IntToString(recharge_type)] = newCount;

		(resultMap[IntToString(recharge_type)]).asValueMap()[IntToString(newCount)] = data;
		(resultMap[IntToString(recharge_type)]).asValueMap()["count"] = newCount;
		keyID = keyID + 1;
	}

	return resultMap;
}

cocos2d::ValueMap LuaCppAdapter::getItemData(int itemID)
{
	ValueMap resultMap;
	TableMap& tm = ParseGameData::getInstance()->getTabMap("item");
	TableMap::iterator it = tm.begin();
	int count = 0;
	while (it != tm.end()) {
		FieldMap fm = it->second;
		if (fm["id"].asString().empty())
		{
			it++;
			continue;
		}
		int id = fm["id"].asInt();
		if (id == itemID)
		{
			resultMap = getFieldMapData(fm);
			return resultMap;
		}
		it++;
	}

	return resultMap;
}

cocos2d::ValueMap LuaCppAdapter::getAllItemData()
{
 	ValueMap resultMap;

	TableMap& tm = ParseGameData::getInstance()->getTabMap("item");
	TableMap::iterator it = tm.begin();
	int count = 0;
	while (it != tm.end()) {
		FieldMap fm = it->second;
		if (fm["id"].asString().empty())
		{
			it++;
			continue;
		}
		count = count + 1;
		int id = fm["id"].asInt();
		//resultMap[IntToString(count)] = getItemData(id);
		resultMap[IntToString(count)] = getFieldMapData(fm);
		it++;
	}
	resultMap["count"] = count;
	return resultMap;

}

cocos2d::ValueMap LuaCppAdapter::getFieldMapData(FieldMap data)
{
	ValueMap resultMap;
	auto it = data.begin();
	while (it != data.end()) {
		string val = (it->second).asString();
		string key = it->first;
		if (key.empty())
		{
			it++;
			continue;
		}
		resultMap[key] = val;
		it++;
	}

	return resultMap;
}

cocos2d::ValueMap LuaCppAdapter::getForgedData(int id)
{
	ValueMap resultMap;
	resultMap["times"] = GAMEVALUE_STRING("cannon", IntToString(id), "times");
	resultMap["interval"] = GAMEVALUE_STRING("cannon", IntToString(id), "interval");
	resultMap["unlock_gem"] = GAMEVALUE_STRING("cannon", IntToString(id), "unlock_gem");
	resultMap["unlock_award"] = GAMEVALUE_STRING("cannon", IntToString(id), "unlock_award");

	ValueMap itemMap;
	string str = GAMEVALUE_STRING("cannon", IntToString(id), "unlock_item");
	if (str.length() > 0)
	{
		vector<std::string> offsetVec1;
		StringUtil::_STR_SPLIT(str, ";", offsetVec1);
		for (auto iter1 : offsetVec1)
		{
			if (iter1.length() > 0)
			{
				vector<std::string> offsetVec2;
				StringUtil::_STR_SPLIT(iter1, ",", offsetVec2);
				itemMap[offsetVec2[0]] = offsetVec2[1];
			}
		}
	}
	resultMap["unlock_item"] = itemMap;

	resultMap["unlock_prob"] = GAMEVALUE_STRING("cannon", IntToString(id), "unlock_prob");
	resultMap["succ_need"] = GAMEVALUE_STRING("cannon", IntToString(id), "succ_need");
	return resultMap;
}

cocos2d::ValueMap LuaCppAdapter::getForgedData(int id, int endId)
{
	ValueMap resultMap;
	resultMap[GAMEVALUE_STRING("cannon", IntToString(id), "times")] = getForgedData(id);
	if (endId != -1)
	{
		while (true)
		{
			id = id + 1;
			if (id > endId || GAMEVALUE_STRING("cannon", IntToString(id), "id").empty())
			{
				break;
			}
			resultMap[GAMEVALUE_STRING("cannon", IntToString(id), "times")] = getForgedData(id);
		}
	}
	return resultMap;
}

int LuaCppAdapter::getGunInterval(int curRate)
{
	int keyID = 920000001;
	while (true)
	{
		if (GAMEVALUE_STRING("cannon", IntToString(keyID), "id").empty())
		{
			break;
		}
		int interval = GAMEVALUE_INT("cannon", IntToString(keyID), "interval");
		int times = GAMEVALUE_INT("cannon", IntToString(keyID), "times");
		if (times == curRate)
		{
			return interval;
		}
		keyID = keyID + 1;
	}
	return 0;
}

ValueMap LuaCppAdapter::getVIPByCostMoney(int money_need)
{
	ValueMap resultMap;
	int keyID = 840000000;
	while (true)
	{
		if (GAMEVALUE_STRING("vip", IntToString(keyID+1), "id").empty())
		{
			resultMap["vip_level"] = GAMEVALUE_INT("vip", IntToString(keyID), "vip_level");
			resultMap["next_All_money"] = 0;
			resultMap["extra_sign"] = GAMEVALUE_INT("vip", IntToString(keyID), "extra_sign");
			resultMap["daily_items_reward"] = GAMEVALUE_STRING("vip", IntToString(keyID), "daily_items_reward");
			return resultMap;
		}
		int money_need_min = GAMEVALUE_INT("vip", IntToString(keyID), "money_need");
		int money_need_Max = GAMEVALUE_INT("vip", IntToString(keyID+1), "money_need");
		if (money_need >= money_need_min && money_need < money_need_Max)
		{
			resultMap["vip_level"] = GAMEVALUE_INT("vip", IntToString(keyID), "vip_level");
			resultMap["next_All_money"] = money_need_Max;
			resultMap["extra_sign"] = GAMEVALUE_INT("vip", IntToString(keyID), "extra_sign");
			resultMap["daily_items_reward"] = GAMEVALUE_STRING("vip", IntToString(keyID), "daily_items_reward");
			return resultMap;
		}
		keyID = keyID + 1;
	}

	return resultMap;
}

cocos2d::ValueMap LuaCppAdapter::getVIPDataByKeyID(int keyID)
{
	ValueMap resultMap;
	if (GAMEVALUE_STRING("vip", IntToString(keyID), "id").empty())
	{
		resultMap["cannon_type"] = 0;
		resultMap["extra_sign"] = 0;
		return resultMap;
	}
	resultMap["cannon_type"] = GAMEVALUE_INT("vip", IntToString(keyID), "cannon_type");
	resultMap["extra_sign"] = GAMEVALUE_INT("vip", IntToString(keyID), "extra_sign");
	resultMap["daily_items_reward"] = GAMEVALUE_STRING("vip", IntToString(keyID), "daily_items_reward");

	int id = (resultMap["cannon_type"]).asInt() + 930000000;
	resultMap["cannon_img"] = GAMEVALUE_STRING("cannonoutlook", IntToString(id), "cannon_img");
	resultMap["base_img"] = GAMEVALUE_STRING("cannonoutlook", IntToString(id), "base_img");
	

	ValueVector textMap;
	string str = GAMEVALUE_STRING("vip", IntToString(keyID), "show_text");
	vector<std::string> offsetVec;
	StringUtil::_STR_SPLIT(str, ";", offsetVec);
	for (auto iter : offsetVec)
	{
		textMap.push_back(Value(iter));
	}
	resultMap["show_text"] = textMap;

	
	return resultMap;
}

cocos2d::ValueMap LuaCppAdapter::myCreateBullet(ValueMap data)
{
	//失败类型，0.成功	1.没有玩家	2.子弹数太多	3.切换炮倍	4.当前炮倍大于自己最高炮倍	5.没钱了 6,当前炮倍大于自己最高炮倍并且炮倍大于1000
	ValueMap map;
	ValueMap bulletData = data["data"].asValueMap();
	int playerId = bulletData["playerId"].asInt();
	Player* player = PlayerManager::getInstance()->getPlayerById(playerId);
	if (player ==NULL)
	{
		map["isSucceed"] = 1;
		return map;
	}

	RoleData* roleData = player->getRoleData();
	float showGold = roleData->getGlod().realCount - roleData->getGlod().flyingCount - roleData->getGlod().unSureCount;
	if (showGold <= 0)
	{
		map["isSucceed"] = 5;
		return map;
	}

	//判断子弹数
	int bulletCount = BulletManager::getInstance()->getBulletCount(playerId);
	int id = PlayerManager::getInstance()->getCurGameRoomID();
	int maxCount = GAMEVALUE_INT("room", IntToString(id + 910000000), "max_bullet");
	if (bulletCount >= maxCount)
	{
		map["isSucceed"] = 2;
		return map;
	}

	int maxGunRate = roleData->maxGunRate;
	if (roleData->maxGunRate < roleData->currentGunRate)
	{
		if (roleData->maxGunRate >= 1000)
		{
			map["isSucceed"] = 6;
		}
		else
		{
			map["isSucceed"] = 4;
		}
		return map;
	}

	//低于房间最低炮倍，判断是否需要切换
	int effectId = bulletData["effectId"].asInt();
	float multiple = 1;
	if (effectId == 17)
	{
		multiple = 0.5;
	}
	showGold = showGold*multiple;
	int mCurRoomMinRate = PlayerManager::getInstance()->getCurRoomMinRate();
	if (showGold < mCurRoomMinRate)
	{

		int curMaxRate = getGunRate(3, maxGunRate, mCurRoomMinRate, maxGunRate, showGold)["Rate"].asInt();
		if (curMaxRate != roleData->currentGunRate)
		{
			map["isSucceed"] = 3;
			map["currentGunRate"] = curMaxRate;
			if (curMaxRate > 0)
			{
				roleData->currentGunRate = curMaxRate;
			}
			
			return map;
		}
	}

	if (showGold < roleData->currentGunRate)
	{
		map["isSucceed"] = 3;
		//帮助玩家自动切换炮倍
		int curMaxRate = getGunRate(3, roleData->currentGunRate, mCurRoomMinRate, maxGunRate, showGold)["Rate"].asInt();
		if (curMaxRate != roleData->currentGunRate)
		{
			map["currentGunRate"] = curMaxRate;
			if (curMaxRate > 0)
			{
				roleData->currentGunRate = curMaxRate;
			}
		}
		return map;
	}
	else
	{
		int timelineId = 0;
		int fishArrayId = 0;
		if (SkillLockManager::getInstance()->getIsLock())
		{
			Fish* aimFish = SkillLockManager::getInstance()->getAimFish(playerId);
			if (aimFish == nullptr)
			{
				map["isSucceed"] = 7;
				return map;
			}
			timelineId = aimFish->getTimelineId();
			fishArrayId = aimFish->getFishArrayId();
			Node* cannon = BulletManager::getInstance()->getCannon(playerId);
			map["lockDegree"] = cannon->getRotation() + 90;
		}
		map["timelineId"] = timelineId;
		map["fishArrayId"] = fishArrayId;
		bulletData["timelineId"] = timelineId;
		bulletData["fishArrayId"] = fishArrayId;
		map["isSucceed"] = 0;
		map["bulletRate"] = roleData->currentGunRate;
		map["frameId"] = FishFrameManager::getInstance()->getCurrentFrame();
		bulletData["bulletRate"] = roleData->currentGunRate;
		this->playerFire(bulletData);
		return map;
	}
	return map;
}

//创建朋友场的子弹
cocos2d::ValueMap LuaCppAdapter::myCreateFriendBullet(ValueMap data)
{
	//失败类型，0.成功	1.没有玩家	2.飞行子弹数太多	3.没子弹了 
	ValueMap map;
	ValueMap bulletData = data["data"].asValueMap();
	int playerId = bulletData["playerId"].asInt();
	Player* player = PlayerManager::getInstance()->getPlayerById(playerId);
	if (player == NULL)
	{
		map["isSucceed"] = 1;
		return map;
	}

	RoleData* roleData = player->getRoleData();
	if (bulletData["isCost"].asBool())
	{
		int showCount = roleData->getPropData(BULLET).realCount - roleData->getPropData(BULLET).flyingCount - roleData->getPropData(BULLET).unSureCount;
		if (showCount <= 0)
		{
			map["isSucceed"] = 3;
			return map;
		}
	}

	//判断子弹数
	int bulletCount = BulletManager::getInstance()->getBulletCount(playerId);
	int id = 4;
	int maxCount = GAMEVALUE_INT("room", IntToString(id + 910000000), "max_bullet");
	if (bulletCount >= maxCount)
	{
		map["isSucceed"] = 2;
		return map;
	}

	int timelineId = 0;
	int fishArrayId = 0;
	if (SkillLockManager::getInstance()->getIsLock())
	{
		Fish* aimFish = SkillLockManager::getInstance()->getAimFish(playerId);
		if (aimFish == nullptr)
		{
			map["isSucceed"] = 7;
			return map;
		}
		timelineId = aimFish->getTimelineId();
		fishArrayId = aimFish->getFishArrayId();
		Node* cannon = BulletManager::getInstance()->getCannon(playerId);
		map["lockDegree"] = cannon->getRotation() + 90;
	}
	map["timelineId"] = timelineId;
	map["fishArrayId"] = fishArrayId;
	bulletData["timelineId"] = timelineId;
	bulletData["fishArrayId"] = fishArrayId;

	map["isSucceed"] = 0;
	map["bulletRate"] = roleData->currentGunRate;
	map["frameId"] = FishFrameManager::getInstance()->getCurrentFrame();
	bulletData["bulletRate"] = roleData->currentGunRate;
	playerFire(bulletData);

	PropData newUsedBullet = player->getRoleData()->getPropData(USED_BULLET);
	int UsedBulletShowCount = newUsedBullet.realCount + newUsedBullet.flyingCount + newUsedBullet.unSureCount;

	map["usedBulletShowCount"] = UsedBulletShowCount;
	return map;

}

cocos2d::ValueMap LuaCppAdapter::getVIPData()
{
	ValueMap resultMap;
	int keyID = 0;
	while (true)
	{
		if (GAMEVALUE_STRING("vip", IntToString(keyID + 840000000), "id").empty())
		{
			break;
		}
		resultMap[IntToString(keyID)] = getVIPDataByKeyID(keyID + 840000000);
		keyID = keyID + 1;
	}

	return resultMap;
}

cocos2d::ValueMap LuaCppAdapter::getRewardData()
{
	ValueMap resultMap;
	int keyID = 1;
	while (true)
	{
		if (GAMEVALUE_STRING("reward", IntToString(keyID + 940000000), "id").empty())
		{
			break;
		}

		ValueMap map;
		map["limit"] = GAMEVALUE_INT("reward", IntToString(keyID + 940000000), "limit");
		map["reward"] = GAMEVALUE_STRING("reward", IntToString(keyID + 940000000), "reward");

		string keyStr = StringUtil::format("%d", keyID);
		resultMap[keyStr] = map;

		keyID = keyID + 1;
	}

	resultMap["count"] = keyID-1;
	return resultMap;
}

void LuaCppAdapter::addPlayer(ValueMap map)
{
	if (map["addType"].asString() == "friend")
	{
		PlayerManager::getInstance()->addFriendPlayer(map);
	}
	else
	{
		PlayerManager::getInstance()->addPlayer(map);
	}
}

void LuaCppAdapter::bindUI(ValueMap map, ui::TextBMFont* text, ui::TextBMFont* addText)
{
	PlayerManager::getInstance()->bindUI(map, text, addText);
}

void LuaCppAdapter::updateInline(ValueMap data)
{
	string upDataType = data["upDataType"].asString();
	//log("-------------------------------------upDataType=%s", upDataType.c_str());
	if (upDataType == "onPlayerShoot")	//玩家射击
	{
		playerFire(data);
	}
	else if (upDataType == "onHit")	//玩家击中结果
	{
		FishFrameManager::getInstance()->onHit(data);
	}
	else if (upDataType == "refreshNormalPropData")	//普通道具添加，删除,使用
	{
		int playerId = data["playerId"].asInt();
		int useType = data["useType"].asInt();
		float updateDelayTime = data["updateDelayTime"].asFloat();
		ValueMap propData = data["propData"].asValueMap();
		PlayerManager::getInstance()->refreshNormalPropData(playerId, propData, useType, updateDelayTime);
	}
	else if (upDataType == "upDataByPropId")	//刷新真实数据
	{
		int playerId = data["playerId"].asInt();
		int propId = data["propId"].asInt();
		int propCount = data["propCount"].asInt();
		bool isShow = data["isShow"].asBool();

		int flyingCount = data["flyingCount"].asInt();
		float delayTime = data["delayTime"].asFloat();
		if (isShow)
		{
			PlayerManager::getInstance()->refreshPropData(playerId, propId, propCount, flyingCount, delayTime);
		}
		else
		{
			PlayerManager::getInstance()->setPropRealData(playerId, propId, propCount);
		}
		
	}
	else if (upDataType == "setAddFlyProp")		//飞行数据
	{
		PlayerManager::getInstance()->setAddFlyProp(data);
	}
	else if (upDataType == "isSurePropData")		//临时数据
	{
		PlayerManager::getInstance()->isSurePropData(data);
	}
	else if (upDataType == "addTrueAndFlyProp")	//增加真实数量
	{
		PlayerManager::getInstance()->addTrueAndFlyProp(data);
	}
	else if (upDataType == "ApplyAlmResult")	//救济金申请结果
	{
		PlayerManager::getInstance()->coinJumpAct(data);
	}
	else if (upDataType == "CannonUpgrade")	//炮升级
	{
		int addMoneyCount = PlayerManager::getInstance()->coinJumpAct(data);

		//更新真实水晶
		int playerId = data["playerId"].asInt();
		int propId = GEM;
		int propCount = data["newCrystal"].asInt();
		PlayerManager::getInstance()->refreshPropData(playerId, propId, propCount, 0, 0);

	}
	else if (upDataType == "refreshSeniorPropData")	//高级道具添加，删除,使用
	{
		int playerId = data["playerId"].asInt();
		int propId = data["propId"].asInt();
		int useType = data["useType"].asInt();
		float updateDelayTime = data["updateDelayTime"].asFloat();
		ValueMap propData = data["propData"].asValueMap();
		PlayerManager::getInstance()->refreshSeniorPropData(playerId, propId, propData, useType, updateDelayTime);
	}
	else if (upDataType == "clearAllSeniorProp")	//删除所有高级道具
	{
		int playerId = data["playerId"].asInt();
		int clearType = data["clearType"].asInt();
		PlayerManager::getInstance()->clearAllSeniorProp(playerId, clearType);
	}
	else if (upDataType == "clearUnSureData")
	{
		int playerId = data["playerId"].asInt();
		Player* player = PlayerManager::getInstance()->getPlayerById(playerId);
		if (player != NULL)
		{
			player->getRoleData()->clearUnSureData();
		}
	}
	else if (upDataType == "bossRateChangeEnd")
	{
		int playerId = data["playerId"].asInt();
		int chairId = data["chairId"].asInt();
		int fishId = data["fishId"].asInt();
		int coinNum = data["coinNum"].asInt();
		float posX = data["posX"].asFloat();
		float posY = data["posY"].asFloat();
		int dropCoin = data["dropCoin"].asInt();
		int costPropId = data["costPropId"].asInt();
		CoinManager::getInstance()->killBonusFish(coinNum, fishId, chairId, playerId);
		float allDelayTime = CoinManager::getInstance()->showGainCoinEffect(playerId, chairId, coinNum, dropCoin, Vec2(posX, posY), playerId == LuaCppAdapter::getInstance()->getPlayerId());
		Player* player = PlayerManager::getInstance()->getPlayerById(playerId);
		if (player != NULL)
		{
			auto act = Sequence::create(DelayTime::create(allDelayTime), CallFunc::create([=](){
				printf("-----allDelayTime=%f-----------coinNum=%d", allDelayTime, coinNum);
				player->getRoleData()->setAddFlyProp(true, costPropId, coinNum); 
			}), nullptr);
			player->runAction(act);
		}
	}
}

ValueMap LuaCppAdapter::luaUseCppFun(ValueMap data)
{
	ValueMap map;
	string funName = data["funName"].asString();
	if (funName == "updateInline")
	{
		ValueVector vecData = data["data"].asValueVector();
		for (int i = 0; i < vecData.size(); i++)
		{
			ValueMap data = vecData[i].asValueMap();
			updateInline(data);
		}
	}
	else if (funName == "myCreateBullet")	//创建我的子弹并返回是否成功和倍率和frameId
	{
		string createType = data["data"].asValueMap()["createType"].asString();
		if (createType == "friend")
		{
			map = myCreateFriendBullet(data);
		}
		else
		{
			map = myCreateBullet(data);
		}

		return map;
	}
	else if (funName == "setCurGameRoomID")
	{
		int curGameRoomID = data["curGameRoomID"].asInt();
		PlayerManager::getInstance()->setCurGameRoomID(curGameRoomID);
	}
	else if (funName == "setFishState")
	{
		this->setFishState(data);
	}
	else if (funName == "getBulletCount")
	{
		int playerId = data["playerId"].asInt();
		int bulletCount = BulletManager::getInstance()->getBulletCount(playerId);
		map["bulletCount"] = bulletCount;
		return map;
	}
	else if (funName == "getLockFishByScore")
	{
		Fish* aimFish = FishFrameManager::getInstance()->getLockFishByScore();
		if (aimFish != nullptr)
		{
			map["timelineId"] = aimFish->getTimelineId();
			map["fishArrayId"] = aimFish->getFishArrayId();
		}
		else
		{
			map["timelineId"] = 0;
			map["fishArrayId"] = 0;
		}
		return map;
	}
	else if (funName == "showLockPointUI")
	{
		FishFrameManager::getInstance()->showLockPointUI();
	}
	else if (funName == "getFishByPos")
	{
		float posX = data["posX"].asFloat();
		float posY = data["posY"].asFloat();
		Fish* aimFish = FishFrameManager::getInstance()->getFishByPos(Vec2(posX, posY));
		if (aimFish != nullptr)
		{
			map["timelineId"] = aimFish->getTimelineId();
			map["fishArrayId"] = aimFish->getFishArrayId();
		}
		else
		{
			map["timelineId"] = 0;
			map["fishArrayId"] = 0;
		}

		return map;
	}
	else if (funName == "getAimFishPos")
	{
		int playerId = data["playerId"].asInt();
		Fish* aimFish = SkillLockManager::getInstance()->getAimFish(playerId);
		if (aimFish != nullptr)
		{
			Vec2 lockPos = aimFish->getLockPosition();
			map["posX"] = lockPos.x;
			map["posY"] = lockPos.y;
			map["state"] = aimFish->getFishState();
		}
		else
		{
			map["posX"] = 0;
			map["posY"] = 0;
			map["state"] = 0;
		}

		return map;
	}
	else if (funName == "getLockBullet")
	{
		int playerId = data["playerId"].asInt();
		map = BulletManager::getInstance()->getLockBullet(playerId);
		return map;
	}
	else if (funName == "changePlayerBulletAim")
	{
		int playerId = data["playerId"].asInt();
		int timelineId = data["timelineId"].asInt();
		int fishArrayId = data["fishArrayId"].asInt();
		ValueMap bullers = data["bullers"].asValueMap();
		map = BulletManager::getInstance()->getLockBullet(playerId);
		return map;
	}
	else if (funName == "setFishRed")
	{
		int timelineId = data["timelineId"].asInt();
		int fishArrayId = data["fishArrayId"].asInt();
		FishFrameManager::getInstance()->setFishRed(timelineId, fishArrayId);
	}
	else if (funName == "setFishDeath")
	{
		int timelineId = data["timelineId"].asInt();
		int fishArrayId = data["fishArrayId"].asInt();
		FishFrameManager::getInstance()->setFishDeath(timelineId, fishArrayId);
	}
	else if (funName == "getFishByID")
	{
		int timelineId = data["timelineId"].asInt();
		int fishArrayId = data["fishArrayId"].asInt();
		Fish* aimFish = FishFrameManager::getInstance()->getFishByID(timelineId, fishArrayId);
		if (aimFish != nullptr)
		{
			map["posX"] = aimFish->getPositionX();
			map["posY"] = aimFish->getPositionY();
			map["score"] = aimFish->getScore();
			map["death_effect"] = aimFish->getDeathEffect();
			map["coin_num"] = aimFish->getDropCoin();
		}
		else
		{
			map["posX"] = 0;
			map["posY"] = 0;
			map["score"] = 0;
			map["death_effect"] = 0;
			map["coin_num"] = 0;
		}

		return map;
	}
	else if (funName == "getRewardData")
	{
		map = getRewardData();
		return map;
	}
	else if (funName == "delPlayerCannon")
	{
		BulletManager::getInstance()->delPlayerCannon(data["data"].asInt());
	}
	else if (funName == "getRoomFish")
	{
		int curRoomID = data["curRoomID"].asInt();
		map = this->getRoomFish(curRoomID);
		return map;
	}
	else if (funName == "getShopData")
	{
		map = this->getShopData();
		return map;
	}
	else if (funName == "getItemData")
	{
		int itemID = data["itemID"].asInt();
		map = this->getItemData(itemID);
		return map;
	}
	else if (funName == "getAllItemData")
	{
		map = this->getAllItemData();
		return map;
	}
	else if (funName == "getGunInterval")
	{
		int curRate = data["curRate"].asInt();
		map["gunInterval"] = this->getGunInterval(curRate);
		return map;
	}
	else if (funName == "setGunChange")
	{
		std::string id = data["id"].asString();
		int playerId = data["playerId"].asInt();

		_vipResMap[playerId] = id;


	}
	else if (funName == "getGunChangeData")
	{
		string id = data["id"].asString();
		map["type"] = GAMEVALUE_STRING("cannonoutlook", id, "type");
		map["base_img"] = GAMEVALUE_STRING("cannonoutlook", id, "base_img");
		map["cannon_img"] = GAMEVALUE_STRING("cannonoutlook", id, "cannon_img");
		map["fire_pos"] = GAMEVALUE_STRING("cannonoutlook", id, "fire_pos");
		map["bullet_img"] = GAMEVALUE_STRING("cannonoutlook", id, "bullet_img");
		map["fire_effect"] = GAMEVALUE_STRING("cannonoutlook", id, "fire_effect");
		map["net_res"] = GAMEVALUE_STRING("cannonoutlook", id, "net_res");
		map["net_radius"] = GAMEVALUE_STRING("cannonoutlook", id, "net_radius");
		return map;
	}
	else if (funName == "setForgedChange")
	{

	}
	else if (funName == "getForgedChangeData")
	{
		int id = data["id"].asInt();
		int endId = -1;
		if (!data["endId"].isNull())
		{
			endId = data["endId"].asInt();
		}
		map = this->getForgedData(id, endId);
		return map;
	}
	else if (funName == "getVIPByCostMoney")
	{
		int costMoney = data["costMoney"].asInt();
		map = this->getVIPByCostMoney(costMoney);

		return map;
	}
	else if (funName == "getVIPDataByKeyID")
	{
		int keyID = data["keyID"].asInt();
		map = this->getVIPDataByKeyID(keyID);

		return map;
	}
	else if (funName == "getVIPData")
	{
		map = this->getVIPData();
		return map;
	}
	else if (funName == "showGunUpEffect")
	{
		int playerId = data["playerId"].asInt();
		int chairId = data["chairId"].asInt();
		int moneyCount = data["moneyCount"].asInt();
		int coinNum = data["coinNum"].asInt();
		CoinManager::getInstance()->showGunUpEffect(playerId, chairId, moneyCount, coinNum);
	}
	else if (funName == "AudioControl")
	{
		int setType = data["setType"].asInt();
		if (setType == 1)
		{
			bool BackgroundMusicStatus = data["MusicStatus"].asBool();
			AudioControl::getInstance()->setBackgroundMusicStatus(BackgroundMusicStatus);
		}
		else if (setType == 2)
		{
			bool EffectStatus = data["EffectStatus"].asBool();
			AudioControl::getInstance()->setEffectStatus(EffectStatus);
		}
		else if (setType == 3)
		{
			float BackgroundMusicVolume = data["BackgroundMusicVolume"].asFloat();
			AudioControl::getInstance()->setBackgroundMusicVolume(BackgroundMusicVolume);
		}
		else if (setType == 4)
		{
			float EffectVolume = data["EffectVolume"].asFloat();
			AudioControl::getInstance()->setEffectsVolume(EffectVolume);
		}

	}
	else if (funName == "getLoginType")
	{
		map["loginType"] = LUA_DEBUG;
	}
	else if (funName == "delPlayer")
	{
		int playerId = data["playerId"].asInt();
		PlayerManager::getInstance()->delPlayer(playerId);
	}
	else if (funName == "isSurePropData")
	{
		PlayerManager::getInstance()->isSurePropData(data);
	}
	else if (funName == "setMyPlayerId")
	{
		int playerId = data["playerId"].asInt();
		PlayerManager::getInstance()->setMyPlayerId(playerId);
	}
	else if (funName == "getPlayerPropData")
	{
		int playerId = data["playerId"].asInt();
		int propId = data["propId"].asInt();
		int propItemId = data["propItemId"].asInt();
		Player* player = PlayerManager::getInstance()->getPlayerById(playerId);
		if (player != NULL)
		{
			if (propItemId == 0 )
			{
				PropData propData = player->getRoleData()->getPropData(propId);
				map["realCount"] = propData.realCount;
				map["flyingCount"] = propData.flyingCount;
				map["unSureCount"] = propData.unSureCount;
			}
			else
			{
				map = player->getRoleData()->getSeniorPropData(propId, propItemId);
			}
		}
	}
	else if (funName == "setIsBankup")
	{
		int playerId = data["playerId"].asInt();
		bool isBankup = data["isBankup"].asBool();
		Player* player = PlayerManager::getInstance()->getPlayerById(playerId);
		if (player != NULL)
		{
			player->getRoleData()->setIsBankup(isBankup);
		}
	}
	else if (funName == "getGunRate")	//得到炮倍
	{
		int playerId = data["playerId"].asInt();
		Player* player = PlayerManager::getInstance()->getPlayerById(playerId);
		if (player != NULL)
		{
			int type = data["type"].asInt();
			int maxGunRate = data["maxGunRate"].asInt();
			int currentGunRate = data["currentGunRate"].asInt();
			int minGunRate = PlayerManager::getInstance()->getCurRoomMinRate();

			PropData propData = player->getRoleData()->getGlod();
			int coin = propData.realCount - propData.flyingCount - propData.unSureCount;
			map = getGunRate(type, currentGunRate, minGunRate, maxGunRate, coin);
		}
		return map;
	}
	else if (funName == "changeGunRate")	//切换炮倍和最高炮倍
	{
		int playerId = data["playerId"].asInt();
		int currentGunRate = data["currentGunRate"].asInt();
		int maxGunRate = data["maxGunRate"].asInt();
		Player* player = PlayerManager::getInstance()->getPlayerById(playerId);
		if (player != NULL)
		{
			if (currentGunRate > 0)
			{
				printf("------------0-----------currentGunRate=%d\n", currentGunRate);
				player->getRoleData()->currentGunRate = currentGunRate;
			}
			if (maxGunRate > 0)
			{
				printf("------------1-----------maxCannonRate=%d\n", maxGunRate);
				player->getRoleData()->maxGunRate = maxGunRate;
			}
		}
	}
	
	else if (funName == "getAndSetPlayerData")	// 得到或设置玩家数据
	{
		map = PlayerManager::getInstance()->getAndSetPlayerData(data);
		return map;
	}
	else if (funName == "setGameState")	// 当前所在状态
	{
		PlayerManager::getInstance()->setGameState(data["gameState"].asInt());
	}
	else if (funName == "getLVByExp")	// 得到或设置玩家数据
	{
		int gradeExp = data["gradeExp"].asInt();
		map = getLVByExp(gradeExp);
		return map;
	}
	else if (funName == "getPlayerData")	// 得到或设置玩家数据
	{
		int playerId = data["playerId"].asInt();
		Player* player = PlayerManager::getInstance()->getPlayerById(playerId);
		if (player != NULL)
		{
			map = player->getRoleData()->getPlayerData();
		}

		return map;
	}
	else if (funName == "getTableByName")
	{
		map.insert(make_pair("result", Value(getValueTable(data))));
	}
	else if (funName == "playCppEffect")
	{
		int playType = data["playType"].asInt();
		float delayTime = data["delayTime"].asFloat();
		float posX = data["posX"].asFloat();
		float posY = data["posY"].asFloat();
		Node* parent = nullptr;
		CoinManager::getInstance()->playCppEffect(playType, delayTime, parent, Vec2(posX, posY));

	}
	else if (funName == "showGainCoinEffect")
	{
		int playerId = data["playerId"].asInt();
		int chairId = data["chairId"].asInt();
		int propId = data["propId"].asInt();
		int propCount = data["propCount"].asInt();
		int dropCount = data["dropCount"].asInt();
		float firstPosX = data["firstPosX"].asFloat();
		float firstPosY = data["firstPosY"].asFloat();
		float EndPosX = data["EndPosX"].asFloat();
		float EndPosY = data["EndPosY"].asFloat();
		bool isShowAddCount = data["isShowAddCount"].asBool();
		float allDelayTime = CoinManager::getInstance()->showGainCoinEffect(playerId, chairId, propCount, dropCount, Vec2(firstPosX, firstPosY), playerId == LuaCppAdapter::getInstance()->getPlayerId(), Vec2(EndPosX, EndPosY), isShowAddCount,0);
	}
	else if (funName == "setSocketPause")
	{
		bool isPause = data["isPause"].asBool();
		AppDelegate::setSocketPause(isPause);
	}
	else if (funName == "setGameType")
	{
		_gameType = data["gameType"].asInt();
		log("------------c------setBlastCoinEff------------_gameType=%d", _gameType);
		CoinManager::getInstance()->setGameType(data["gameType"].asInt());
	}
	else if (funName == "openLog")
	{
		AppDelegate::createFile();
	}
	else if (funName == "setLockData")
	{
		map = SkillLockManager::getInstance()->setLockData(data);
	}

	return map;
}

void LuaCppAdapter::setLuaNode(int type, Node *pManagerLayer, ValueMap data)
{
	if (type == 1)
	{
		printf("------------init lock layer------------\n");
		SkillLockManager::getInstance()->reset();
		SkillLockManager::getInstance()->setLockLayer((Layer*)pManagerLayer);
		
	}
	else if (type == 2)
	{
		SkillLockManager::getInstance()->setLuaNode(pManagerLayer, data);

	}
}


