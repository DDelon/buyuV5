#include "LockStrategy.h"
#include "Bullet.h"
#include "BulletManager.h"
#include "FishFrameManager.h"
#include "PlayerManager.h"
#include "LuaCppAdapter.h"
#include "SkillLockManager.h"
#include <iostream>
#define FOLLOW_FRAME_INTERVAL 5

bool LockStrategy::init()
{
	bool bRet = false;

	do
	{
		CC_BREAK_IF(!BStrategyBase::init());

		_isInvalid = false;
		_lockTargetFish = nullptr;

		bRet = true;
	} while (false);

	return bRet;
}

void LockStrategy::initStrategyData(Bullet *pThis, BulletInfo info)
{
	BStrategyBase::initStrategyData(pThis, info);

	//锁定子弹 通过playerid获取该玩家目前锁定的鱼保存下来
	
	Fish *lockTargetFish = SkillLockManager::getInstance()->getAimFish(info.playerId);
	//Fish *lockTargetFish = FishFrameManager::getInstance()->getFishByID(info.timelineId, info.fishArrayId);
 	if (lockTargetFish == nullptr)
	{
		//通过timelineid和fisharrayid去找鱼
		lockTargetFish = FishFrameManager::getInstance()->getFishByID(info.timelineId, info.fishArrayId);
	}
	
	_lockTargetFish = lockTargetFish;
	_followFrameInterval = 0;

}

void LockStrategy::update(Bullet *pThis)
{


	if (_lifeTime > 0)
	{
		//加速同步
		pThis->setVisible(false);
		_lifeTime -= 20;
		if (_lifeTime <= 0)
		{
			_lifeTime = 0;
			pThis->setVisible(true);
		}
	}

	if (--_followFrameInterval <= 0)
	{
		_followFrameInterval = FOLLOW_FRAME_INTERVAL;
	}
	else
	{
		return;
	}

	pThis->stopAllActions();
	Vec2 curPos = pThis->getPosition();

	if (_lockTargetFish != nullptr && _lockTargetFish->isAlive() && _lockTargetFish->isVisible())
	{
		
		//锁定鱼有效
		Vec2 pos = (_playerId == PlayerManager::getInstance()->getMyPlayerId() ? _lockTargetFish->getLockPosition() : _lockTargetFish->getOtherLockPosition());
		float degree = atan((curPos.x - pos.x) / (curPos.y - pos.y)) * 180 / M_PI;

		if (curPos.y > pos.y)
		{
			degree = degree + 180;
		}
		pThis->stopActionByTag(BULLET_ROTATE_AIM_TAG);
		auto rotateAction = RotateTo::create(1.0f / 40.0f, degree);
		rotateAction->setTag(BULLET_ROTATE_AIM_TAG);
		pThis->runAction(rotateAction);
		float dis = pos.distance(curPos);
		auto moveToAction = MoveTo::create((dis / _speed)*0.6, pos);
		moveToAction->setTag(1158);
		pThis->stopActionByTag(1158);
		pThis->runAction(moveToAction);
	}
	else
	{
		//锁定鱼无效或找不到锁定鱼，飞出屏幕外
		_isInvalid = true;

		Vec2 targetPos = caclTargetPoint(pThis->getRotationSkewX() - 180, curPos);
		float dis = targetPos.distance(curPos); 
		MoveTo *pMove = MoveTo::create(dis / _speed, targetPos);
		pMove->setTag(1159);
		pThis->runAction(Sequence::create(pMove,
			CallFuncN::create(CC_CALLBACK_1(LockStrategy::sendlockBulletClear, this)),
			nullptr));
	}

}

void LockStrategy::setLockFish(int timelineId, int fishArrayId)
{
	this->setLockFish(FishFrameManager::getInstance()->getFishByID(timelineId, fishArrayId));
}

void LockStrategy::setLockFish(Fish *pFish)
{
	if (pFish != nullptr)
	{
		
		_timelineId = pFish->getTimelineId();
		_fishArrayId = pFish->getFishArrayId();

		_lockTargetFish = pFish;
	}
}

Fish *LockStrategy::getLockFish()
{
	if (_lockTargetFish != nullptr
		&& _lockTargetFish->isAlive()
		&& _lockTargetFish->getTimelineId() == _timelineId
		&& _lockTargetFish->getFishArrayId() == _fishArrayId)
	{
		return _lockTargetFish;
	}
	else
	{
		return nullptr;
	}
}

void LockStrategy::sendlockBulletClear(Ref *pThis)
{
	ValueMap data;
	ValueVector allFishDataVec;
	data["collisionFishTab"] = Value(allFishDataVec);
	ValueMap bulletPos;
	// 	bulletPos["x"] = this->getPositionX();
	// 	bulletPos["y"] = this->getPositionY();
	data["bulletPos"] = Value(bulletPos);
	data["bulletId"] = _bulletId.c_str();
	data["frame"] = FishFrameManager::getInstance()->getCurrentFrame();
	Director::getInstance()->getScheduler()->performFunctionInCocosThread([=](){
		LuaCppAdapter::getInstance()->callLuaFunc("FishGMF", "CppToLua", "sendNetCollision", data);
	});
	dynamic_cast<Bullet *>(pThis)->signRemove();
	dynamic_cast<Bullet *>(pThis)->setVisible(false);
}

void LockStrategy::remove()
{
	_lockTargetFish = nullptr;
}