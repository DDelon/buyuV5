#ifndef __ORDINARY_STRATEGY_H__
#define __ORDINARY_STRATEGY_H__
#include "BStrategyBase.h"
//ÆÕÍ¨×Óµ¯
class OrdinaryStrategy : public BStrategyBase
{
public:
	virtual bool init();
	CREATE_FUNC(OrdinaryStrategy);

	virtual void initStrategyData(Bullet *pThis, BulletInfo info);

	virtual bool isLock() { return false; }

	virtual void update(Bullet *pThis);

protected:

	char _preReturn;

};

#endif