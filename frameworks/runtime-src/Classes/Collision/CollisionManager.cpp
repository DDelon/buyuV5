#include "CollisionManager.h"
#include "BGThread.h"
#include "Collider.h"
#include "DebugLog.h"
#include "WaitNotifyThread.h"

CollisionManager *CollisionManager::_pInstance = nullptr;
CollisionManager *CollisionManager::getInstance()
{
	if (_pInstance == nullptr)
	{
		_pInstance = new CollisionManager();
	}

	return _pInstance;
}

CollisionManager::CollisionManager()
{
	_pCollisionThread = new WaitNotifyThread();
	_pCollider1 = nullptr;
	_pCollider2 = nullptr;

}


CollisionManager::~CollisionManager()
{
}

void CollisionManager::init(Collider *pCollider1, Collider *pCollider2)
{
	Task task = [&]()->void{
		_pCollider1->clearInvalidNode();
		_pCollider2->clearInvalidNode();
		_pCollider1->colliderWithCollision(_pCollider2);
	};
	_pCollider1 = pCollider1;
	_pCollider2 = pCollider2;
	_pCollisionThread->start(task);
}

void CollisionManager::checkCollision()
{
	_pCollisionThread->notify();
}

void CollisionManager::insertFishVec(std::string bulletId, ValueVector collFishInfoVec)
{

	_collisionValueMap.clear();
	_collisionValueMap[bulletId] = collFishInfoVec;
}

ValueVector *CollisionManager::getCollFishVec(std::string bulletId)
{
	auto iter = _collisionValueMap.find(bulletId);
	if (iter != _collisionValueMap.end())
	{
		return &(*iter).second.asValueVector();
	}
	return NULL;

}

void CollisionManager::clearData()
{
	_collisionValueMap.clear();

	if (_pCollider1 != nullptr)
	{
		_pCollider1->clearData();
		_pCollider1 = nullptr;
	}
	
	if (_pCollider2 != nullptr)
	{
		_pCollider2->clearData();
		_pCollider2 = nullptr;
	}

	_pCollisionThread->stop();
}

