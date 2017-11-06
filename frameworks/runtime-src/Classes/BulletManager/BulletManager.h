#ifndef __BULLET_MANAGER_H__
#define __BULLET_MANAGER_H__
#include "Bullet.h"
#include "WaitNotifyThread.h"
#include "Collision/Collider.h"
#include "ObjectPool.h"

#define  CANNON_BACK_DIS  0
class BulletManager : public Collider, public ObjectPool
{
public:
	static BulletManager *create(ValueMap data);
	static BulletManager *getInstance();
	bool init();
	void start(Layer *pBulletLayer, ValueMap data);
	void reset();

	void createBullet(std::string bulletId, int playerId, Vec2 launcherPos, float angle, int lifeTime, int timelineId, int fishArrayId, int rate, int effectId = 0, bool isNeedAction = true);
	
	void openUpdate();

	void getPlayerBulletVec(int playerId, std::vector<Bullet *> &vec);

	//玩家退出清空所有子弹
	void clearBullet(int playerId);

	//子弹碰撞清空单独的子弹
	void bulletCollision(std::string bulletId);

	//获得玩家子弹数量
	int getBulletCount(int playerId);
	//获得特殊子弹
	ValueMap getLockBullet(int playerId);

	//改变子弹目标
	void changeBulletAim(int playerId, int timelineId, int fishArrayId);
	//改变子弹目标
	void changeBulletAim(int playerId, Fish*aimFish);


	ValueVector * getCollisionFishes(string bulletId);

	Bullet* getBulletById(string bulletId);

	void addPlayerCannon(int playerId,Node* cannon);
	void delPlayerCannon(int playerId);
	Node* getCannon(int playerId);

	//--------------------------Collider-------------------------------------
	virtual ValueMap dealCollision(CollisionNode *pOther);
protected:
	//ObjectPool
	virtual Node *createPoolSingleNode();
	virtual bool isUse(Node *pNode) { return dynamic_cast<Bullet *>(pNode)->isUse();  }
	virtual void clearPool();

private:
	//开启线程
	void syncOldBullet(ValueVector bulletVec);
	void updateBullet(float dt);

	
private:
	static BulletManager *_pInstance;
	int _timelineId;
	int _fishArrayId;
	int _playerId;
	int _roomLv;
	Layer *_pBulletLayer;
	std::vector<Bullet *> _bulletVec;

	Size _netSize;
	std::map<int,Node*> _CannonMap;

	std::map<int, Fish*> _AimFishMap;

	//Action* mFireAction;
	map<int, Action*> mFireActionMap;
};

#endif