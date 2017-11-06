#include "OrdinaryStrategy.h"
#include "Bullet.h"

bool OrdinaryStrategy::init()
{
	bool bRet = false;

	do 
	{
		CC_BREAK_IF(!BStrategyBase::init());


		bRet = true;
	} while (false);

	return bRet;
}

void OrdinaryStrategy::initStrategyData(Bullet *pThis, BulletInfo info)
{
	BStrategyBase::initStrategyData(pThis, info);

	_preReturn = 'N';
}

void OrdinaryStrategy::update(Bullet *pThis)
{
	const auto& winSize = Director::getInstance()->getWinSize();
	int targetRotat = 0;
	int curRotat = 0;

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

	if (pThis->getPositionX() <= 0 && _preReturn != 'L' /*&& this->getPositionY() > 0 && this->getPositionY() < winSize.height*/)
	{
		Vec2 pos;
		pos.x = 0;
		pos.y = pThis->getPositionY() - (pThis->getPositionX() / (tan((pThis->getRotation()*3.14/180))));

		curRotat = pThis->getRotation() - 90;
		curRotat = curRotat % 360;

		pThis->stopAllActions();
		_preReturn = 'L';

		if (curRotat >= 180)
		{
			targetRotat = 450 - curRotat;
			flight(pThis, pos, caclTargetPoint(targetRotat, pos));
		}
		else
		{
			targetRotat = 450 - curRotat;
			flight(pThis, pos, caclTargetPoint(targetRotat, pos));
		}
	}

	else if (pThis->getPositionY() <= 0 && _preReturn != 'D'/* && this->getPositionX() > 0 && this->getPositionX() < winSize.width*/)
	{
		Vec2 pos;
		pos.y = 0;
		pos.x = pThis->getPositionX() - pThis->getPositionY()*tan((pThis->getRotation()*3.14 / 180));

		curRotat = pThis->getRotation() - 90;
		curRotat = curRotat % 360;

		pThis->stopAllActions();
		_preReturn = 'D';

		if (curRotat <= 90)
		{
			targetRotat = 270 - curRotat;
			flight(pThis, pos, caclTargetPoint(targetRotat, pos));
		}
		else
		{
			targetRotat = 270 - curRotat;
			flight(pThis, pos, caclTargetPoint(targetRotat, pos));
		}
	}

	else if (pThis->getPositionX() >= winSize.width && _preReturn != 'R'/* && this->getPositionY() > 0 && this->getPositionY() < winSize.height*/)
	{
		Vec2 pos;
		pos.x = winSize.width;
		pos.y = pThis->getPositionY() - ((pThis->getPositionX() - winSize.width) / (tan((pThis->getRotation()*3.14 / 180))));


		curRotat = pThis->getRotation() - 90;
		curRotat = curRotat % 360;

		pThis->stopAllActions();
		_preReturn = 'R';

		if (curRotat > 270 || curRotat == 0)
		{
			targetRotat = 450 - curRotat;
			flight(pThis, pos, caclTargetPoint(targetRotat, pos));
		}
		else
		{
			targetRotat = 450 - curRotat;
			flight(pThis, pos, caclTargetPoint(targetRotat, pos));
		}
	}

	else if (pThis->getPositionY() >= winSize.height && _preReturn != 'U' /*&& this->getPositionX() > 0 && this->getPositionX() < winSize.width*/)
	{
		Vec2 pos;
		pos.y = winSize.height;
		pos.x = pThis->getPositionX() - (pThis->getPositionY() - winSize.height)*tan((pThis->getRotation()*3.14 / 180));

		curRotat = pThis->getRotation() - 90;
		curRotat = curRotat % 360;

		pThis->stopAllActions();
		_preReturn = 'U';

		if (curRotat <= 270)
		{
			targetRotat = 360 - curRotat - 90;
			flight(pThis, pos, caclTargetPoint(targetRotat, pos));
		}
		else if (curRotat > 270)
		{
			targetRotat = 360 - curRotat - 90;
			flight(pThis, pos, caclTargetPoint(targetRotat, pos));
		}
	}
}


