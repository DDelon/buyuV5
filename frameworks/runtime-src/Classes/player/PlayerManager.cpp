#include "PlayerManager.h"
#include "StringUtil.h"
#include "CoinManager.h"
#include "ParseGameData.h"

static PlayerManager* sInstance = nullptr;

PlayerManager* PlayerManager::getInstance()
{
	if (sInstance == nullptr)
	{
		sInstance = new(std::nothrow) PlayerManager();
		if (sInstance && sInstance->init())
		{
			sInstance->retain();
		}
	}
	return sInstance;
}

bool PlayerManager::init()
{
	Node::init();
	mPlayerVec.clear();
	mCurGameRoomID = -1;
	mMyPlayerId = -1;
	mGameState = 0;

	Director::getInstance()->getEventDispatcher()->addCustomEventListener("refreshUiByEvent", std::bind(&PlayerManager::refreshUiByEvent, this, std::placeholders::_1));
	
	
	return true;
}

void PlayerManager::reset()
{
	mPlayerVec.clear();
	this->removeAllChildren();
}

void PlayerManager::setCurGameRoomID(int curGameRoomID)
{
	mCurGameRoomID = curGameRoomID;
	mCurRoomMinRate = GAMEVALUE_INT("room", IntToString(curGameRoomID + 910000000), "cannon_min");
	printf("----------------setCurGameRoomID----mCurRoomMinRate=%d\n", mCurRoomMinRate);
}

void PlayerManager::addPlayer(ValueMap map)
{
	Player* player = Player::create();
	player->getRoleData()->playerId = map["playerId"].asInt();
	addPlayer(player);
	player->getRoleData()->currentGunRate = map["currentGunRate"].asInt();
	player->getRoleData()->maxGunRate = map["maxGunRate"].asInt();
	player->getRoleData()->refreshPropData(GOLD, map["gold"].asInt());
	player->getRoleData()->refreshPropData(GEM, map["gem"].asInt());

	player->getRoleData()->curPos = Vec2(map["posX"].asFloat(), map["posY"].asFloat());

	auto iter = map.find("props");
	if (iter != map.end())//有道具
	{
		auto props = map["props"].asValueVector();
		for (std::vector<Value>::iterator it = props.begin(); it != props.end(); it++)
		{
			ValueMap mapProp = (*it).asValueMap();
			int propId = mapProp["propId"].asInt();
			int propCount = mapProp["propCount"].asInt();
			player->getRoleData()->refreshPropData(propId, propCount, 0, 0);
		}
	}

	auto senioriter = map.find("seniorProps");
	if (senioriter != map.end())//有高级道具
	{
		auto props = map["seniorProps"].asValueVector();
		log("----------------------------------------- senioriter.size=%d--------", props.size());
		for (std::vector<Value>::iterator it = props.begin(); it != props.end(); it++)
		{
			
			ValueMap mapProp = (*it).asValueMap();
			int propId = mapProp["propId"].asInt();
			log("----------------------------------------- mapseniorProps--------propId=%d",propId);
			player->getRoleData()->refreshSeniorPropData(propId, mapProp,1, 0);
		}
	}
}

void PlayerManager::addFriendPlayer(ValueMap map)
{
	Player* player = Player::create();
	player->getRoleData()->playerId = map["playerId"].asInt();
	addPlayer(player);
	player->getRoleData()->currentGunRate = map["currentGunRate"].asInt();
	player->getRoleData()->maxGunRate = map["maxGunRate"].asInt();

	player->getRoleData()->refreshPropData(SCORE, map["score"].asInt());
	player->getRoleData()->refreshPropData(USED_BULLET, map["bulletUsed"].asInt());
	player->getRoleData()->refreshPropData(GEM, map["crystal"].asInt());

	player->getRoleData()->curPos = Vec2(map["posX"].asFloat(), map["posY"].asFloat());

	auto iter = map.find("friendProps");
	if (iter != map.end())//有道具
	{
		auto props = map["friendProps"].asValueVector();
		for (std::vector<Value>::iterator it = props.begin(); it != props.end(); it++)
		{
			ValueMap mapProp = (*it).asValueMap();
			int propId = mapProp["propId"].asInt();
			int propCount = mapProp["propCount"].asInt();
			player->getRoleData()->refreshPropData(propId, propCount, 0, 0);
		}
	}


}

void PlayerManager::addPlayer(Player* player)
{
	int playerId = player->getRoleData()->playerId;
	delPlayer(playerId);
	mPlayerVec.push_back(player);
	this->addChild(player);
}

bool PlayerManager::delPlayer(int playerId)
{
	bool isDel = false;
	for (std::vector<Player*>::iterator it = mPlayerVec.begin(); it != mPlayerVec.end();it++)
	{
		if ((*it)->getRoleData()->playerId == playerId)
		{
			CCLOG("delPlayer:%d",playerId);
			(*it)->setMoneyUI(GEM,NULL,NULL);
			(*it)->setMoneyUI(GOLD, NULL, NULL);
			(*it)->removeFromParent();
			mPlayerVec.erase(it);
			isDel = true;
			break;
		}
	}

	return isDel;
}

Player* PlayerManager::getPlayerById(int playerId)
{
	Player* player = NULL;
	for (std::vector<Player*>::iterator it = mPlayerVec.begin(); it != mPlayerVec.end(); it++)
	{
		if ((*it)->getRoleData()->playerId == playerId)
		{
			player = *it;
			break;
		}
	}

	return player;
}

//手动设置数据,不刷新界面
void PlayerManager::setPropRealData(int playerId, int propId, int propCount)
{
	Player* player = getPlayerById(playerId);
	if (player != NULL)
	{
		if (propCount < 0)
		{
			propCount = 0;
		}
		player->getRoleData()->setPropRealData(propId, propCount);
	}
}

void PlayerManager::refreshPropData(int playerId, int propId, int realCount, int flyingCount, float updateDelayTime)
{
	Player* player = getPlayerById(playerId);

	if (player!=NULL)
	{
		if (flyingCount < 0)
		{
			flyingCount = 0;
		}
		player->getRoleData()->refreshPropData(propId, realCount, flyingCount, updateDelayTime);
	}
}

void PlayerManager::isSurePropData(ValueMap data)
{
	int playerId = data["playerId"].asInt();
	bool isSure = data["isSure"].asBool();
	int propId = data["propId"].asInt();
	int propCount = data["propCount"].asInt();

	Player* player = getPlayerById(playerId);

	if (player != NULL)
	{
		player->getRoleData()->isSurePropData(isSure, propId, propCount);
	}
}

void PlayerManager::setAddFlyProp(ValueMap data)
{
	int playerId = data["playerId"].asInt();
	bool isSure = data["isSure"].asBool();
	int propId = data["propId"].asInt();
	int propCount = data["propCount"].asInt();

	Player* player = getPlayerById(playerId);
	if (player != NULL)
	{
		player->getRoleData()->setAddFlyProp(isSure, propId, propCount);
	}
}

void PlayerManager::addTrueAndFlyProp(ValueMap data)
{
	int playerId = data["playerId"].asInt();
	Player* player = getPlayerById(playerId);
	if (player == NULL)
	{
		return;
	}

	int propId = data["propId"].asInt();
	int curCount = player->getRoleData()->getPropData(propId).realCount;

	int addpropCount = data["propCount"].asInt();
	int propCount = addpropCount + curCount;

	bool isShow = data["isShow"].asBool();
	int flyingCount = data["flyingCount"].asInt();
	float delayTime = data["delayTime"].asFloat();

	if (isShow)
	{
		refreshPropData(playerId, propId, propCount, flyingCount, delayTime);
	}
	else
	{
		setPropRealData(playerId, propId, propCount);
	}
}

void PlayerManager::refreshUiByEvent(EventCustom* evnet)
{
	//CCLOG("----------refreshUI");
	PropFlyingRfreshData* data = (PropFlyingRfreshData*)(evnet->getUserData());
	refreshUI(data);
}

void PlayerManager::refreshUI(PropFlyingRfreshData* data)
{
	for (std::vector<Player*>::iterator it = mPlayerVec.begin(); it != mPlayerVec.end(); it++)
	{
		Player* player = *it;
		if (data && player->getRoleData()->playerId == data->playerId)
		{
			if (player)
			{
				if (mGameState == 2)
				{
					//在大厅直接回调lua
					int showCount = 0;
					int propItemId = 0;
					if (data->isSenior)
					{
						propItemId = data->propItemId;
						showCount = player->getRoleData()->getSeniorPropShowCount(data->propId);
					}
					else
					{
						showCount = player->getRoleData()->getPropData(data->propId).realCount - player->getRoleData()->getPropData(data->propId).flyingCount - player->getRoleData()->getPropData(data->propId).unSureCount;
					}
					player->refreshPropShow(data->propId, showCount, data->flyingCount, propItemId, data->data);
					return;
				}
				switch (data->propId)
				{
				case GOLD:
					player->refreshGoldShow(player->getRoleData()->getGlod().realCount - player->getRoleData()->getGlod().flyingCount - player->getRoleData()->getGlod().unSureCount, data->flyingCount);
					break;
				case GEM:
					player->refreshGemShow(player->getRoleData()->getGem().realCount - player->getRoleData()->getGem().flyingCount - player->getRoleData()->getGem().unSureCount, data->flyingCount);
					break;
				case SCORE:
					player->refreshScoreShow(player->getRoleData()->getPropData(data->propId).realCount - player->getRoleData()->getPropData(data->propId).flyingCount - player->getRoleData()->getPropData(data->propId).unSureCount, data->flyingCount);
					break;
				case BULLET:
					player->refreshBulletShow(player->getRoleData()->getPropData(data->propId).realCount - player->getRoleData()->getPropData(data->propId).flyingCount - player->getRoleData()->getPropData(data->propId).unSureCount, data->flyingCount);
					break;
				default:
					int showCount = 0;
					int propItemId = 0;
					if (data->isSenior)
					{
						propItemId = data->propItemId;
						showCount = player->getRoleData()->getSeniorPropShowCount(data->propId);
					}
					else
					{
						showCount = player->getRoleData()->getPropData(data->propId).realCount - player->getRoleData()->getPropData(data->propId).flyingCount - player->getRoleData()->getPropData(data->propId).unSureCount;
					}
					player->refreshPropShow(data->propId, showCount, data->flyingCount, propItemId, data->data);
					break;
				}

			}
		}
	}
}

int PlayerManager::coinJumpAct(ValueMap data)
{
	int playerId = data["playerId"].asInt();
	int newFishIcon = data["newFishIcon"].asInt();

	Player* player = PlayerManager::getInstance()->getPlayerById(playerId);
	if (player == nullptr)
	{
		return 0 ;
	}
	PropData propData = player->getRoleData()->getPropData(GOLD);
	int moneyCount = newFishIcon - propData.realCount;
	
	if (playerId == mMyPlayerId)
	{
		int chairId = data["chairId"].asInt();
		int coinNum = data["coinNum"].asInt();
		float allDelayTime = CoinManager::getInstance()->showGunUpEffect(playerId, chairId, 0, coinNum);
		PlayerManager::getInstance()->refreshPropData(playerId, GOLD, newFishIcon, moneyCount, allDelayTime);
	}
	else
	{
		PlayerManager::getInstance()->refreshPropData(playerId, GOLD, newFishIcon, moneyCount, 0);
	}
	return moneyCount;
}

void PlayerManager::bindUI(ValueMap map, ui::TextBMFont* text, ui::TextBMFont* addText)
{
	int playerId = map["playerId"].asInt();
	int propId = map["propId"].asInt();
	Player* player = PlayerManager::getInstance()->getPlayerById(playerId);
	if (player == nullptr)
	{
		return;
	}
	player->setMoneyUI(propId, text, addText);
}

cocos2d::ValueMap PlayerManager::getAndSetPlayerData(ValueMap data)
{
	int playerId = data["playerId"].asInt();
	bool isGet = data["isGet"].asBool();
	string dataName = data["dataName"].asString();
	int val = data["val"].asInt();

	Player* player = PlayerManager::getInstance()->getPlayerById(playerId);
	ValueMap data2;
	if (player != nullptr)
	{
		data2 = player->getRoleData()->getAndSetPlayerData(data);
	}
	return data2;

}

void PlayerManager::refreshSeniorPropData(int playerId, int propId, ValueMap data, int useType, float updateDelayTime /*= 2.7f*/)
{
	Player* player = PlayerManager::getInstance()->getPlayerById(playerId);
	if (player != nullptr)
	{
		player->getRoleData()->refreshSeniorPropData(propId, data, useType, updateDelayTime);
	}
}

void PlayerManager::refreshNormalPropData(int playerId, ValueMap data, int useType, float updateDelayTime /*= 2.7f*/)
{
	Player* player = PlayerManager::getInstance()->getPlayerById(playerId);
	if (player != nullptr)
	{
		player->getRoleData()->refreshNormalPropData(data, useType, updateDelayTime);
	}
}

void PlayerManager::clearAllSeniorProp(int playerId, int clearType)
{
	Player* player = PlayerManager::getInstance()->getPlayerById(playerId);
	if (player != nullptr)
	{
		player->getRoleData()->clearAllSeniorProp(clearType);
	}
}

