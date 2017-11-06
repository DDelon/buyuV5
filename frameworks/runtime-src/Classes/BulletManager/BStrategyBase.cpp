#include "BStrategyBase.h"
#include "Bullet.h"
#include "LockStrategy.h"
#include "OrdinaryStrategy.h"
#include "ParseGameData.h"


bool BStrategyBase::init()
{
	bool bRet = false;
	do 
	{


		bRet = true;
	} while (false);

	return bRet;
}

BStrategyBase *BStrategyBase::createStrategy(Bullet *pThis, BulletInfo info)
{
	if (info.timelineId != 0)
	{
		//锁定
		auto strategy = LockStrategy::create();
		strategy->initStrategyData(pThis, info);
		strategy->retain();
		return strategy;
	}
	else
	{
		//普通
		auto strategy = OrdinaryStrategy::create();
		strategy->initStrategyData(pThis, info);
		strategy->retain();
		return strategy;
	}
}

void BStrategyBase::initStrategyData(Bullet *pThis, BulletInfo info)
{
	
	_lifeTime = info.lifeTime;
	_accelerateFrameInterval = _lifeTime;
	//_lifeTime = (_lifeTime > 0 ? _lifeTime + (int)(_lifeTime / 20) : _lifeTime);
	_speed = GAMEVALUE_INT("config", "990000004", "data");
	_playerId = info.playerId;
	_timelineId = info.timelineId;
	_fishArrayId = info.fishArrayId;
	_bulletId = info.bulletId;

	Vec2 targetPos = caclTargetPoint(info.angle - 180, info.launcherPos);
	flight(pThis, info.launcherPos, targetPos);
	
	
}

Vec2 BStrategyBase::caclTargetPoint(float angle, const Vec2& srcPos)
{
	float rad = angle * M_PI / 180;
	const auto& winSize = Director::getInstance()->getWinSize();
	Vec2 ans;

	auto l = winSize.width + winSize.height;

	rad -= M_PI_2;
	auto fsin = sin(rad);
	auto fcos = cos(rad);

	auto x = l * fcos;
	auto y = l * fsin;

	Vec2 ptTarget = srcPos + Vec2(-x, y);
	return ptTarget;

}

float BStrategyBase::calcAngle(const Vec2& srcPos, const Vec2& targetPos)
{
	float angle = atan2f(targetPos.y - srcPos.y, targetPos.x - srcPos.x);
	float rotation = 180.0f - angle * 180.0f / M_PI + EXTRA_ANGLE + 90;
	return rotation;
}

void BStrategyBase::flight(Bullet *pThis, const Vec2 &srcPos, const Vec2 &targetPos)
{
	//根据子弹是否需要同步来决定速度
	float speed = _lifeTime > 0 ? _speed * 20 : _speed;

	float dis = targetPos.distance(srcPos);
	//同步需要的时间
	float needTime = _lifeTime / 20.0f;

	speed = (_lifeTime > 0 && needTime < (dis / _speed) ? _speed : speed);

	pThis->setPosition(srcPos);
	pThis->setRotation(calcAngle(srcPos, targetPos) + 90);
	auto action = MoveTo::create(dis / speed, targetPos);
	action->setTag(BULLET_MOVEING_TAG);
	pThis->runAction(action);

}

void BStrategyBase::remove()
{
}
