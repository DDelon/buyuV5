#ifndef _FSHIFRAMEMANAGER_H__
#define _FSHIFRAMEMANAGER_H__

#include "Fish.h"
#include "FishArray.h"
#include "Collision/Collider.h"
#include "ObjectPool.h"
#define FISH_TAG 10000
#define FRAME_DELAY_VAL 20

class FishFrameManager : public Collider, public ObjectPool
{
public:
	static FishFrameManager* getInstance();
	CREATE_FUNC(FishFrameManager);

	int getCurrentFrame() { return mCurrentFrame; }
	int getSubFrame() { return mSubFrame;  }
	bool init();
	void start();
	void stop();
	void setFishLayer(Layer* node) { mFishLayer = node; }
	Layer *getFishLayer() { return mFishLayer;  }
	void addFishArr(int arrId, int timelineId, int startFrameIndex, int arrBornFrame);
	void addFishTimeline(int timelineId, int frame = 0);
	void addFishGroup(int groupId, int frame = 0);
	void addKilledFishes(ValueVector &killedFishes) { _killedFished = killedFishes; }
	void addCalledFish(ValueVector fishesInfo);
	void reset();
	void pauseFish();
	void setRoomLv(int roomLv) { mRoomLv = roomLv;  }
	void setIsFrozen(bool isFrozen) { mIsFrozen = isFrozen;  }

	Vector<Node *> &getAllFish() { return mFishLayer->getChildren(); }

	void addPath(int id,std::vector<Vec3> pathPointVec);
	std::vector<Vec3>* getPath(int id);

	Vec3 getPathById(int timelineId, int fishArrayId, int frame);

	Fish* bornFish(int id,int pathId, int timelineId, int fishArrayId, int frame, Vec2 offset = Vec2(0, 0),bool isCallFish = false);

	void pauseAllFish(int type);
	void resumeAllFish();
	bool getIsFrozen() { return mIsFrozen;  }

	void moveToOut();

	//
	void setFilpVal(bool val) { mIsFilp = val; }
	bool getFilpVal() { return mIsFilp; }

	//心跳同步
	void syncFrame(ValueMap data);
	void setMyAimFish(int timelineId, int fishArrayId);
	Fish* getMyAimFish();

	Fish* getLockFishByScore();
	Fish* getFishByPos(Vec2 pos);
	Fish* getFishByID(int timelineId, int fishArrayId);

	void setFishRed(int timelineId, int fishArrayId);
	void setFishDeath(int timelineId, int fishArrayId);

	ValueVector getKilledFishesVec() { return _killedFished;  }
	void clearKilledFishesVec() { _killedFished.clear(); }

	//特殊鱼
	void getEffectedFishes(int poolValue, int price, int bulletRate, std::vector<Fish *> &waitSelectedFishes, std::vector<Fish *> &effectedFishes);
	void getThunderPool(int bulletRate, std::vector<Fish *> &effectedFishes);
	void getSameFishType(int fishType, std::vector<Fish *> &effectedFishes);
	void getRangeFish(int range, Vec2 pos, std::vector<Fish *> &effectedFishes);
	void getNBombRangeFish(int range, Vec2 pos, std::vector<Fish *> &effectedFishes);
	void getNBombFish(int bombLv, Vec2 pos, std::vector<Fish *> &effectedFishes);

	void setThunderRate(int thunderRate) { mThunderRate = thunderRate;  }
	int getThunderRate() { return mThunderRate; }

	void setNBombRate(int nBombRate) { mNBombRate = nBombRate;  }
	int getNBombRate() { return mNBombRate; }

	void onHit(ValueMap data);
	int onFishHit(Fish* fish, ValueMap data);

	void showLockPointUI();


	//--------------------------Collider-------------------------------------
	virtual ValueMap dealCollision(CollisionNode *pOther);

protected:
	//ObjectPool
	virtual Node *createPoolSingleNode();
	virtual bool isUse(Node *pNode) { return dynamic_cast<Fish *>(pNode)->isAlive(); }
	virtual void clearPool();

private:
	void updateFish(float dt);

	void getRandIndexTab(std::vector<int> &indexVec, int maxNum);
	
	int mCurrentFrame;
	int mSubFrame;	//客户端和服务器相差帧数
	Layer* mFishLayer;
	Layer* mFishArrLayer;
	Layer* mFishTimelineLayer;
	Layer* mFishGroupLayer;

	PathMap mPathMap;

	ValueVector _killedFished;

	std::vector<Fish *> _collFishVec;

	bool mIsPause;
	bool mIsFrozen;

	Fish* myAimFish;
	bool mIsFilp;
	int mThunderRate;
	int mNBombRate;
	int mRoomLv;

};


#endif