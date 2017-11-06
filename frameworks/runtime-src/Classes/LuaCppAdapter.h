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

	//�����а�
	void openRankWeb(std::string url);

	//�ٻ���
	void callFish(ValueMap data);

	//��ȡ�˵���Χ�ﱻɱ������
	ValueVector getNBombKilledFishes(int bombLv, Vec2 pos);

	//��ȡplayerid
	int getPlayerId() { return _playerId;  }

	//��ȡ��ǰ֡��
	int getCurFrame();

	//ͬ��������Ϣ
	void syncFrame(ValueMap data);

	//���õ�ǰ��ײ�ӵ��ı���
	void setGunRate(int gunRate);

	//�㻻��ɫ
	void fishSwithColor(ValueMap data);

	//�㳱����
	void fishGroupCome(ValueMap data);

	//������ܳ�
	void fishAccelerateOut();

	//ʱ��������
	void fishTimeLineCome(ValueMap data);

	//�����ӵ�
	void playerFire(ValueMap data);

	//�ӵ��任Ŀ��
	void bulletChangeTarget(ValueMap data);

	//ɾ������ӵ�
	void removePlayerBullet(ValueMap data);

	//��������ӵ���ײ
	void otherPlayerBulletCollision(ValueMap data);

	//���µ�ǰ��ҵ������ֵ
	void updateThunderRate(int rate);

	//���µ�ǰ��ҵĺ˵���ֵ
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
	//ˢ������+shoot+hit
	void updateInline(ValueMap data);

	//�õ��ڱ�
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

	//�õ���ǰ�ȼ�
	ValueMap getLVByExp(int curExp);

	//�õ��齱��Ʒ����
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