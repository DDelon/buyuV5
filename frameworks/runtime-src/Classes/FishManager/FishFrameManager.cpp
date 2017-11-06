#include "FishFrameManager.h"
#include "FishTimeline.h"
#include "FishGroup.h"
#include "LuaCppAdapter.h"
#include "CustomOBB.h"
#include "DebugLog.h"
#include "BulletManager.h"
#include "ParseGameData.h"
#include "CoinManager.h"
#include "Player/PlayerManager.h"
#include "Collision/CollisionManager.h"
#include "NetsManager.h"
#include "StringUtil.h"

static FishFrameManager * sInstance = NULL;

FishFrameManager * FishFrameManager::getInstance()
{
	if (!sInstance)
	{
		sInstance =  FishFrameManager::create();
		sInstance->retain();
	}
	return sInstance;
}

bool FishFrameManager::init()
{
	Collider::init();
	ObjectPool::initObjectPool(50);
	mCurrentFrame = 0;
	mFishLayer = NULL;
	mFishArrLayer = Layer::create();
	this->addChild(mFishArrLayer);
	mFishTimelineLayer = Layer::create();
	this->addChild(mFishTimelineLayer);
	mFishGroupLayer = Layer::create();
	this->addChild(mFishGroupLayer);
	mIsPause = false;
	mIsFilp = false;
	mIsFrozen = false;
	myAimFish = nullptr;
	OBBManager::getInstance();
	return true;
}

void FishFrameManager::addFishArr(int arrId, int timelineId, int startFrameIndex, int arrBornFrame)
{
	FishArray* fishArr = FishArray::craete(arrId, timelineId, startFrameIndex, arrBornFrame, mFishLayer);
	fishArr->setBornFrame(mCurrentFrame);
	mFishArrLayer->addChild(fishArr);
}

void FishFrameManager::updateFish(float dt)
{
	if (mFishLayer==NULL)
	{
		return;
	}
	Vector<Node*> fishVec = mFishLayer->getChildren();
	for (int i = 0; i < fishVec.size();i++)
	{
		Fish* fish = dynamic_cast<Fish*>(fishVec.at(i));
		if (fish && fish->isAlive())
		{
			fish->actionInFrame(mCurrentFrame);
		}
	}

	Vector<Node*> fishArrVec = mFishArrLayer->getChildren();
	for (int i = 0; i < fishArrVec.size(); i++)
	{
		FishArray* fisharr = dynamic_cast<FishArray*>(fishArrVec.at(i));
		if (fisharr)
		{
			fisharr->actionInFrame(mCurrentFrame);
		}
	}

	Vector<Node*> fishTimelineVec = mFishTimelineLayer->getChildren();
	for (int i = 0; i < fishTimelineVec.size(); i++)
	{
		FishTimeline* fishTimeline = dynamic_cast<FishTimeline*>(fishTimelineVec.at(i));
		if (fishTimeline)
		{
			fishTimeline->actionInFrame(mCurrentFrame);
		}
	}

	Vector<Node*> fishGroupVec = mFishGroupLayer->getChildren();
	for (int i = 0; i < fishGroupVec.size(); i++)
	{
		FishGroup* fishGroup = dynamic_cast<FishGroup*>(fishGroupVec.at(i));
		if (fishGroup)
		{
			fishGroup->actionInFrame(mCurrentFrame);
		}
	}

	mCurrentFrame++;
	LuaCppAdapter::getInstance()->frameRunning(mCurrentFrame);

	
}

void FishFrameManager::start()
{
	this->schedule(schedule_selector(FishFrameManager::updateFish), 1.0f / 20.0f);
}

void FishFrameManager::stop()
{
	this->unschedule(schedule_selector(FishFrameManager::updateFish));
}

void FishFrameManager::moveToOut()
{

	Vector<Node*> fishVec = mFishLayer->getChildren();
	for (int i = 0; i < fishVec.size(); i++)
	{
		Fish* fish = dynamic_cast<Fish*>(fishVec.at(i));
		if (fish && fish->getTimelineId()>0)
		{
			fish->accelerate();
		}
	}
	mFishTimelineLayer->removeAllChildren();
	mFishGroupLayer->removeAllChildren();
	mFishArrLayer->removeAllChildren();

}

void FishFrameManager::reset()
{
	if (_nodePool == nullptr)
	{
		return;
	}
	
	this->clearPool();
	myAimFish = nullptr;
	mIsFrozen = false;
	mCurrentFrame = 0;
	mFishArrLayer->removeAllChildren();
	//mFishLayer->removeAllChildren();
	mFishTimelineLayer->removeAllChildren();
	mFishGroupLayer->removeAllChildren();

}

void FishFrameManager::addCalledFish(ValueVector fishesInfo)
{
	for (auto iter : fishesInfo)
	{
		ValueMap data = iter.asValueMap();
		int lifeFrame = (mCurrentFrame - data["frameId"].asInt()) < 0 ? 0 : (mCurrentFrame - data["frameId"].asInt());
		Fish *pFish = this->bornFish(data["fishTypeId"].asInt(), data["pathId"].asInt(), data["playerId"].asInt()*-1, data["callFishId"].asInt(), lifeFrame,Vec2(0,0),true);
		pFish->setBornFrame(data["frameId"].asInt());
		this->getFishLayer()->addChild(pFish);

		if (mIsFrozen)
		{
			pFish->setFishState(FishState::FREEZE);
		}
	}
}

void FishFrameManager::addFishTimeline(int timelineId, int frame)
{
	LuaCppAdapter::getInstance()->changeTimeline(timelineId);
	mCurrentFrame = frame;
	FishTimeline* timeline = FishTimeline::craete(mRoomLv, timelineId, false, frame, mFishLayer, mFishArrLayer);
	timeline->setBornFrame(0);
	mFishTimelineLayer->addChild(timeline);


	FishTimeline* timeline_s = FishTimeline::craete(mRoomLv, timelineId, true, frame, mFishLayer, mFishArrLayer);
	timeline_s->setBornFrame(0);
	mFishTimelineLayer->addChild(timeline_s);
}

void FishFrameManager::addFishGroup(int groupId, int frame)
{
	LuaCppAdapter::getInstance()->changeTimeline(groupId);
	ValueMap val;
	//LuaCppAdapter::getInstance()->callLuaFunc("FishGF", "CppToLua", "fishGroupCome", val);
	mCurrentFrame = frame;
	FishGroup* group = FishGroup::craete(groupId, frame, mFishArrLayer, mFishLayer);
	group->setBornFrame(0);
	mFishGroupLayer->addChild(group);
}


void FishFrameManager::addPath(int id, std::vector<Vec3> pathPoint)
{
	mPathMap.insert(make_pair(id,pathPoint));
}

std::vector<Vec3>* FishFrameManager::getPath(int id)
{
	return &(mPathMap[id]);
}

Vec3 FishFrameManager::getPathById(int timelineId, int fishArrayId, int frame)
{
	Vector<Node*> fishVec = mFishLayer->getChildren();
	for (auto iter : fishVec)
	{
		Fish *pFish = static_cast<Fish *>(iter);
		if (pFish->getTimelineId() == timelineId && pFish->getFishArrayId() == fishArrayId)
		{
			return pFish->getPathPoint(frame);
		}
	}
	return Vec3::ZERO;
}

Fish* FishFrameManager::bornFish(int id, int pathId, int timelineId, int fishArrayId, int frame, Vec2 offset, bool isCallFish)
{
	if (id == -1)
		return nullptr;

	//已经被杀死的鱼
	for (auto iter = _killedFished.begin(); iter != _killedFished.end(); ++iter)
	{
		if (iter->asValueMap()["timelineId"].asInt() == timelineId && iter->asValueMap()["fishArrayId"].asInt() == fishArrayId)
		{
			_killedFished.erase(iter);
			return nullptr;
		}
	}

	Fish *pFish = dynamic_cast<Fish *>(getPoolSingleNode());
	pFish->setFishId(id, timelineId, fishArrayId, isCallFish);
	pFish->setOffset(offset);
	pFish->setPathId(pathId, frame);
	pFish->alive();
	this->addCollisionNode(pFish);
	////CCLOG("!!!!!!!alive fish ex!!!!!== id:%d,path:%d",id, pathId);
	return pFish;
}

ValueMap FishFrameManager::dealCollision(CollisionNode *pOther)
{
	Bullet *pBullet = dynamic_cast<Bullet *>(pOther);

	//在另一个线程里面 
	//选取和网碰撞的鱼
	_collFishVec.clear();

	//存放碰撞的鱼的timelineid fisharrayid的数据 传递给lua
	ValueVector collFishInfoVec;
	ValueVector effectedFishVec;

	if (pBullet->getTimelineId() != 0)
	{
		//特殊子弹

		Fish *pAimFish = pBullet->getAimFish();
		if (pAimFish != NULL)
		{
			ValueMap fishInfo;
			fishInfo["timelineId"] = pAimFish->getTimelineId();
			fishInfo["fishArrayId"] = pAimFish->getFishArrayId();
			//collFishInfoVec.push_back(Value(fishInfo));

			if (pAimFish->getTraceType() == 6
				|| pAimFish->getTraceType() == 7
				|| pAimFish->getTraceType() == 8)
			{
				//特殊鱼 插到头
				pAimFish->getEffectedFishes(pBullet->getRate(), effectedFishVec);
				collFishInfoVec.insert(collFishInfoVec.begin(), Value(fishInfo));
			}
			else
			{
				collFishInfoVec.push_back(Value(fishInfo));
			}
		}
		else
		{
			ValueMap fishInfo;
			fishInfo["timelineId"] = pBullet->getTimelineId();
			fishInfo["fishArrayId"] = pBullet->getFishArrayId();
			collFishInfoVec.push_back(Value(fishInfo));
		}

	}
	else
	{
		//普通子弹
		Size netSize = NetsManager::getInstance()->getNetSize();
		Rect netRect = Rect(Vec2(pBullet->getPositionX() - netSize.width / 2, pBullet->getPositionY() - netSize.height / 2), netSize);
		Vector<Node *> allFishVec = mFishLayer->getChildren();
		for (auto iter : allFishVec)
		{
			Fish *pFish = dynamic_cast<Fish *>(iter);
			if (pFish && pFish->isAlive()
				&& pFish->getFishState() != FishState::DEATH
				&& pFish->getFishState() != FishState::FADE
				&& pFish->getFishState() != FishState::ACCELERATE
				&& pFish->obbCollisionWithAABB(netRect))
			{
				ValueMap fishInfo;
				fishInfo["timelineId"] = pFish->getTimelineId();
				fishInfo["fishArrayId"] = pFish->getFishArrayId();

				if (pFish->getTraceType() == 6
					|| pFish->getTraceType() == 7
					|| pFish->getTraceType() == 8)
				{
					//特殊鱼 插到头
					pFish->getEffectedFishes(pBullet->getRate(), effectedFishVec);
					collFishInfoVec.insert(collFishInfoVec.begin(), Value(fishInfo));
				}
				else
				{
					collFishInfoVec.push_back(Value(fishInfo));
				}
			}
		}
	}


	//创建发给lua的数据
	ValueMap allFishInfo;
	allFishInfo["collisionFishTab"] = collFishInfoVec;
	allFishInfo["effectedFishTab"] = effectedFishVec;
	allFishInfo["frame"] = mCurrentFrame;
	return allFishInfo;
}

Node *FishFrameManager::createPoolSingleNode()
{
	Fish* pFish = Fish::create();
	return pFish;
}

void FishFrameManager::clearPool()
{
	for (int i = 0; i < _nodePool->count(); i++)
	{
		Fish* fish = dynamic_cast<Fish*>(_nodePool->objectAtIndex(i));
		if (fish && fish->getTimelineId()>0)
		{
			fish->removeFromScreen();
		}
	}
}

void FishFrameManager::pauseFish()
{
	if (mIsPause)
	{
		this->resumeSchedulerAndActions();
	}
	else
	{
		this->pauseSchedulerAndActions();
	}
	mIsPause = !mIsPause;
}

void FishFrameManager::pauseAllFish(int type)
{
	mIsFrozen = true;
	this->pauseSchedulerAndActions();
	Vector<Node*> &fishList = this->getAllFish();
	for (int i = 0; i < fishList.size(); i++)
	{
		Fish* fish = dynamic_cast<Fish*>(fishList.at(i));
		if (fish != nullptr)
		{
			fish->setFishState(type);
		}
	}
}

void FishFrameManager::resumeAllFish()
{
	mIsFrozen = false;
	this->resumeSchedulerAndActions();
	Vector<Node*> fishList;
	fishList = this->getAllFish();
	for (int i = 0; i < fishList.size(); i++)
	{
		Fish* fish = dynamic_cast<Fish*>(fishList.at(i));
		if (fish != nullptr)
		{
			fish->setFishState(1);
		}
	}
}

void FishFrameManager::syncFrame(ValueMap data)
{
	int serverFrame = data["frameCount"].asInt();

	mSubFrame = serverFrame - mCurrentFrame;
	if (mSubFrame >= FRAME_DELAY_VAL)
	{
		for (int frame = mCurrentFrame; frame <= serverFrame; ++frame)
		{
			updateFish(frame + 1);
		}
	}
	else if (mSubFrame <= -FRAME_DELAY_VAL)
	{
		//客户端比服务器走得快
	}
}

void FishFrameManager::setMyAimFish(int timelineId, int fishArrayId)
{
	Vector<Node*> &fishList = this->getAllFish();
	for (int i = 0; i < fishList.size(); i++)
	{
		Fish* fish = dynamic_cast<Fish*>(fishList.at(i));
		if (fish != nullptr  && fish->getTimelineId() == timelineId && fish->getFishArrayId() == fishArrayId)
		{
			myAimFish = fish;
			break;
		}
	}
}

Fish* FishFrameManager::getMyAimFish()
{
	return myAimFish;
}

Fish* FishFrameManager::getLockFishByScore()
{
	Size sizeWin = Director::getInstance()->getWinSize();
	Vector<Node*> &fishList = this->getAllFish();
	Fish* aimFish = nullptr;
	for (int i = 0; i < fishList.size(); i++)
	{
		Fish* fish = dynamic_cast<Fish*>(fishList.at(i));
		if (fish != nullptr)
		{
			if (fish->getFishState() == 3 || fish->getFishState() == FishState::FADE)
			{
				continue;
			}
			
			Vec2 pos = fish->getPosition();
			if (pos.x  > 0 && pos.x <sizeWin.width && pos.y > 0 && pos.y <sizeWin.height)
			{
				if (aimFish == nullptr || aimFish->getScore() <fish->getScore())
				{
					aimFish = fish;
				} 
			}
		}
	}
	return aimFish;

}

Fish* FishFrameManager::getFishByPos(Vec2 pos)
{
	Size sizeWin = Director::getInstance()->getWinSize();
	Vector<Node*> &fishList = this->getAllFish();
	Fish* aimFish = nullptr;
	float minDistance = 150;
	for (int i = 0; i < fishList.size(); i++)
	{
		Fish* fish = dynamic_cast<Fish*>(fishList.at(i));
		if (fish != nullptr)
		{
			if (fish->getFishState() == FishState::DEATH || fish->getFishState() == FishState::FADE)
			{
				continue;
			}
			Vec2 fishPos = fish->getPosition();
			float distance = pos.distance(fishPos);
			if (distance < minDistance)
			{
				aimFish = fish;
				minDistance = distance;
			}
			
		}
	}
	if (aimFish != nullptr)
	{
		//选取到鱼 设置锁定点
		aimFish->selectLockPoint(aimFish->convertToNodeSpace(pos));
	}	
	return aimFish;
}

void FishFrameManager::setFishRed(int timelineId, int fishArrayId)
{
	Vector<Node*> &fishList = this->getAllFish();
	for (int i = 0; i < fishList.size(); i++)
	{
		Fish* fish = dynamic_cast<Fish*>(fishList.at(i));
		if (fish != nullptr  && fish->getTimelineId() == timelineId && fish->getFishArrayId() == fishArrayId)
		{
			fish->setFishColor();
			break;
		}
	}
}

void FishFrameManager::setFishDeath(int timelineId, int fishArrayId)
{
	Vector<Node*> &fishList = this->getAllFish();
	for (int i = 0; i < fishList.size(); i++)
	{
		Fish* fish = dynamic_cast<Fish*>(fishList.at(i));
		if (fish != nullptr  && fish->getTimelineId() == timelineId && fish->getFishArrayId() == fishArrayId)
		{
			fish->setFishDeath();
			break;
		}
	}
	
}

void FishFrameManager::getEffectedFishes(int poolValue, int price, int bulletRate, std::vector<Fish *> &waitSelectedFishes, std::vector<Fish *> &effectedFishes)
{
	int curPoolValue = poolValue + price * bulletRate;
	std::vector<int> indexVec;
	getRandIndexTab(indexVec, waitSelectedFishes.size());

	for (int i = 0; i < indexVec.size(); ++i)
	{
		int coinNum = waitSelectedFishes[indexVec[i]]->getScore()*bulletRate;

		if (curPoolValue > 0)
		{
			curPoolValue -= coinNum;
			effectedFishes.push_back(waitSelectedFishes[indexVec[i]]);
		}
		else
		{
			break;
		}
	}
	
}

void FishFrameManager::getThunderPool(int bulletRate, std::vector<Fish *> &effectedFishes)
{
	int curThunderRate = mThunderRate + 100 * bulletRate;
	std::vector<int> indexVec;
	


	effectedFishes.clear();
	//闪电鱼规则
	Vector<Node*> &fishList = this->getAllFish();
	
	//待选择的列表
	std::vector<Fish*> waitSelList;
	for (int i = 0; i < fishList.size(); i++)
	{
		//判断是不是小鱼(除了BOSS、奖金鱼、其他功能鱼 30倍以上的鱼)
		Fish* fish = dynamic_cast<Fish*>(fishList.at(i));
		if (fish && fish->isAlive()
			&& fish->getFishState() != FishState::DEATH
			&& fish->getFishState() != FishState::FADE
			&& fish->getFishState() != FishState::ACCELERATE
			&& fish->getTraceType() != 3
			&& fish->getTraceType() != 5
			&& fish->getTraceType() != 6
			&& fish->getTraceType() != 7
			&& fish->getTraceType() != 8)
		{
			if (fish->getTraceType() == 1 || fish->getTraceType() == 2 || fish->getTraceType() == 4)
			{
				if (fish->getScore() <= GAMEVALUE_INT("config", "990000024", "data"))
				{
					waitSelList.push_back(fish);
				}
			}
		}
		
	}
	
	//排序
	/*auto sortFunc = [=](Fish *pFish1, Fish *pFish2)->bool{
		if (pFish1->getScore() < pFish2->getScore())
		{
			return true;
		}
		else
		{
			return false;
		}
	};*/
	//std::sort(waitSelList.begin(), waitSelList.end(), sortFunc);
	getEffectedFishes(mThunderRate, 100, bulletRate, waitSelList, effectedFishes);
	//从大到小选鱼 

	
}

void FishFrameManager::getSameFishType(int fishType, std::vector<Fish *> &effectedFishes)
{
	effectedFishes.clear();
	//DebugLog::getInstance()->log("same fish type bomb fishtype: " + Value(fishType).asString());
	//同类炸弹
	Vector<Node*> &fishList = this->getAllFish();
	for (int i = 0; i < fishList.size(); i++)
	{
		Fish* fish = dynamic_cast<Fish*>(fishList.at(i));
		if (fish && fish->isAlive()
			&& fish->getFishState() != FishState::DEATH
			&& fish->getFishState() != FishState::FADE
			&& fish->getFishState() != FishState::ACCELERATE)
		{
			if (fish->getId() == fishType)
			{
				effectedFishes.push_back(fish);
			}
		}
		
	}

	
}

void FishFrameManager::getRangeFish(int range, Vec2 pos, std::vector<Fish *> &effectedFishes)
{
	//局部炸弹
	Vector<Node*> &fishList = this->getAllFish();
	for (int i = 0; i < fishList.size(); i++)
	{
		Fish* fish = dynamic_cast<Fish*>(fishList.at(i));
		if (fish && fish->isAlive()
			&& fish->getFishState() != FishState::DEATH
			&& fish->getFishState() != FishState::FADE
			&& fish->getFishState() != FishState::ACCELERATE
			&& fish->getTraceType() != 3
			&& fish->getTraceType() != 5
			&& fish->getTraceType() != 6
			&& fish->getTraceType() != 7
			&& fish->getTraceType() != 8)
		{
			//effectedFishes.push_back(fish);

			
			//float radius = fish->getRadius();
			//float dis = pos.distance(fish->getPosition());
			//if (dis <= radius + range)
			if (fish->obbCollisionWithCircle(pos, range))
			{
				effectedFishes.push_back(fish);
			}
		}
	}
	
}

void FishFrameManager::getNBombRangeFish(int range, Vec2 pos, std::vector<Fish *> &effectedFishes)
{

	//核弹
	Vector<Node*> &fishList = this->getAllFish();
	for (int i = 0; i < fishList.size(); i++)
	{
		Fish* fish = dynamic_cast<Fish*>(fishList.at(i));
		if (fish && fish->isAlive()
			&& fish->getFishState() != FishState::DEATH
			&& fish->getFishState() != FishState::FADE
			&& fish->getFishState() != FishState::ACCELERATE
			&& fish->getTraceType() != 5
			&& fish->getTraceType() != 6
			&& fish->getTraceType() != 7
			&& fish->getTraceType() != 8
			&& fish->getTraceType() != 10)
		{
			if (fish->obbCollisionWithCircle(pos, range))
			{
				effectedFishes.push_back(fish);
			}
		}
	}
	/*LayerColor *pLayerColor = LayerColor::create(Color4B(100, 0, 0, 100));
	pLayerColor->setContentSize(Size(range * 2, range * 2));
	pLayerColor->setPosition(Vec2(pos.x - range, pos.y-range));
	Director::getInstance()->getRunningScene()->addChild(pLayerColor, 18181);*/
	
}

void FishFrameManager::getNBombFish(int bombLv, Vec2 pos, std::vector<Fish *> &effectedFishes)
{
	//int poolValue = this->getNBombRate();
	std::string keyStr = "44000000" + Value(bombLv).asString();
	//int price = GAMEVALUE_INT("item", "200000006", "inner_value");
	int range = GAMEVALUE_INT("bomb", keyStr, "range");
	log("bomb range:%d", range);
	//int itemId = GAMEVALUE_INT("bomb", keyStr, "item_id");
	std::vector<Fish *> waitSelectedFishes;

	this->getNBombRangeFish(range, pos, effectedFishes);
	//this->getEffectedFishes(poolValue, 0, 1, waitSelectedFishes, effectedFishes);
}

Fish* FishFrameManager::getFishByID(int timelineId, int fishArrayId)
{
	Vector<Node*> &fishList = this->getAllFish();
	for (int i = 0; i < fishList.size(); i++)
	{
		Fish* fish = dynamic_cast<Fish*>(fishList.at(i));
		if (fish != nullptr  && fish->getTimelineId() == timelineId && fish->getFishArrayId() == fishArrayId)
		{
			if (fish == nullptr )
			{
				return fish;
			}
			return fish;
		}
	}
	return nullptr;
}

void FishFrameManager::getRandIndexTab(std::vector<int> &indexVec, int maxNum)
{
	std::vector<int> randVec;
	for (int i = 0; i < maxNum; ++i)
	{
		randVec.push_back(i);
	}
	int randNum = (maxNum < 50) ? maxNum : 50;
	

	while (true)
	{

		if (randNum <= 0)
		{
			break;
		}
		int index = RandomHelper::random_int(0, (int)randVec.size()-1);
		int num = randVec[index];
		auto iter = randVec.begin() + index;
		randVec.erase(iter);
		indexVec.push_back(num);
		randNum--;
		////CCLOG("-----------------loop");
	}
	////CCLOG("---------------pause");
	/*for (int i = 0; i < randVec.size(); ++i)
	{
		bool isContinue = false;
		int num = RandomHelper::random_int(0, maxNum);
		for (auto iter : indexVec)
		{
			if (num == iter)
			{
				i--;
				isContinue = true;
				break;
			}
		}
		if (isContinue)
			continue;
		indexVec.push_back(num);
	}*/
}

void FishFrameManager::onHit(ValueMap data)
{
	auto scene = Director::getInstance()->getRunningScene();
	int playerId = data["playerId"].asInt();
	int chairId = data["chairId"].asInt();
	float gunRate = data["gunRate"].asFloat();
	int frameId = data["frameId"].asInt();
	string bulletId = data["bulletId"].asString();

	bool isTax = true;
	int costPropId = GOLD;
	int newPropCount = 0;
	auto hitType = data["hitType"].asString();
	float rateBuff = 1;
	if (hitType == "friendHit")
	{
		data["dropType"] = "friend";
		isTax = false;
		costPropId = SCORE;
		newPropCount = data["newScore"].asInt();
		//更新剩余子弹
		int leftBullet = data["leftBullet"].asInt();
		PlayerManager::getInstance()->refreshPropData(playerId, BULLET, leftBullet, 0, 0);

		//判断子弹加成
		float buff = 1;
		auto iter = data.find("effects");
		if (iter != data.end())//有加成
		{
			
			ValueVector effects = data["effects"].asValueVector();
			for (int i = 0; i < effects.size(); i++)
			{
				int effectId = effects[i].asInt();
				if (effectId == 2)
				{
					
					buff = buff * 2;
				}
				if (effectId == 5)
				{
					buff = buff / 2;
				
				}
			}
		}
		rateBuff = rateBuff*buff;

	}
	else
	{
		data["dropType"] = "normal";
		newPropCount = data["newFishIcon"].asInt();
	}

	//新倍数

	gunRate = gunRate*rateBuff;
	CoinManager::getInstance()->resetPlayCoinNum();
	float posX = -999.0f;
	float posY = -999.0f;

	Bullet* bullet = BulletManager::getInstance()->getBulletById(bulletId);
	if (bullet)
	{
		bullet->delaySignRemove(frameId - mCurrentFrame);
	}

	//=====================处理鱼======================
	int normalCoin = 0;
	ValueVector killVector;
	auto iter = data.find("killedFishes");
	if (iter!=data.end())//有杀死鱼
	{
		killVector = data["killedFishes"].asValueVector();
		int timelineId = killVector[0].asValueMap()["timelineId"].asInt();
		int fishArrayId = killVector[0].asValueMap()["fishArrayId"].asInt();
		Fish* firstFish = getFishByID(timelineId, fishArrayId);
		int specialFishKillCoin = 0;
		
		if (firstFish)
		{
			if (firstFish->isSpecialFish())
			{
				//特殊鱼先死亡
				firstFish->setFishDeath();
				firstFish->getTraceType() == 7 ? specialFishKillCoin = 0 : specialFishKillCoin += (gunRate * firstFish->getScore());
				float allDelayTime = CoinManager::getInstance()->showGainCoinEffect(playerId, chairId, gunRate * firstFish->getScore(), firstFish->getDropCoin(), firstFish->getPosition(), playerId == LuaCppAdapter::getInstance()->getPlayerId());
				for (int i = 1; i < killVector.size(); i++)
				{
					int timelineId = killVector[i].asValueMap()["timelineId"].asInt();
					int fishArrayId = killVector[i].asValueMap()["fishArrayId"].asInt();
					Fish* fish = getFishByID(timelineId, fishArrayId);
					if (fish)
					{
						posX = fish->getPositionX();
						posY = fish->getPositionY();
						ValueMap fishData;
						data["isSpecialFish"] = true;
						data["firstFishPosX"] = firstFish->getPositionX();
						data["firstFishPosY"] = firstFish->getPositionY();
						data["firstFishTraceType"] = firstFish->getTraceType();
						data["gunRate"] = gunRate;
						data["isTax"] = isTax;
						data["randomFishScore"] = data["randomFishScore"].asInt();
						data["chairId"] = chairId;
						data["playerId"] = playerId;
						data["costPropId"] = costPropId;
						data["newPropCount"] = newPropCount;
						this->onFishHit(fish, data);

					}
					else
					{
						printf("------------fish == null----------0------");
					}
				}
				if (specialFishKillCoin >= GAMEVALUE_INT("config", "990000069", "data") && playerId == LuaCppAdapter::getInstance()->getPlayerId())
				{
					ValueMap val;
					val["score"] = specialFishKillCoin;
					val["playerId"] = playerId;
					LuaCppAdapter::getInstance()->callLuaFunc("FishGMF", "CppToLua", "propMegaWin", val);
				}
			}
			else
			{
				//普通鱼
				for (int i = 0; i < killVector.size(); i++)
				{
					int timelineId = killVector[i].asValueMap()["timelineId"].asInt();
					int fishArrayId = killVector[i].asValueMap()["fishArrayId"].asInt();
					Fish* fish = getFishByID(timelineId, fishArrayId);
					if (fish)
					{
						posX = fish->getPositionX();
						posY = fish->getPositionY();
						ValueMap fishData;
						data["isSpecialFish"] = false;
						data["gunRate"] = gunRate;
						data["isTax"] = isTax;
						data["randomFishScore"] = data["randomFishScore"].asInt();
						data["chairId"] = chairId;
						data["playerId"] = playerId;
						data["costPropId"] = costPropId;
						data["newPropCount"] = newPropCount;
						int coin = this->onFishHit(fish, data);
						normalCoin = normalCoin + coin;
					}
					else
					{
						printf("------------fish == null----------0------");
					}
				}
			}
		}
	}
	else
	{
		//没有杀死的鱼也要更新下鱼币 破产判断时候用
		PlayerManager::getInstance()->refreshPropData(playerId, costPropId, newPropCount, 0, 0);
	}

	//==========================鱼变红
	ValueVector *hitVector = CollisionManager::getInstance()->getCollFishVec(bulletId);
	if (hitVector != NULL)
	{
		//DebugLog::getInstance()->log("hit vec size:" + Value(hitVector->size()).asString());
		for (int i = 0; i < hitVector->size(); i++)
		{
			int timelineId = (*hitVector)[i].asValueMap()["timelineId"].asInt();
			int fishArrayId = (*hitVector)[i].asValueMap()["fishArrayId"].asInt();
			bool isDeath = false;
			for (int k = 0; k < killVector.size(); k++)
			{
				if (killVector[k].asValueMap()["timelineId"].asInt() == timelineId
					&&killVector[k].asValueMap()["fishArrayId"].asInt() == fishArrayId)
				{
					isDeath = true;
					break;
				}
			}

			if (!isDeath&&playerId == LuaCppAdapter::getInstance()->getPlayerId())
			{
				setFishRed(timelineId, fishArrayId);
			}
		}
	}

	//===========核弹===============水晶处理
	if (hitType == "NBombHit")
	{
		int useType = data["useType"].asInt();

		//更新真实数据
		ValueMap mapGem;
		mapGem["playerId"] = playerId;
		mapGem["isShow"] = true;
		int nPropID = data["nPropID"].asInt();
		int propId = 0;
		int propCount = 0;
		if (useType == 1)
		{

			string price = GAMEVALUE_STRING("item", IntToString(200000000 + nPropID), "price");
			vector<string> priceTab;
			StringUtil::_STR_SPLIT(price, ",", priceTab);

			propId = atoi(priceTab[0].c_str());
			propCount = -atoi(priceTab[1].c_str());
			log("---------NBombHit-------propId=%d----------propCount=%d----", propId, propCount);
		}
		else
		{
			propId = nPropID;
			propCount = -1;
		}

		mapGem["propId"] = propId;
		mapGem["propCount"] = propCount;
		bool isSucceed = data["isSuccess"].asBool();
		if (isSucceed)
		{
			PlayerManager::getInstance()->addTrueAndFlyProp(mapGem);
		}
		else
		{
			DebugLog::getInstance()->log("-------------bomb is faile-----------------");
		}
		//清除自己的缓存
		int myPlayerId = PlayerManager::getInstance()->getMyPlayerId();
		if (myPlayerId == playerId)
		{
			mapGem["isSure"] = true;
			if (propCount <0)
			{
				propCount = -propCount;
			}
			mapGem["propCount"] = propCount;
			PlayerManager::getInstance()->isSurePropData(mapGem);
		}

		if (nPropID == 16 && normalCoin >= GAMEVALUE_INT("config", "990000090", "data") && playerId == LuaCppAdapter::getInstance()->getPlayerId())
		{
			ValueMap val;
			val["score"] = normalCoin;
			val["playerId"] = playerId;
			LuaCppAdapter::getInstance()->callLuaFunc("FishGMF", "CppToLua", "propWindfall", val);
		}

		return;
	}



	//==========普通击中================处理掉落
	bool isDrop = false;
	//处理水晶
	auto iternewCrystal = data.find("newCrystal");
	auto iterDropCrystal = data.find("dropCrystal");
	if (iternewCrystal != data.end())
	{
		if (iterDropCrystal != data.end())
		{
			int newCrystal = data["newCrystal"].asInt();
			isDrop = true;
			//手动设置数据,不刷新界面
			PlayerManager::getInstance()->setPropRealData(playerId, 2, newCrystal);

			//提前加入缓存
			int dropCrystal = data["dropCrystal"].asInt();
			ValueMap mapGem;
			mapGem["playerId"] = playerId;
			mapGem["isSure"] = false;
			mapGem["propId"] = 2;
			mapGem["propCount"] = dropCrystal;
			PlayerManager::getInstance()->setAddFlyProp(mapGem);

		}
		else
		{
			int newCrystal = data["newCrystal"].asInt();
			//直接更新水晶数据
			PlayerManager::getInstance()->refreshPropData(playerId, 2, newCrystal, 0, 0);
		}
	}
	
	//掉落道具处理
	auto iterDropProps = data.find("dropProps");
	if (iterDropProps != data.end())
	{
		isDrop = true;
		//掉落道具
		auto dropProps = data["dropProps"].asValueVector();
		for (std::vector<Value>::iterator it = dropProps.begin(); it != dropProps.end(); it++)
		{
			ValueMap mapProp = (*it).asValueMap();
			int propId = mapProp["propId"].asInt();
			int propCount = mapProp["propCount"].asInt();
			ValueMap map;
			map["playerId"] = data["playerId"].asInt();
			map["isSure"] = false;
			map["propId"] = propId;
			map["propCount"] = propCount;
			//刷新真实道具
			PlayerManager::getInstance()->addTrueAndFlyProp(map);
			//提前加入缓存
			PlayerManager::getInstance()->setAddFlyProp(map);
		}
	}

	//掉落高级道具处理
	auto iterDropSeniorProps = data.find("dropSeniorProps");
	if (iterDropSeniorProps != data.end())
	{
		isDrop = true;
		//掉落高级道具
		auto dropSeniorProps = data["dropSeniorProps"].asValueVector();
		for (std::vector<Value>::iterator it = dropSeniorProps.begin(); it != dropSeniorProps.end(); it++)
		{
			ValueMap mapProp = (*it).asValueMap();
			int propId = mapProp["propId"].asInt();
			int useType = 8;
			float updateDelayTime = 0;
			ValueMap propData = mapProp;
			PlayerManager::getInstance()->refreshSeniorPropData(playerId, propId, propData, useType, updateDelayTime);
		}
	}

	if (isDrop)
	{
		data["posX"] = posX;
		data["posY"] = posY;
		LuaCppAdapter::getInstance()->callLuaFunc("FishGMF", "CppToLua", "dropThings", data);
	}
}

int FishFrameManager::onFishHit(Fish* fish, ValueMap data)
{
	if (fish == nullptr)
	{
		return 0 ;
	}
	bool isSpecialFish = data["isSpecialFish"].asBool();
	float gunRate = data["gunRate"].asFloat();
	bool isTax = data["isTax"].asBool();
	int randomFishScore = data["randomFishScore"].asInt();
	int chairId = data["chairId"].asInt();
	int playerId = data["playerId"].asInt();
	int costPropId = data["costPropId"].asInt();
	int newPropCount = data["newPropCount"].asInt();

	int score = fish->getScore();
	float posX = fish->getPositionX();
	float posY = fish->getPositionY();
	int traceType = fish->getTraceType();

	if (isSpecialFish)
	{
		float firstFishPosX = data["firstFishPosX"].asFloat();
		float firstFishPosY = data["firstFishPosY"].asFloat();
		int firstFishTraceType = data["firstFishTraceType"].asInt();
		fish->specialFishDeath(Vec2(firstFishPosX, firstFishPosY), firstFishTraceType);
	}
	else
	{
		fish->setFishDeath();
	}

	float delayTime = 0;
	//变倍率boss
	if (traceType == 10)
	{
		delayTime = 3.5 + COIN_ANIMATION_DELAY + COIN_LABEL_ANIMATION_DELAY;
		score = data["randomFishScore"].asInt();
	}
	if (delayTime == 0)
	{
		delayTime = COIN_ANIMATION_DELAY + COIN_LABEL_ANIMATION_DELAY;
	}

	int coinNum = gunRate * score;
	bool isBonusFish = false;
	if (traceType == 3 || traceType == 4 || traceType == 5 || traceType == 10)
	{
		if (isTax && (traceType == 3 || traceType == 4))
		{
			coinNum = gunRate * score * (1 - (float)GAMEVALUE_INT("config", "990000052", "data") / 10000);
		}
		isBonusFish = true;
	}

	if (traceType != 10)
	{
		if (isBonusFish)
		{
			CoinManager::getInstance()->killBonusFish(coinNum, fish->getId(), chairId, playerId, delayTime);
		}
		float allDelayTime = CoinManager::getInstance()->showGainCoinEffect(playerId, chairId, coinNum, fish->getDropCoin(), Vec2(posX, posY), playerId == LuaCppAdapter::getInstance()->getPlayerId(), Vec2(0, 0), true, delayTime);
		PlayerManager::getInstance()->refreshPropData(playerId, costPropId, newPropCount, coinNum, allDelayTime);

	}
	else
	{
		PlayerManager::getInstance()->setPropRealData(playerId, costPropId, newPropCount);
		Player* player = PlayerManager::getInstance()->getPlayerById(playerId);
		if (player != NULL)
		{
			player->getRoleData()->setAddFlyProp(false, costPropId, coinNum);
		}

		ValueMap val;
		val["bossEndRate"] = score;
		//val["delayTime"] = delayTime - (COIN_ANIMATION_DELAY + COIN_LABEL_ANIMATION_DELAY);
		val["playerId"] = playerId;
		val["chairId"] = chairId;
		val["fishId"] = fish->getId();
		val["coinNum"] = coinNum;
		val["posX"] = posX;
		val["posY"] = posY;
		val["posY"] = posY;
		val["dropCoin"] = fish->getDropCoin();
		val["costPropId"] = costPropId;
		LuaCppAdapter::getInstance()->callLuaFunc("FishGMF", "CppToLua", "playBossRateChange", val);
	}

	return coinNum;
}


void FishFrameManager::showLockPointUI()
{
	Vector<Node*> fishVec = mFishLayer->getChildren();
	for (int i = 0; i < fishVec.size(); i++)
	{
		Fish* fish = dynamic_cast<Fish*>(fishVec.at(i));
		if (fish && fish->isAlive())
		{
			fish->addLockPointUI();
		}
	}
}

