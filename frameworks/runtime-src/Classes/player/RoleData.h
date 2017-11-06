#ifndef __ROLEDATA_H__
#define __ROLEDATA_H__
#include "cocos2d.h"
USING_NS_CC;

const int FRIEND_INDEX = 10000;
enum PropType
{
	GOLD = 1,
	GEM  = 2,
	PROP_FREEZE = 3,
	PROP_LOCK = 4,
	PROP_CALLFISH = 5,
	PROP_BOMB = 6,
	PROP_TIMEREVERT = 14,

	SENIOR_ROOMCARD = 1001,

	SCORE = FRIEND_INDEX + 100,
	USED_BULLET = FRIEND_INDEX + 101,
	BULLET = FRIEND_INDEX + 7,
};
struct PropData
{
	int propId;
	int realCount;//实际数值
	int flyingCount;//窗口掉落中的数值
	int unSureCount;//未确定的数值

	PropData(){
		propId = -1;
		realCount = 0;
		flyingCount = 0;
		unSureCount = 0;
	}
};

//高级道具
struct SeniorPropData
{
	int propItemId;		//唯一标识id
	int propId;			//同类id
	std::string stringProp;	//属性
	int intProp1;		//
	int intProp2;		//

	SeniorPropData(){
		propItemId = -1;
		propId = -1;
		stringProp = "";
		intProp1 = 0;
		intProp2 = 0;
	}
};

struct PropFlyingData
{
	int propId;
	int flyingCount;
	std::string stringProp;	//属性
	PropFlyingData() {
		propId = -1;
		flyingCount = 0;
		stringProp = "";
	}
};

struct PropFlyingRfreshData
{
	int playerId;
	int propId;
	int flyingCount;
	bool isSenior;
	int propItemId;
	ValueMap data;
	PropFlyingRfreshData()
	{
		playerId = -1;
		propId = -1;
		flyingCount = 0;
		isSenior = false;
		propItemId = 0;
		data = ValueMap();
	}
};
class RoleData : public Node
{
public:
	CREATE_FUNC(RoleData);
	bool init();

	Vec2 getCannonPos(){ return curPos; };
	//手动设置数据,不刷新界面
	void setPropRealData(int propId, int propCount);

	//自动接收数据，定时收
	void refreshPropData(int propId, int realCount, int flyingCount = -1 , float updateDelayTime = 0.0f);
	PropData &getPropData(int propId);
	PropData &getGlod() { return getPropData(GOLD); }
	PropData &getGem() { return getPropData(GEM); }

	void isSurePropData(bool isSure, int propId, int unSureCount, std::string stringProp = "");

	//手动接收数据
	void setAddFlyProp(bool isSure, int propId, int addCount);

	void setIsBankup(bool isBankup){ mIsBankup = isBankup; };
	bool getIsBankup(){ return mIsBankup; };

	ValueMap getAndSetPlayerData(ValueMap data);

	void clearNoRealData();

	void clearUnSureData();

	//得到玩家的数据
	ValueMap getPlayerData();


	//高级道具添加，删除,使用  //useType 1.直接添加 2.延时增加  3.直接删除,直接使用  4.延时删除 
								// 5.使用,假扣，加入缓存  6.删除缓存 7.使用,真扣，删除缓存
	void refreshSeniorPropData(int propId, ValueMap data, int useType, float updateDelayTime = 0.0f);

	//ValueMap和SeniorPropData的转换
	void ValueMapToSeniorPropData(ValueMap& data, SeniorPropData& dataProp,bool isLeftToRighr);

	//普通道具添加，删除,使用  //useType 1.直接添加 2.延时增加  3.直接删除,直接使用  4.延时删除 
	// 5.使用,假扣，加入缓存  6.删除缓存 7.使用,真扣，删除缓存
	void refreshNormalPropData(ValueMap data, int useType, float updateDelayTime = 0.0f);


	//得到高级道具列表
	std::map<int, std::map<int, SeniorPropData>> getSeniorPropData(){ return mSeniorPropDataMap; };
	std::map<int, std::map<int, SeniorPropData>> getSeniorPropCache(){ return mSeniorPropCacheMap; };
	//删除高级道具
	void eraseSeniorProp(int propId, int propItemId);
	void eraseSeniorPropCache(int propId, int propItemId);
	//增加高级道具
	void addSeniorProp(int propId, int propItemId, ValueMap data);
	void addSeniorPropCache(int propId, int propItemId, ValueMap data);

	ValueMap &getSeniorPropData(int propId, int propItemId);

	int getSeniorPropShowCount(int propId);

	void clearAllSeniorProp(int clearType);

	void clearTrueSeniorProp();
	void clearSeniorPropCache();

//角色数据
public:
	bool mIsBankup;
	int playerId;
	int currentGunRate;
	int maxGunRate;

	int vipExp;
	int vip_level;
	//...etc

	Vec2 curPos;

//道具数据
private:
	std::map<int, PropData> mPropDataMap;			//总的数据，包含真实数据和飞行数据
	std::vector <PropFlyingData> mPropFlyingVector;
	std::vector <PropFlyingData> mUnSureCountVector;

	std::map<int, std::map<int, SeniorPropData>> mSeniorPropDataMap;	//高级道具
	std::map<int, std::map<int, SeniorPropData>> mSeniorPropCacheMap;	//高级道具缓存池
};


#endif // !__ROLEDATA_H__
