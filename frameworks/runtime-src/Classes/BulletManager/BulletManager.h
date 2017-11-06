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

	//����˳���������ӵ�
	void clearBullet(int playerId);

	//�ӵ���ײ��յ������ӵ�
	void bulletCollision(std::string bulletId);

	//�������ӵ�����
	int getBulletCount(int playerId);
	//��������ӵ�
	ValueMap getLockBullet(int playerId);

	//�ı��ӵ�Ŀ��
	void changeBulletAim(int playerId, int timelineId, int fishArrayId);
	//�ı��ӵ�Ŀ��
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
	//�����߳�
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