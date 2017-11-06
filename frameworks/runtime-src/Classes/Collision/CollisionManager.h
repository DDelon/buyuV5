#ifndef __COLLISION_MANAGER_H__
#define __COLLISION_MANAGER_H__

#include "Collision/BGThread.h"
class WaitNotifyThread;
class Collider;

class CollisionManager
{
protected:
	CollisionManager();
	~CollisionManager();

public:
	static CollisionManager *getInstance();

	void init(Collider *pCollider1, Collider *pCollider2);
	
	void checkCollision();

	WaitNotifyThread *getThread() { return _pCollisionThread;  }

	void insertFishVec(std::string bulletId, ValueVector collFishInfoVec);

	ValueVector *getCollFishVec(std::string bulletId);

	void clearData();
private:
	static CollisionManager *_pInstance;

	WaitNotifyThread *_pCollisionThread;

	Collider *_pCollider1;
	Collider *_pCollider2;

	//客户端本地缓存的子弹碰撞数据
	ValueMap _collisionValueMap;
};

#endif