#ifndef __LUA_CPP_ADAPTER_H__
#define __LUA_CPP_ADAPTER_H__
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "ParseGameData.h"

USING_NS_CC;
using namespace std;
class GameScene;

class LuaCppAdapter
{
public:
	static LuaCppAdapter *getInstance();

protected:
	LuaCppAdapter();
	~LuaCppAdapter();
public:
	//cpp call lua
	
public:
	//lua call cpp
	std::string getConfigData(std::string configName, std::string keyID, std::string keyName);
	ValueMap getConfigDataByName(std::string keyName);

	void addDebugLayer(Layer *pLayer, std::string roomId = "");

	void frameRunning(int frame);

	void changeTimeline(int timelineIndex);

	void startGame(Layer *pManagerLayer, Layer *pFishLayer, ValueMap data, ValueVector killedFishes, ValueVector calledFishes);

	void exitGame();

	void startBullet(Layer *pBulletLayer, ValueMap data);

	void startNets(Layer *pNetsLayer);

	void setCannon(Node *pCannon, int playerId);

	void debugLog(std::string log);

	Vec3 getPathPos(int pathId, int frame);

	//打开排行榜
	void openRankWeb(std::string url);

	//召唤鱼
	void callFish(ValueMap data);

	//获取核弹范围里被杀死的鱼
	ValueVector getNBombKilledFishes(int bombLv, Vec2 pos);

	//获取playerid
	int getPlayerId() { return _playerId;  }

	//获取当前帧号
	int getCurFrame();

	//同步心跳消息
	void syncFrame(ValueMap data);

	//设置当前碰撞子弹的倍率
	void setGunRate(int gunRate);

	//鱼换颜色
	void fishSwithColor(ValueMap data);

	//鱼潮来临
	void fishGroupCome(ValueMap data);

	//鱼加速跑出
	void fishAccelerateOut();

	//时间线来临
	void fishTimeLineCome(ValueMap data);

	//发射子弹
	void playerFire(ValueMap data);

	//子弹变换目标
	void bulletChangeTarget(ValueMap data);

	//删除玩家子弹
	void removePlayerBullet(ValueMap data);

	//其他玩家子弹碰撞
	void otherPlayerBulletCollision(ValueMap data);

	//更新当前玩家的闪电库值
	void updateThunderRate(int rate);

	//更新当前玩家的核弹库值
	void updateNBombRate(int rate);
	
	void setFishState(ValueMap data);

	ValueMap luaUseCppFun(ValueMap data);

	void callLuaFunc(std::string globalTabName, std::string funcName, std::string type, ValueMap val);

	void getLuaValue(std::string globalTabName, std::string valName, Value &val);

	std::string getVIPResId(int playerId);


	void addPlayer(ValueMap map);

	void bindUI(ValueMap map, ui::TextBMFont* text, ui::TextBMFont* addText);

	void loadDataBin();

	int _gameType;

	void setLuaNode(int type,Node *pManagerLayer, ValueMap data);


private:
	//刷新数据+shoot+hit
	void updateInline(ValueMap data);

	//得到炮倍
	ValueMap getGunRate(int type, int curRate, int minGunRate, int maxGunRate, float coin);
	ValueMap getRoomFish(int curRoomID);
	ValueMap getShopData();
	ValueMap getItemData(int itemID);
	ValueMap getAllItemData();
	ValueMap getFieldMapData(FieldMap data);
	int getGunInterval(int curRate);
	ValueMap getForgedData(int id);
	ValueMap getForgedData(int id, int endId);
	ValueMap getVIPByCostMoney(int money_need);
	ValueMap getVIPDataByKeyID(int keyID);
	ValueMap getVIPData();

	ValueMap myCreateBullet(ValueMap data);
	ValueMap myCreateFriendBullet(ValueMap data);

	//得到当前等级
	ValueMap getLVByExp(int curExp);

	//得到抽奖奖品数据
	ValueMap getRewardData();

	void pushValueMap(lua_State *pState, ValueMap val);

	void pushInt(lua_State *pState, int val);

	void pushDouble(lua_State *pState, int val);

	void pushFloat(lua_State *pState, float val);

	void pushBoolean(lua_State *pState, bool val);

	void pushString(lua_State *pState, const char* val);

	void pushVector(lua_State *pState, ValueVector val);

	void pushValue(lua_State *pState, Value val);

	ValueMapIntKey getValueTable(ValueMap& data);
	

	

private:
	static LuaCppAdapter *_pInstance;

	GameScene *_pGameScene;
	lua_State *_pLuaState;
	LabelTTF *_pFrameLabel;
	LabelTTF *_pTimelineLabel;
	LabelTTF *_pRoomIdLabel;

	int _playerId;
	

	std::map<int, std::string> _vipResMap;

};

#endif