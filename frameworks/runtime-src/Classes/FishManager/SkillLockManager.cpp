#include "SkillLockManager.h"
#include "StringUtil.h"
#include "LuaCppAdapter.h"
#include "ParseGameData.h"
#include "AudioControl.h"
#include "Player/PlayerManager.h"
#include "BulletManager.h"
#include "FishFrameManager.h"

SkillLockManager *SkillLockManager::_lockInstance = nullptr;

SkillLockManager * SkillLockManager::getInstance()
{
	if (_lockInstance == nullptr)
	{
		_lockInstance = new(std::nothrow) SkillLockManager();
		if (_lockInstance && _lockInstance->init())
		{
			_lockInstance->retain();

		}
	}
	return _lockInstance;
}

bool SkillLockManager::init()
{
	Node::init();
	nIsLock = false;
	mLockNode = nullptr;
	mAimFishMap.clear();
	mLockLayer = nullptr;
	mLockNode = nullptr;
	mLockLoop = nullptr;
	mLockArrow = nullptr;
	mChainMap.clear();
	mActMap.clear();
	return true;
}

void SkillLockManager::reset()
{
	printf("-------------SkillLockManager--------reset\n");
	if (mLockLayer != nullptr)
	{
		printf("---------------------closeUpdate");
		mLockLayer->unschedule(schedule_selector(SkillLockManager::updateLock));
	}
	if (mLockNode != nullptr)
		setLockSpr(false);
	nIsLock = false;
	mLockNode = nullptr;
	mAimFishMap.clear();
	mLockLayer = nullptr;
	mLockNode = nullptr;
	mLockLoop = nullptr;
	mLockArrow = nullptr;
	mChainMap.clear();
	mActMap.clear();
}

void SkillLockManager::playLockChangeAim()
{
	//printf("------SkillLockManager:playLockChangeAim---------\n");
	auto childlist = mLockNode->getChildren();
	for (auto child : childlist)
	{
		int tag = child->getTag();
		if (mActMap[tag] != nullptr)
		{
			child->runAction(mActMap[tag]->clone());
		}
	}
}

void SkillLockManager::upDataLockSprAct(float aimPosX, float aimPosY)
{
	if (aimPosX <= 0 || aimPosY <= 0 || nIsLock == false)
	{
		setLockSpr(false);
		return;
	}
	setLockSpr(true);

	auto curPosInNode = mLockLayer->convertToNodeSpace(Vec2(aimPosX, aimPosY));
	mLockNode->setPosition(Vec2(curPosInNode.x, curPosInNode.y));
	int playerId = LuaCppAdapter::getInstance()->getPlayerId();
	Player* palyr = PlayerManager::getInstance()->getPlayerById(playerId);
	Vec2 pos = palyr->getRoleData()->curPos;
	float startPosX = pos.x;
	float startPosY = pos.y;
	int chainCount = mChainMap.size();
	for (int i = 1; i <= chainCount; i++)
	{
		float posX = startPosX + (aimPosX - startPosX) / (chainCount + 2)*(i + 1);
		float posY = startPosY + (aimPosY - startPosY) / (chainCount + 2)*(i + 1);
		Vec2 curPos = mLockLayer->convertToNodeSpace(Vec2(posX, posY));
		mChainMap[i]->setPosition(curPos);
	}

}

void SkillLockManager::setLockSpr(bool isShow)
{
	mLockNode->setVisible(isShow);
	int chainCount = mChainMap.size();
	for (int i = 1;i <=chainCount; i++)
	{
		mChainMap[i]->setVisible(isShow);
	}

}

void SkillLockManager::startMyLock()
{
	nIsLock = true;
	mLockLayer->unschedule(schedule_selector(SkillLockManager::updateLock));
	mLockLayer->schedule(schedule_selector(SkillLockManager::updateLock), 0.03f);
}

void SkillLockManager::stopMyLock()
{
	printf("--------------stopMyLock-----------------\n");
	mLockLayer->unschedule(schedule_selector(SkillLockManager::updateLock));
	int playerId = LuaCppAdapter::getInstance()->getPlayerId();
	mAimFishMap[playerId] = nullptr;
	setLockSpr(false);
}

void SkillLockManager::updateLock(float dt)
{
	//printf("--------------updateLock-----------------\n");
	int playerId = LuaCppAdapter::getInstance()->getPlayerId();
	//判断是否停止定时器
	int bullerCount = 0;
	if (!SkillLockManager::getInstance()->getIsLock())
	{
		ValueMap map = BulletManager::getInstance()->getLockBullet(playerId);
		bullerCount = map.size();
		if (bullerCount <= 0)
		{
			//printf("------SkillLockManager:playLockChangeAim---------\n");
			SkillLockManager::getInstance()->stopMyLock();
			return;
		}
	}

	//超出屏幕或者鱼死亡，转换目标鱼
	auto sizeWin = Director::getInstance()->getWinSize();
	Fish* myAimFish = SkillLockManager::getInstance()->getAimFish(playerId);
	bool isCahnge = false;
	if (myAimFish != nullptr)
	{
		int state = myAimFish->getFishState();
		Vec2 aimPos = myAimFish->getLockPosition();
		if (aimPos == Vec2(0, 0) || state == FishState::DEATH || (aimPos.x <= 0 || aimPos.x >= sizeWin.width || aimPos.y <= 0 || aimPos.y >= sizeWin.height))
		{
			isCahnge = true;
		}
	}
	else
	{
		isCahnge = true;
	}
	
	if (isCahnge)
	{
		SkillLockManager::getInstance()->chooseFishByScore();
	}

	Fish* curAimFish = SkillLockManager::getInstance()->getAimFish(playerId);
	if (curAimFish == nullptr)
	{
		SkillLockManager::getInstance()->setLockSpr(false);
		return;
	}
	Vec2 aimPos = curAimFish->getLockPosition();
	SkillLockManager::getInstance()->upDataLockSprAct(aimPos.x, aimPos.y);

	//发射子弹
	Player* player = PlayerManager::getInstance()->getPlayerById(playerId);
	Node* cannon = BulletManager::getInstance()->getCannon(playerId);
	if (cannon != nullptr)
	{
		Node* parent = cannon->getParent();
		Vec2 firstPos = parent->convertToWorldSpace(cannon->getPosition());
		float  deg = atan((aimPos.x - firstPos.x) / (aimPos.y - firstPos.y));//h弧度
		float  arg = (deg * 360) / (2 * 3.14);//化成角度了
		if ((aimPos.y - firstPos.y) < 0)
		{
			arg = arg + 180;
		}
		cannon->setRotation(arg);
	}

}

void SkillLockManager::chooseFishByScore()
{
	int playerId = LuaCppAdapter::getInstance()->getPlayerId();
	Fish* aimFish = FishFrameManager::getInstance()->getLockFishByScore();
	mAimFishMap[playerId] = aimFish;
	ValueMap val;
	if (aimFish == nullptr)
	{
		mAimFishMap[playerId] = nullptr;
	}
	else
	{
		//回调lua
		val["timelineId"] = aimFish->getTimelineId();
		val["fishArrayId"] = aimFish->getFishArrayId();
		BulletManager::getInstance()->changeBulletAim(playerId, aimFish);
		LuaCppAdapter::getInstance()->callLuaFunc("FishGMF", "CppToLua", "sendChangeAimFish", val);
		playLockChangeAim();
	}
}

//lua方面设置c++
cocos2d::ValueMap SkillLockManager::setLockData(ValueMap data)
{
	ValueMap backMap;
	int setType = data["setType"].asInt();
	int playerId = data["playerId"].asInt();
	int timelineId = data["timelineId"].asInt();
	int fishArrayId = data["fishArrayId"].asInt();
	if (setType == 1)			//开始我的锁定
	{
		if (timelineId != 0)
		{
			Fish* aimFish = FishFrameManager::getInstance()->getFishByID(timelineId, fishArrayId);
			if (aimFish != nullptr)
			{
				mAimFishMap[playerId] = aimFish;
				backMap["x"] = aimFish->getLockPosition().x;
				backMap["y"] = aimFish->getLockPosition().y;
				BulletManager::getInstance()->changeBulletAim(playerId, aimFish);
			}
			else
			{
				mAimFishMap[playerId] = nullptr;
			}
		}
		else
		{
			mAimFishMap[playerId] = nullptr;
		}
		startMyLock();
	}
	else if (setType == 2)		//停止我的锁定
	{
		//printf("----------setType == 2-------------\n");
		nIsLock = false;
	}
	else if (setType == 3)		//切换目标鱼
	{
		if (timelineId != 0)
		{
			Fish* aimFish = FishFrameManager::getInstance()->getFishByID(timelineId, fishArrayId);
			if (aimFish != nullptr)
			{
				
				mAimFishMap[playerId] = aimFish;
				backMap["x"] = aimFish->getLockPosition().x;
				backMap["y"] = aimFish->getLockPosition().y;
				int myPlayerId = LuaCppAdapter::getInstance()->getPlayerId();
				if (myPlayerId == playerId)
				{
					playLockChangeAim();
				}
				BulletManager::getInstance()->changeBulletAim(playerId, aimFish);
			}
			else
			{
				mAimFishMap[playerId] = nullptr;
				//printf("----------mAimFishMap == nullptr----timelineId=%d,fishArrayId=%d---------\n", timelineId, fishArrayId);
			}
		}
		else
		{
			mAimFishMap[playerId] = nullptr;
		}
		

	}
	return backMap;
}

Fish* SkillLockManager::getAimFish(int playerId)
{
	Fish* aimFish = mAimFishMap[playerId];
	return aimFish;
}

void SkillLockManager::setAimFish(int playerId, Fish* aimFish)
{
	mAimFishMap[playerId] = aimFish;
}

void SkillLockManager::setLuaNode(Node* bindNode, ValueMap data)
{
	string bindType = data["bindType"].asString();
	if (bindType == "lockNode")
	{
		mLockNode = bindNode;
		auto childlist = mLockNode->getChildren();
		for (auto child : childlist)
		{
			int tag = child->getTag();
			auto act = child->getActionByTag(tag);
			mActMap[tag] = act->clone();
			mActMap[tag]->retain();
			child->resume();
		}
	}
	else if (bindType == "chain")
	{
		int index = data["index"].asInt();
		mChainMap[index] = (Sprite*)bindNode;
	}
}


