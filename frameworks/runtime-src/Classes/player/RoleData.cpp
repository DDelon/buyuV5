#include "RoleData.h"
#include "LuaCppAdapter.h"
#include "PlayerManager.h"
#include "StringUtil.h"

void RoleData::setPropRealData(int propId, int propCount)
{
	getPropData(propId).realCount = propCount;
}

void RoleData::refreshPropData(int propId, int realCount, int flyingCount, float updateDelayTime /*= 0.0f*/)
{
	setPropRealData(propId, realCount);
	//getPropData(propId).realCount = realCount;
	if (flyingCount > 0 && updateDelayTime > 0 )
	{
		PropFlyingData flyingData;
		flyingData.propId = propId;
		flyingData.flyingCount = flyingCount;
		mPropFlyingVector.push_back(flyingData);
		getPropData(propId).flyingCount += flyingCount;

		this->runAction(Sequence::createWithTwoActions(
			DelayTime::create(updateDelayTime),
			CallFunc::create([=](){
					std::vector <PropFlyingData>::iterator it;
					for (it = mPropFlyingVector.begin(); it != mPropFlyingVector.end(); it++)
					{
						if (it->propId == propId && it->flyingCount == flyingCount)
						{
							break;
						}
					}

					if (it != mPropFlyingVector.end())
					{
						getPropData(propId).flyingCount -= it->flyingCount;
						if (getPropData(propId).flyingCount < 0 )
						{
							getPropData(propId).flyingCount = 0;
						}
						//CCLOG("================prop id:%d,flyingCount:%d,realCount:%d", getPropData(propId).propId, it->flyingCount, getPropData(propId).realCount);
						PropFlyingRfreshData refreshData;
						refreshData.playerId = playerId;
						refreshData.propId = it->propId;
						refreshData.flyingCount = it->flyingCount;
						_eventDispatcher->dispatchCustomEvent("refreshUiByEvent", &refreshData);
						mPropFlyingVector.erase(it);
					}

		
				})
			));
	}
	else
	{
		PropFlyingRfreshData refreshData;
		refreshData.playerId = playerId;
		refreshData.propId = propId;
		refreshData.flyingCount = flyingCount;
		_eventDispatcher->dispatchCustomEvent("refreshUiByEvent", &refreshData);
	}
	
}

PropData & RoleData::getPropData(int propId)
{
	auto iter = mPropDataMap.find(propId);
	if (iter != mPropDataMap.end())
	{
		return iter->second;
	}
	else
	{
		PropData data;
		data.propId = propId;
		mPropDataMap[propId] = data;

		return mPropDataMap[propId];
	}
}

bool RoleData::init()
{
	Node::init();

	mIsBankup = false;
	playerId = -1;
	currentGunRate = -1;
	maxGunRate = -1;
	vipExp = 0;
	vip_level = 0;
	curPos = Vec2(-1, -1);

	mPropDataMap.clear();
	mPropFlyingVector.clear();
	mUnSureCountVector.clear();
	return true;
}

void RoleData::setAddFlyProp(bool isSure, int propId, int addCount)
{
	int flyingCount = 0;
	if (isSure == false)
	{
		//塞入飞行中的数据
		PropFlyingData flyingData;
		flyingData.propId = propId;
		flyingData.flyingCount = addCount;
		mPropFlyingVector.push_back(flyingData);
		getPropData(propId).flyingCount += addCount;
	}
	else
	{
		//删除以收取的数据
		std::vector <PropFlyingData>::iterator it;
		for (it = mPropFlyingVector.begin(); it != mPropFlyingVector.end(); it++)
		{
			if (it->propId == propId && it->flyingCount == addCount)
			{
				break;
			}
		}

		if (it != mPropFlyingVector.end())
		{
			flyingCount = it->flyingCount;
			getPropData(propId).flyingCount -= it->flyingCount;
			if (getPropData(propId).flyingCount < 0)
			{
				getPropData(propId).flyingCount = 0;
			}
			//CCLOG("================prop id:%d,flyingCount:%d,realCount:%d", getPropData(propId).propId, it->flyingCount, getPropData(propId).realCount);
			mPropFlyingVector.erase(it);
		}
	}

	PropFlyingRfreshData refreshData;
	refreshData.playerId = playerId;
	refreshData.propId = propId;
	refreshData.flyingCount = flyingCount;
	_eventDispatcher->dispatchCustomEvent("refreshUiByEvent", &refreshData);
}

void RoleData::isSurePropData(bool isSure, int propId, int unSureCount, string stringProp)
{

	if (isSure == false)
	{
		//塞入未确定的数据
		PropFlyingData unSureData;
		unSureData.propId = propId;
		unSureData.flyingCount = unSureCount;
		unSureData.stringProp = stringProp;
		mUnSureCountVector.push_back(unSureData);
		getPropData(propId).unSureCount += unSureCount;
	}
	else
	{
		//删除未确定的数据
		std::vector <PropFlyingData>::iterator it;
		for (it = mUnSureCountVector.begin(); it != mUnSureCountVector.end(); it++)
		{
			if (stringProp == "")
			{
				if (it->propId == propId && it->flyingCount == unSureCount)
				{
					break;
				}
			}
			else
			{
				if ( it->propId == propId && it->stringProp == stringProp)
				{
					break;
				}
			}

		}

		if (it != mUnSureCountVector.end())
		{
			getPropData(propId).unSureCount -= it->flyingCount;
			if (getPropData(propId).unSureCount < 0)
			{
				getPropData(propId).unSureCount = 0;
			}
			//CCLOG("================prop id:%d,flyingCount:%d,realCount:%d", getPropData(propId).propId, it->flyingCount, getPropData(propId).realCount);
			mUnSureCountVector.erase(it);
		}
	}

	PropFlyingRfreshData refreshData;
	refreshData.playerId = playerId;
	refreshData.propId = propId;
	refreshData.flyingCount = 0;
	_eventDispatcher->dispatchCustomEvent("refreshUiByEvent", &refreshData);
}

cocos2d::ValueMap RoleData::getAndSetPlayerData(ValueMap data)
{
	int playerId = data["playerId"].asInt();
	bool isGet = data["isGet"].asBool();
	string dataName = data["dataName"].asString();
	int val = data["val"].asInt();
	ValueMap map;
	if (dataName == "vipExp")
	{
		if (isGet && val >= 0)
		{
			vipExp = val;
			ValueMap map2;
			map2["funName"] = "getVIPByCostMoney";
			map2["costMoney"] = val;
			map = LuaCppAdapter::getInstance()->luaUseCppFun(map2);
			vip_level = map["vip_level"].asInt();
		}
		map["vip_level"] = vip_level;
		map["vipExp"] = vipExp;
	}
	else if (dataName == "maxGunRate")
	{
		map["maxGunRate"] = maxGunRate;
	}
	else if (dataName == "currentGunRate")
	{
		map["currentGunRate"] = currentGunRate;
	}
	return map;
}

void RoleData::clearNoRealData()
{
	map<int, PropData>::iterator iter;
	for (iter = mPropDataMap.begin(); iter != mPropDataMap.end(); iter++)
	{
		iter->second.flyingCount = 0;
		iter->second.unSureCount = 0;
	}
	mSeniorPropCacheMap.clear();
	mPropFlyingVector.clear();
	mUnSureCountVector.clear();
}

void RoleData::clearUnSureData()
{
	map<int, PropData>::iterator iter;
	for (iter = mPropDataMap.begin(); iter != mPropDataMap.end(); iter++)
	{
		if (iter->second.unSureCount != 0 )
		{
			iter->second.unSureCount = 0;
			PropFlyingRfreshData refreshData;
			refreshData.playerId = playerId;
			refreshData.propId = iter->second.propId;
			refreshData.flyingCount = 0;
			_eventDispatcher->dispatchCustomEvent("refreshUiByEvent", &refreshData);
		}
	}
	mUnSureCountVector.clear();
}

cocos2d::ValueMap RoleData::getPlayerData()
{
	ValueMap resultMap;
	ValueMap PropMap;
	map<int, PropData>::iterator iter;
	for (iter = mPropDataMap.begin(); iter != mPropDataMap.end(); iter++)
	{
		int propId = iter->first;
		int propCount = iter->second.realCount;
		PropMap[IntToString(propId)] = propCount;
	}

	resultMap["props"] = PropMap;
	resultMap["mIsBankup"] = mIsBankup;
	resultMap["playerId"] = playerId;
	resultMap["currentGunRate"] = currentGunRate;
	resultMap["maxGunRate"] = maxGunRate;
	resultMap["vipExp"] = vipExp;
	resultMap["vip_level"] = vip_level;
	resultMap["curPosX"] = curPos.x;
	resultMap["curPosY"] = curPos.y;

	//高级道具
	ValueMap seniorProps;
	std::map<int, std::map<int, SeniorPropData>>::iterator allSeniorPropIter;
	for (allSeniorPropIter = mSeniorPropDataMap.begin(); allSeniorPropIter != mSeniorPropDataMap.end(); allSeniorPropIter++)
	{
		int propId = allSeniorPropIter->first;
		ValueMap SeniorPropMap;
		std::map < int, SeniorPropData > SeniorPropDataMap = allSeniorPropIter->second;
		std::map < int, SeniorPropData >::iterator seniorPropIter;
		for (seniorPropIter = SeniorPropDataMap.begin(); seniorPropIter != SeniorPropDataMap.end(); seniorPropIter++)
		{
			int propItemId = seniorPropIter->first;
			SeniorPropData itemData = seniorPropIter->second;
			ValueMap itemValeData;
			ValueMapToSeniorPropData(itemValeData, itemData, false);
			SeniorPropMap[IntToString(propItemId)] = itemValeData;
		}
		seniorProps[IntToString(propId)] = SeniorPropMap;
	}
	resultMap["seniorProps"] = seniorProps;

	return resultMap;
}

void RoleData::refreshSeniorPropData(int propId, ValueMap data, int useType, float updateDelayTime /*= 0.0f*/)
{
	int propItemId = data["propItemId"].asInt();
	bool isDel = false;
	if (useType == 1)		//1.直接添加
	{
		addSeniorProp(propId, propItemId, data);
	}
	else if (useType == 2)//2.延时增加
	{
		addSeniorProp(propId, propItemId, data);
		addSeniorPropCache(propId, propItemId, data);

		this->runAction(Sequence::createWithTwoActions(
			DelayTime::create(updateDelayTime),
			CallFunc::create([=](){

			eraseSeniorPropCache(propId, propItemId);

			PropFlyingRfreshData refreshData;
			refreshData.playerId = playerId;
			refreshData.propId = propId;
			refreshData.flyingCount = 0;
			refreshData.isSenior = true;
			refreshData.propItemId = propItemId;
			refreshData.data = data;
			//refreshData.data["isDel"] = true;
			_eventDispatcher->dispatchCustomEvent("refreshUiByEvent", &refreshData);

		})
			));
	}
	else if (useType == 3)//3.直接删除,直接使用
	{
		eraseSeniorProp(propId, propItemId);
		eraseSeniorPropCache(propId, propItemId);
		isDel = true;
	}
	else if (useType == 4)//4.延时删除
	{
		eraseSeniorProp(propId, propItemId);
		addSeniorPropCache(propId, propItemId, data);

		this->runAction(Sequence::createWithTwoActions(
			DelayTime::create(updateDelayTime),
			CallFunc::create([=](){

			eraseSeniorPropCache(propId, propItemId);
			
			PropFlyingRfreshData refreshData;
			refreshData.playerId = playerId;
			refreshData.propId = propId;
			refreshData.flyingCount = 0;
			refreshData.isSenior = true;
			refreshData.propItemId = propItemId;
			refreshData.data = data;
			refreshData.data["isDel"] = true;
			_eventDispatcher->dispatchCustomEvent("refreshUiByEvent", &refreshData);

		})
			));
	}
	else if (useType == 5)//5.使用,假扣，加入缓存
	{
		addSeniorPropCache(propId, propItemId, data);
		isDel = true;
	}
	else if (useType == 6)//6.删除缓存
	{
		eraseSeniorPropCache(propId, propItemId);
	}
	else if (useType == 7)//7.真扣并删除缓存
	{
		eraseSeniorProp(propId, propItemId);
		eraseSeniorPropCache(propId, propItemId);
		isDel = true;
	}
	else if (useType == 8)//8.收到道具，并加入缓存，等待lua清除缓存
	{
		addSeniorProp(propId, propItemId, data);
		addSeniorPropCache(propId, propItemId, data);
	}

	PropFlyingRfreshData refreshData;
	refreshData.playerId = playerId;
	refreshData.propId = propId;
	refreshData.flyingCount = 0;
	refreshData.isSenior = true;
	refreshData.propItemId = propItemId;
	refreshData.data = data;
	refreshData.data["isDel"] = isDel;
	_eventDispatcher->dispatchCustomEvent("refreshUiByEvent", &refreshData);

}

void RoleData::ValueMapToSeniorPropData(ValueMap& data, SeniorPropData& dataProp, bool isLeftToRighr)
{
	if (isLeftToRighr)
	{
		dataProp.propItemId = data["propItemId"].asInt();
		dataProp.propId = data["propId"].asInt();
		dataProp.stringProp = data["stringProp"].asString();
		dataProp.intProp1 = data["intProp1"].asInt();
		dataProp.intProp2 = data["intProp2"].asInt();
	}
	else
	{
		data["propItemId"] = dataProp.propItemId;
		data["propId"] = dataProp.propId;
		data["stringProp"] = dataProp.stringProp;
		data["intProp1"] = dataProp.intProp1;
		data["intProp2"] = dataProp.intProp2;
	}
}

void RoleData::eraseSeniorProp(int propId, int propItemId)
{
	auto propIdMapIter = mSeniorPropDataMap.find(propId);
	if (propIdMapIter != mSeniorPropDataMap.end())
	{
		auto propItemMapIter = mSeniorPropDataMap[propId].find(propItemId);
		if (propItemMapIter != mSeniorPropDataMap[propId].end())
		{
			mSeniorPropDataMap[propId].erase(propItemMapIter);
		}
	}
}

void RoleData::eraseSeniorPropCache(int propId, int propItemId)
{
	auto propIdCacheMapIter = mSeniorPropCacheMap.find(propId);
	if (propIdCacheMapIter != mSeniorPropCacheMap.end())
	{
		auto propItemCacheMapIter = mSeniorPropCacheMap[propId].find(propItemId);
		if (propItemCacheMapIter != mSeniorPropCacheMap[propId].end())
		{
			mSeniorPropCacheMap[propId].erase(propItemCacheMapIter);
		}
	}
}

void RoleData::addSeniorProp(int propId, int propItemId , ValueMap data)
{
	SeniorPropData newPropItem;
	ValueMapToSeniorPropData(data, newPropItem, true);
	auto propIdMapIter = mSeniorPropDataMap.find(propId);
	if (propIdMapIter == mSeniorPropDataMap.end())
	{
		std::map<int, SeniorPropData> newPropMap;
		mSeniorPropDataMap[propId] = newPropMap;
	}
	mSeniorPropDataMap[propId][propItemId] = newPropItem;
}

void RoleData::addSeniorPropCache(int propId, int propItemId, ValueMap data)
{
	SeniorPropData newPropItem;
	ValueMapToSeniorPropData(data, newPropItem, true);
	auto propIdMapIter = mSeniorPropCacheMap.find(propId);
	if (propIdMapIter == mSeniorPropCacheMap.end())
	{
		std::map<int, SeniorPropData> newPropMap;
		mSeniorPropCacheMap[propId] = newPropMap;
	}
	mSeniorPropCacheMap[propId][propItemId] = newPropItem;
}

ValueMap & RoleData::getSeniorPropData(int propId, int propItemId)
{
	ValueMap result;
	auto iter = mSeniorPropDataMap.find(propId);
	if (iter == mSeniorPropDataMap.end())
	{
		return result;
	}

	auto Itemiter = mSeniorPropDataMap[propId].find(propItemId);
	if (Itemiter == mSeniorPropDataMap[propId].end())
	{
		return result;
	}

	ValueMapToSeniorPropData(result, mSeniorPropDataMap[propId][propItemId], false);
	return result;
}

int RoleData::getSeniorPropShowCount(int propId)
{
	int realCount = 0;
	auto iter = mSeniorPropDataMap.find(propId);
	if (iter == mSeniorPropDataMap.end())
	{
		return realCount;
	}

	std::map<int, SeniorPropData>::iterator propIter;
	for (propIter = mSeniorPropDataMap[propId].begin(); propIter != mSeniorPropDataMap[propId].end(); propIter++)
	{
		realCount = realCount + 1;
	}

	int cacheCount = 0;
	auto cacheiter = mSeniorPropCacheMap.find(propId);
	if (cacheiter == mSeniorPropCacheMap.end())
	{
		return realCount;
	}

	std::map<int, SeniorPropData>::iterator CachepropIter;
	for (CachepropIter = mSeniorPropCacheMap[propId].begin(); CachepropIter != mSeniorPropCacheMap[propId].end(); CachepropIter++)
	{
		cacheCount = cacheCount + 1;
	}
	return realCount - cacheCount;
}

void RoleData::clearAllSeniorProp(int clearType)
{
	if (clearType == 1)
	{
		clearTrueSeniorProp();
	}
	else if (clearType == 2)
	{
		clearSeniorPropCache();
	}
	else
	{
		clearTrueSeniorProp();
		clearSeniorPropCache();
	}
}

void RoleData::clearTrueSeniorProp()
{
	std::map<int, std::map<int, SeniorPropData>>::iterator allSeniorPropIter;
	for (allSeniorPropIter = mSeniorPropDataMap.begin(); allSeniorPropIter != mSeniorPropDataMap.end(); allSeniorPropIter++)
	{
		int propId = allSeniorPropIter->first;
		allSeniorPropIter->second.clear();
		PropFlyingRfreshData refreshData;
		refreshData.playerId = playerId;
		refreshData.propId = propId;
		refreshData.propItemId = 0;
		refreshData.flyingCount = 0;
		refreshData.isSenior = true;
		_eventDispatcher->dispatchCustomEvent("refreshUiByEvent", &refreshData);
	}
	mSeniorPropDataMap.clear();
}

void RoleData::clearSeniorPropCache()
{
	ValueMap seniorProps;
	std::map<int, std::map<int, SeniorPropData>>::iterator allSeniorPropIter;
	for (allSeniorPropIter = mSeniorPropCacheMap.begin(); allSeniorPropIter != mSeniorPropCacheMap.end(); allSeniorPropIter++)
	{
		int propId = allSeniorPropIter->first;
		allSeniorPropIter->second.clear();
		PropFlyingRfreshData refreshData;
		refreshData.playerId = playerId;
		refreshData.propId = propId;
		refreshData.propItemId = 0;
		refreshData.flyingCount = 0;
		refreshData.isSenior = true;
		_eventDispatcher->dispatchCustomEvent("refreshUiByEvent", &refreshData);
	}
	mSeniorPropCacheMap.clear();
}

//刷新普通道具
void RoleData::refreshNormalPropData(ValueMap data, int useType, float updateDelayTime /*= 0.0f*/)
{
	int propId = data["propId"].asInt();
	int propCount = data["propCount"].asInt();
	if (useType == 1)		//直接添加或者直接删除
	{
		int curCount = getPropData(propId).realCount;
		int newPropCount = curCount + propCount;
		refreshPropData(propId, newPropCount, 0, updateDelayTime);
	}
	else if (useType == 2)//使用,假扣，加入缓存
	{
		isSurePropData(false, propId, propCount);
	}

	else if (useType == 3)//删除缓存
	{
		isSurePropData(true, propId, propCount);
	}
	else if (useType == 4)//真扣并删除缓存
	{
		int curCount = getPropData(propId).realCount;
		int newPropCount = curCount - propCount;
		setPropRealData(propId, newPropCount);
		isSurePropData(true, propId, propCount);
	}
	else if (useType == 5)//收到道具增加真实数值，并加入缓存，等待lua清除缓存
	{
		int curCount = getPropData(propId).realCount;
		int newPropCount = curCount + propCount;
		setPropRealData(propId, newPropCount);
		setAddFlyProp(false, propId, propCount);
	}
	else if (useType == 6)//收到道具，删除缓存
	{
		setAddFlyProp(true, propId, propCount);
	}
	else if (useType == 7)//刷新道具真实数量，不显示
	{
		setPropRealData(propId, propCount);
	}
	else if (useType == 8)//刷新道具真实数量（updateDelayTime为延时显示时间）
	{
		int flyingCount = 0;
		if (updateDelayTime > 0)
		{
			int curCount = getPropData(propId).realCount;
			flyingCount = propCount - curCount;
		}
		refreshPropData(propId, propCount, flyingCount, updateDelayTime);
	}
}