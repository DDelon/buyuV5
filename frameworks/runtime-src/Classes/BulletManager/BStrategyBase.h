#ifndef __BSTRATEGY_BASE_H__
#define __BSTRATEGY_BASE_H__
#include "cocos2d.h"
USING_NS_CC;

struct BulletInfo;
class Bullet;


class BStrategyBase : public Ref
{
public:
	virtual bool init();

	virtual void initStrategyData(Bullet *pThis, BulletInfo info);

	static BStrategyBase *createStrategy(Bullet *pThis, BulletInfo info);

	virtual bool isLock() = 0;

	virtual void update(Bullet *pThis) = 0;

	virtual bool isInvalid() { return false; }

	virtual void remove();

protected:

	//计算目标位置
	Vec2 caclTargetPoint(float angle, const Vec2& srcPos);

	//计算子弹角度
	float calcAngle(const Vec2& srcPos, const Vec2& targetPos);

	//子弹飞
	void flight(Bullet *pThis, const Vec2 &srcPos, const Vec2 &targetPos);
protected:
	int _lifeTime;
	int _playerId;
	int _timelineId;
	int _fishArrayId;
	int _speed;
	std::string _bulletId;
	
	//加速到目的帧的间隔帧
	int _accelerateFrameInterval;

};

#endif