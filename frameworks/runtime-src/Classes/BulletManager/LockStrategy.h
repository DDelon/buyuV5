#ifndef __LOCK_STRATEGY_H__
#define __LOCK_STRATEGY_H__
#include "BStrategyBase.h"
class Fish;

//�����ӵ�
class LockStrategy : public BStrategyBase
{
public:
	virtual bool init();
	CREATE_FUNC(LockStrategy);

	virtual bool isLock() { return true; }

	virtual void initStrategyData(Bullet *pThis, BulletInfo info);

	virtual void update(Bullet *pThis);

	virtual bool isInvalid() { return _isInvalid;  }

	virtual void setInvalid(bool invalid) { _isInvalid = invalid; }

	void setLockFish(int timelineId, int fishArrayId);

	void setLockFish(Fish *pFish);

	Fish *getLockFish();

	void sendlockBulletClear(Ref *pThis);

	virtual void remove();

protected:
	bool _isInvalid;

	int _followFrameInterval;

	//��������Ŀ����
	Fish *_lockTargetFish;
};

#endif