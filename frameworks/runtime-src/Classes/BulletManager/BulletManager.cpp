#include "BulletManager.h"
#include "StringUtil.h"
#include "FishFrameManager.h"
#include "ParseGameData.h"
#include "LuaCppAdapter.h"
#include "NetsManager.h"
#include "DebugLog.h"
#include "audio/include/SimpleAudioEngine.h"
#include "AudioControl.h"
#include "Collision/CollisionManager.h"

BulletManager *BulletManager::_pInstance = nullptr;

BulletManager *BulletManager::getInstance()
{
	if (_pInstance == nullptr)
	{
		_pInstance = new(std::nothrow) BulletManager();
		if (_pInstance && _pInstance->init())
		{
			_pInstance->retain();

		}
	}
	return _pInstance;
}

bool BulletManager::init()
{
	Collider::init();
	ObjectPool::initObjectPool(10);

	SpriteFrame *pNetFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName("net_1_00.png");
	_netSize = pNetFrame->getOriginalSize();

// 	auto animation = Animation::create();
// 	for (int i = 0; i < 4; i++)
// 	{
// 		animation->addSpriteFrameWithFile(StringUtil::format("battle/effect/gunfire_1_0%d.png", i));
// 	}
// 	animation->setDelayPerUnit(1.0f / 24.0f);
// 
// 	mFireAction = Sequence::create(Show::create(), Animate::create(animation), Hide::create(), NULL);
// 	mFireAction->retain();

	TableMap& tm = ParseGameData::getInstance()->getTabMap("cannonoutlook");
	TableMap::iterator it = tm.begin();
	while (it != tm.end()) {
		FieldMap fm = it->second;
		int i = fm["id"].asInt();
		if (fm["id"].asString().empty())
		{
			it++;
			continue;
		}
		string fire_effect = fm["fire_effect"].asString();
		if (fire_effect == "")
		{
			it++;
			continue;
		}
		auto animation = Animation::create();
		for (int i = 0; i < 4; i++)
		{
			string name = StringUtil::format("battle/effect/%s_0%d.png", fire_effect.c_str(), i);
			animation->addSpriteFrameWithFile(name);
		}
		animation->setDelayPerUnit(1.0f / 24.0f);

		Action* fireAction = Sequence::create(Show::create(), Animate::create(animation), Hide::create(), NULL);
		fireAction->retain();
		mFireActionMap[i] = fireAction;
		it++;
	}

	return true;
}

Node *BulletManager::createPoolSingleNode()
{
	Bullet* bullet = Bullet::create();
	return bullet;
}

void BulletManager::clearPool()
{
	for (int i = 0; i < _nodePool->count(); i++)
	{
		Bullet* bullet = dynamic_cast<Bullet*>(_nodePool->objectAtIndex(i));
		bullet->removeFromScreen();
	}
}

void BulletManager::start(Layer *pLayer, ValueMap data)
{
	_timelineId = 0;
	_fishArrayId = 0;
	_roomLv = 1;
	_bulletVec.clear();
	_pBulletLayer = pLayer;

	_playerId = LuaCppAdapter::getInstance()->getPlayerId();

	if (data.find("bullets") != data.end())
	{
		ValueVector existBulletVec = data["bullets"].asValueVector();
		syncOldBullet(existBulletVec);
	}

	this->openUpdate();
	//this->startCollisionThread();
}

void BulletManager::openUpdate()
{
	this->schedule(schedule_selector(BulletManager::updateBullet), 1.0f / 20.0f);
}

void BulletManager::updateBullet(float dt)
{
	//clock_t beginTime = clock();
	for (std::vector<Bullet *>::iterator it = _bulletVec.begin(); it != _bulletVec.end(); )
    {
		Bullet* bullet = *it;
		if (!bullet->isRemoved())
		{
			bullet->update();
			it++;
		}
		else
		{
			it = _bulletVec.erase(it);
			bullet->removeFromScreen();
		}
	}

	

	/*for (std::vector<Bullet *>::iterator it = _bulletVec.begin(); it != _bulletVec.end();)
	{
		Bullet* bullet = *it;
		if (bullet->isRemoved() && !bullet->isVisible())
		{
			it = _bulletVec.erase(it);
			bullet->removeFromScreen();
		}
		else
		{
			it++;
		}
	}*/
	CollisionManager::getInstance()->checkCollision();

}

void BulletManager::createBullet(std::string bulletId, int playerId, Vec2 launcherPos, float angle, int lifeTime, int timelineId, int fishArrayId, int rate, int effectId, bool isNeedAction)
{
	BulletInfo info;
	info.bulletId = bulletId;
	info.vipRes = LuaCppAdapter::getInstance()->getVIPResId(playerId);
	info.playerId = playerId;
	info.launcherPos = launcherPos;
	info.angle = angle;
	info.lifeTime = lifeTime;
	info.timelineId = timelineId;
	info.fishArrayId = fishArrayId;
	info.rate = rate;
	info.effectId = effectId;
	if (isNeedAction)
	{
		if (info.playerId == _playerId)
		{
			AudioControl::getInstance()->playEffect("sound/gunfire_01.mp3");
		}
		Node* cannon = getCannon(playerId);
		if (cannon != NULL && cannon->getParent()!= NULL)
		{
			cannon->stopAllActions();
			cannon->setPosition(Vec2(0, 38));
			
			float rotationRad = cannon->getParent()->getRotation()* M_PI / 180;
			float backDisX = CANNON_BACK_DIS* sin(rotationRad);
			float backDisY = CANNON_BACK_DIS* cos(rotationRad);

			cannon->runAction(Spawn::createWithTwoActions(
				Sequence::createWithTwoActions(MoveBy::create(0.05, Vec2(-backDisX, -backDisY)), MoveBy::create(0.05, Vec2(backDisX, backDisY))),
				Sequence::createWithTwoActions(ScaleTo::create(0.05, 1,0.8), ScaleTo::create(0.05, 1,1))
				));



			auto fireSprite = cannon->getChildByName("spr_gunfire");
			if (fireSprite)
			{
				string resId = LuaCppAdapter::getInstance()->getVIPResId(playerId);
				fireSprite->runAction(mFireActionMap[atoi(resId.c_str())]->clone());
			}
		}
	}
	
	Bullet *pBullet = dynamic_cast<Bullet *>(getPoolSingleNode());
	
	pBullet->setBulletInfo(info);
	_pBulletLayer->addChild(pBullet);
	_bulletVec.push_back(pBullet);
	this->addCollisionNode(pBullet);
}

void BulletManager::getPlayerBulletVec(int playerId, std::vector<Bullet *> &vec)
{
	for (int i = 0; i < _bulletVec.size(); ++i)
	{
		if (_bulletVec[i]->getPlayerId() == playerId && !_bulletVec[i]->isRemoved())
		{
			vec.push_back(_bulletVec[i]);
		}
	}
}

void BulletManager::syncOldBullet(ValueVector bulletVec)
{
	for (auto iter : bulletVec)
	{
		
		std::string bulletId = iter.asValueMap()["bulletId"].asString();
		int playerId = iter.asValueMap()["playerId"].asInt();
		float x = iter.asValueMap()["pos"].asValueMap()["x"].asFloat();
		float y = iter.asValueMap()["pos"].asValueMap()["y"].asFloat();
		Vec2 launcherPos(x, y);
		float angle = iter.asValueMap()["angle"].asFloat();
		int lifeTime = iter.asValueMap()["frameCount"].asInt();
		int timelineId = iter.asValueMap()["timelineId"].asInt();
		int fishArrayId = iter.asValueMap()["fishArrayId"].asInt();
		//
		createBullet(bulletId, playerId, launcherPos, angle, lifeTime, timelineId, fishArrayId, 0,false);
	}
}

void BulletManager::clearBullet(int playerId)
{
	for (int i = 0; i < _bulletVec.size(); ++i)
	{
		if (_bulletVec[i]->getPlayerId() == playerId)
		{
			_bulletVec[i]->signRemove();
		}
		
	}
}

void BulletManager::bulletCollision(std::string bulletId)
{
	for (int i = 0; i < _bulletVec.size(); ++i)
	{
		if (StringUtil::_STR_EQUAL(bulletId, _bulletVec[i]->getBulletId()))
		{
			ValueMap data;
			ValueMap pos;
			pos["x"] = _bulletVec[i]->getPositionX();
			pos["y"] = _bulletVec[i]->getPositionY();
			data["bulletPos"] = pos;
			//发送传递数据给lua
			LuaCppAdapter::getInstance()->callLuaFunc("FishGMF", "CppToLua", "otherBulletCollision", data);
			_bulletVec[i]->signRemove();
		}
	}
}


int BulletManager::getBulletCount(int playerId)
{
	int bulletCount = 0;
	for (int i = 0; i < _bulletVec.size(); ++i)
	{
		if (_bulletVec[i]->getPlayerId() == playerId && !_bulletVec[i]->isRemoved() 
			&& _bulletVec[i]->isVisible() && !_bulletVec[i]->isInvalid())
		{
			bulletCount++;
		}
	}
	return bulletCount;
}

ValueMap BulletManager::getLockBullet(int playerId)
{
	ValueMap map;
	int bulletCount = 0;
	for (int i = 0; i < _bulletVec.size(); ++i)
	{
		if (_bulletVec[i]->getPlayerId() == playerId && _bulletVec[i]->getTimelineId() > 0 && !_bulletVec[i]->isRemoved())
		{
			bulletCount++;
			string key = StringUtil::format("%d", bulletCount);
			map[key] = Value(_bulletVec[i]->getBulletId());
		}
	}
	return map;
}

void BulletManager::changeBulletAim(int playerId, int timelineId, int fishArrayId)
{
	auto mAimFish = FishFrameManager::getInstance()->getFishByID(timelineId, fishArrayId);
	if (mAimFish == nullptr)
	{
		return;
	}

	for (int i = 0; i < _bulletVec.size(); ++i)
	{
		if (_bulletVec[i]->isVisible()&& !_bulletVec[i]->isRemoved() &&_bulletVec[i]->getPlayerId() == playerId)
		{
			if (_bulletVec[i]->getTimelineId() > 0)
			{
				_bulletVec[i]->setAimFish(mAimFish);
			}
		}
	}
}

void BulletManager::changeBulletAim(int playerId, Fish*aimFish)
{
	if (aimFish == nullptr)
	{
		return;
	}

	for (int i = 0; i < _bulletVec.size(); ++i)
	{
		if (_bulletVec[i]->isVisible() && !_bulletVec[i]->isRemoved() && _bulletVec[i]->getPlayerId() == playerId)
		{
			if (_bulletVec[i]->getTimelineId() > 0)
			{
				_bulletVec[i]->setAimFish(aimFish);
			}
		}
	}
}

ValueMap BulletManager::dealCollision(CollisionNode *pOther)
{
	return ValueMap();
}

void BulletManager::reset()
{
	_timelineId = 0;
	_fishArrayId = 0;
	_bulletVec.clear();
	_CannonMap.clear();
	_AimFishMap.clear();
	this->unschedule(schedule_selector(BulletManager::updateBullet));
}


Bullet* BulletManager::getBulletById(string bulletId)
{
	for (int i = 0; i < _bulletVec.size(); ++i)
	{
		if (_bulletVec[i]->getBulletId() == bulletId && !_bulletVec[i]->isRemoved())
		{
			return _bulletVec[i];
		}
	}
	return NULL;
}

void BulletManager::addPlayerCannon(int playerId, Node* cannon)
{
	_CannonMap[playerId] = cannon;
}

void BulletManager::delPlayerCannon(int playerId)
{
	if (playerId == -1)
	{
		_CannonMap.clear();
	}
	else
	{
		auto iter = _CannonMap.find(playerId);
		if (iter != _CannonMap.end())
		{
			_CannonMap.erase(iter);
		}
	}
	
}

Node* BulletManager::getCannon(int playerId)
{
	auto iter = _CannonMap.find(playerId);
	if (iter != _CannonMap.end())
	{
		return iter->second;
	}
	return NULL;
}

