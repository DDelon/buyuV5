#ifndef __BULLET_H__
#define __BULLET_H__
#include "cocos2d.h"
#include "Fish.h"
#include "Collision/CollisionNode.h"
#include "BStrategyBase.h"
USING_NS_CC;
#define EXTRA_ANGLE 90
#define BULLET_MOVEING_TAG 0x100
#define BULLET_ROTATE_AIM_TAG 0x200
struct BulletInfo
{
	std::string bulletId;
	std::string vipRes;
	int playerId;
	Vec2 launcherPos;
	float angle;
	int lifeTime;
	int timelineId;
	int fishArrayId;
	int rate;
	int effectId;
};

enum BulletStatus
{
	NORMAL,
	RUN,
	REMOVE,
};

class Bullet : public CollisionNode
{
public:

	virtual bool init();

	CREATE_FUNC(Bullet);

	void setBulletInfo(BulletInfo info);

	const BulletInfo &getBulletInfo() { return _info;  }

	std::string getBulletId() { return _info.bulletId; }
	int getPlayerId() { return _info.playerId;  }
	int getRate() { return _info.rate; }
	virtual Rect getBoundingBox();
	bool isUse() { return !(_status == BulletStatus::NORMAL); }
	int getTimelineId() { return _info.timelineId; }
	int getFishArrayId() { return _info.fishArrayId; }
	std::string getVipResId() { return _info.vipRes; }

	void update();
	void signRemove() { _status = BulletStatus::REMOVE; }

	void delaySignRemove(int removeFrame) { _delayRemoveFrame = removeFrame;  }
	int getDelaySignRemove(){ return _delayRemoveFrame; }
	bool isRemoved() { return (_status == BulletStatus::REMOVE); }
	bool isInvalid() { return _pStrategy->isInvalid(); }

	void removeFromScreen();
	//子弹追踪
	
	void setAimFish(int timelineId, int fishArrayId);
	void setAimFish(Fish* fish);
	Fish* getAimFish();
	void setRotation(float rotation);
	
	//--------------------------CollisionNode-------------------------------------
	virtual bool isUseCollAlgori();
	virtual ValueMap dealNodeCollision(CollisionNode *pOther);
	virtual bool isCanCheck() { return (_status == BulletStatus::RUN ? true : false); }
	virtual bool customCollision();
protected:
	virtual void updatePoints();

	bool isOverScreen();

protected:
	void initSprite(std::string resId);

	void initEffect(int effectId);

protected:
	static int _count;
	BStrategyBase *_pStrategy;	//普通子弹和特殊子弹的区分类
	Sprite *_pSprite;
	Sprite *_pColliSpr;
	BulletStatus _status;

	BulletInfo _info;
	int _speed;
	int _delayRemoveFrame;	//延时删除变量 记录延时几帧删除
	bool _isInvalid;

	
};

#endif