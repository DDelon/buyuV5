#include "Bullet.h"
#include "StringUtil.h"
#include "FishFrameManager.h"
#include "LuaCppAdapter.h"
#include "ParseGameData.h"
#include "DebugLog.h"
#include "BulletManager.h"
#include "NetsManager.h"
#include "PlayerManager.h"
#include "LockStrategy.h"

int Bullet::_count = 0;
bool Bullet::init()
{
	bool bRet = false;

	do 
	{
		CC_BREAK_IF(!CollisionNode::init());
		_status = BulletStatus::NORMAL;
		_pStrategy = nullptr;

		bRet = true;
	} while (false);

	return bRet;
}

void Bullet::initSprite(std::string resId)
{
	
	std::string sprRes("battle/effect/");
	sprRes.append(GAMEVALUE_STRING("cannonoutlook", resId, "bullet_img"));
	_pSprite = Sprite::create(sprRes);
	_pSprite->setVisible(true);
	_pSprite->setAnchorPoint(Vec2(0.5, 0.5));
	_pSprite->setPosition(Vec2(_pSprite->getContentSize().width / 2, _pSprite->getContentSize().height / 2));
	_pColliSpr = Sprite::create("battle/battleUI/bl_pic_bullet_collision.png");
	_pColliSpr->setAnchorPoint(Vec2(0.5, 0.5));
	_pColliSpr->setPosition(Vec2(_pSprite->getContentSize().width*0.75, _pSprite->getContentSize().height / 2 + 28));
	_pColliSpr->setVisible(false);

}

void Bullet::initEffect(int effectId)
{
	if (effectId == 17) //狂暴
	{
		Sprite *pTemp = Sprite::create("battle/effect/effect_bullet_superpos_01.png");
		pTemp->setPosition(Vec2(_pSprite->getContentSize().width / 2, _pSprite->getContentSize().height / 2));
		_pSprite->addChild(pTemp, 100);
	}
}

void Bullet::setBulletInfo(BulletInfo info)
{
	_info = info;
	_count++;
	_speed = GAMEVALUE_INT("config", "990000004", "data");
	
	_status = BulletStatus::RUN; 

	_delayRemoveFrame = -1;

	//
	
	this->initSprite(_info.vipRes);
	this->initEffect(_info.effectId);
	if (_pStrategy != nullptr)
	{
		_pStrategy->release();
		_pStrategy = nullptr;
	}
	_pStrategy = BStrategyBase::createStrategy(this, _info);
	this->setContentSize(_pSprite->getContentSize());
	this->setAnchorPoint(Vec2(0.5, 0.5));
	this->addChild(_pSprite);
	this->addChild(_pColliSpr);
	this->setPosition(_info.launcherPos);
	this->setRotation(_info.angle);

	
	//初始化碰撞数据
	this->initCollisionPoint(Rect(_pColliSpr->getBoundingBox().origin, _pColliSpr->getBoundingBox().size));
}

Rect Bullet::getBoundingBox()
{
	//log("Bullet::getBoundingBox begin this:%p colliSpr:%p", this, _pColliSpr);
	Rect rect = Node::getBoundingBox();
	rect.origin.x += ((rect.size.width - 20) / 2);
	rect.origin.y += ((rect.size.height - 38) / 2)+28;
	rect.size.width = 20;
	rect.size.height = 38;
	return rect;
	/*if (_pColliSpr == nullptr)
	{
		return Rect::ZERO;
	}
	return Rect(this->convertToWorldSpace(_pColliSpr->getBoundingBox().origin), _pColliSpr->getBoundingBox().size);*/

}

void Bullet::removeFromScreen()
{
	
	_pSprite->setVisible(true);
	this->setVisible(true);
	this->removeChild(_pColliSpr);
	_pColliSpr = nullptr;
	_status = BulletStatus::NORMAL;
	this->stopAllActions();
	this->removeAllChildren();
	this->removeFromParent();
	this->clearCollData();
	_info.timelineId = 0;
}

bool Bullet::isUseCollAlgori()
{
	//log("Bullet::isUseCollAlgori begin");
	if (_pStrategy != nullptr)
	{
		return !_pStrategy->isLock();
	}
	else
	{
		//log("Bullet::_pStrategy is nil");
		return false;
	}
	
}

ValueMap Bullet::dealNodeCollision(CollisionNode *pOther)
{
	Fish *pFish = dynamic_cast<Fish *>(pOther);
	//删除
	_isCollision = true;
	//temp
	signRemove();

	//撒网
	Vec2 netsPos = this->convertToWorldSpace(_pColliSpr->getPosition());
	Director::getInstance()->getScheduler()->performFunctionInCocosThread([=](){
		NetsManager::getInstance()->createNets(_info.vipRes, netsPos, _info.effectId);
	});

	//返回子弹位置信息
	ValueMap bulletPos;
	bulletPos["x"] = this->getPositionX();
	bulletPos["y"] = this->getPositionY();

	ValueMap bulletInfo;
	bulletInfo["bulletPos"] = bulletPos;
	bulletInfo["bulletId"] = _info.bulletId.c_str();
	bulletInfo["isMy"] = (PlayerManager::getInstance()->getMyPlayerId() == _info.playerId ? true : false);
	return bulletInfo;

}

bool Bullet::customCollision()
{
	LockStrategy *pStrategy = dynamic_cast<LockStrategy *>(_pStrategy);
	if (pStrategy == nullptr)
		return false;
	Fish *pFish = pStrategy->getLockFish();
	if (pFish && pFish->isAlive())
	{
		//锁定的鱼是有效的
		Vec2 fishPos = (_info.playerId == PlayerManager::getInstance()->getMyPlayerId() ? pStrategy->getLockFish()->getLockPosition() : pStrategy->getLockFish()->getOtherLockPosition());
		float distance = this->getPosition().distance(fishPos);
		if (distance <= 50)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		//锁定的鱼无效 换鱼或者子弹飞出去
		Fish* aimFish = FishFrameManager::getInstance()->getLockFishByScore();
		if (aimFish != nullptr)
		{
			pStrategy->setLockFish(aimFish);
		}
		else
		{
			pStrategy->setInvalid(true);
			pStrategy->sendlockBulletClear(this);
		}
		return false;
	}
	
}

void Bullet::updatePoints()
{
	//重写CollisionNode updatePoints
	/*_curCollisionPoints.clear();
	Rect rect = 
	Vec2 leftUpPoint = Vec2(rect.origin.x, rect.origin.y + rect.size.height);
	Vec2 leftDownPoint = Vec2(rect.origin.x, rect.origin.y);
	Vec2 rightUpPoint = Vec2(rect.origin.x + rect.size.width, rect.origin.y + rect.size.height);
	Vec2 rightDownPoint = Vec2(rect.origin.x + rect.size.width, rect.origin.y);

	_curCollisionPoints.push_back(leftUpPoint);
	_curCollisionPoints.push_back(leftDownPoint);
	_curCollisionPoints.push_back(rightUpPoint);
	_curCollisionPoints.push_back(rightDownPoint);
	*/
	//this->resetRect(this->getBoundingBox());
	//DebugLog::getInstance()->log("size:" + Value(_curCollisionPoints.size()).asString());
	//DebugLog::getInstance()->log("left up point:" + Value(leftUpPoint.x).asString() + "," + Value(leftUpPoint.y).asString());
}

void Bullet::update()
{
	if (_delayRemoveFrame != -1)
	{
		_delayRemoveFrame -= 1;
		if (_delayRemoveFrame == 0)
		{
			this->signRemove();
			_delayRemoveFrame = -1;
		}
	}
	this->updatePoints();

	if (_pStrategy->isInvalid())
	{
		return;
	}

	_pStrategy->update(this);

	if (this->isOverScreen() || this->isVisible() == false)
	{
		ValueMap info;
		info["bulletId"] = _info.bulletId;
		info["collisionFishTab"] = ValueMap();
		LuaCppAdapter::getInstance()->callLuaFunc("FishGMF", "CppToLua", "sendNetCollision", info);
		this->signRemove();
	}

}

void Bullet::setAimFish(int timelineId, int fishArrayId)
{
	//_info.timelineId = timelineId;
	//_info.fishArrayId = fishArrayId;

	//更新数据
	_info.timelineId = timelineId;
	_info.fishArrayId = fishArrayId;

	LockStrategy *pStrategy = dynamic_cast<LockStrategy *>(_pStrategy);
	if (pStrategy != nullptr)
	{
		pStrategy->setLockFish(timelineId, fishArrayId);
	}
}

void Bullet::setAimFish(Fish* fish)
{
	//更新数据
	_info.timelineId = fish->getTimelineId();
	_info.fishArrayId = fish->getFishArrayId();

	LockStrategy *pStrategy = dynamic_cast<LockStrategy *>(_pStrategy);
	if (pStrategy != nullptr)
	{
		pStrategy->setLockFish(fish);
	}
}



Fish* Bullet::getAimFish()
{
	if (_pStrategy == nullptr)
		return NULL;
	LockStrategy *pStrategy = dynamic_cast<LockStrategy *>(_pStrategy);
	if (pStrategy)
	{
		return  pStrategy->getLockFish();
	}
	else
	{
		return NULL;
	}
}

void Bullet::setRotation(float rotation)
{
	CollisionNode::setRotation(rotation);
	_oriCollisionPoints.clear();
	this->initCollisionPoint(Rect(_pColliSpr->getBoundingBox().origin, _pColliSpr->getBoundingBox().size));
}

bool Bullet::isOverScreen()
{
	
	Size screenSize = Director::getInstance()->getWinSize();
	float range = screenSize.width / 4;
	if (this->getPositionX() >= screenSize.width + range ||
		this->getPositionX() <= -range ||
		this->getPositionY() >= screenSize.height + range ||
		this->getPositionY() <= -range)
	{
		return true;
	}
	else
	{
		return false;
	}
}