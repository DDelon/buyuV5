#ifndef __FISH_H__
#define __FISH_H__



#include "Collision/CollisionNode.h"
#define FISH_ID_START 100000000
#define STANDING_ACTION_TAG 0x100
#define MOVEING_ACTION_TAG  0x200
#define SHADOW_TAG 0x300
#define DISC_ACTION_TAG 0x400
#define HALO_ACTION_TAG 0x500

#include <unordered_map>
#include "cocos2d.h"
USING_NS_CC;
using namespace std;
#define PATH_INTERVAL 3
#define BASE_RESOLUTION_WIDTH 1280
#define BASE_RESOLUTION_HEIGHT 720

struct FishArrData
{
	bool isused;
	int id;
	int fishid;
	int frameIndex;
	int startFrameIndex;
	int pathId;
	float offsetX;
	float offsetY;
};

struct TimelineData
{
	bool isused;
	int id;
	int fishid;
	int frameIndex;
	int startFrameIndex;
	int pathId;
};

struct FishGroupData
{
	bool isused;
	int id;
	int fisharrid;
	int frameIndex;
	int startFrameIndex;
	int endFrameIndex;
};

enum FishState
{
	FADE = 0,
	MOVE = 1,
	PAUSE = 2,
	DEATH = 3,
	FREEZE = 4,
	START_FREEZE = 5,
	ACCELERATE = 6,
	ELECTRICED = 7,

};

typedef unordered_map<int, std::vector<Vec3>> PathMap;
typedef unordered_map<int, int> FishIdMap;
typedef unordered_map<int, FishArrData> FishArrMap;
typedef unordered_map<int, TimelineData> TimelineMap;
typedef unordered_map<int, FishGroupData> FishGroupMap;

class Fish : public CollisionNode
{
public:
	static Fish* create();
	bool init();
	void setFishId(int fishId, int timelineId, int fishArrayId, bool isCallFish = false);
	void setPathId(int pathId, int frame);
	Vec3 getPathPoint(int frame);

	void actionInFrame(int frameIndex);
	void setOffset(Vec2 offset);

	void setBornFrame(int frame) { 
		mBornFrame = frame; }
	int getBornFrame() { return mBornFrame; }

	int getTraceType() { return mFishType; }
	bool isSpecialFish();

	bool isAlive() { return mIsAlive; }
	void alive() { mIsAlive = true; }
	void removeFromScreen();

	int getTimelineId() { return mTimelineId;  }
	int getFishArrayId() { return mFishArrayId; }
	int getId() { return mId; }

	void setFishState(int state);
	int getFishState(){ return mFishState; }
	int getScore(){ return mScore; }
	int getDeathEffect(){ return mDeathEffect; }
	int getDropCoin(){ return mCoinNum; }

	void setFishColor();

	void setIsRed(bool isRed);
	void specialFishDeath(Vec2 startPos, int specialType, bool isPlayAni = true);
	void addThunderStartPoint();
	void setFishDeath();
	void accelerate();

	void getEffectedFishes(int bulletRate, ValueVector &fishTab);

	void reset();

	Vec2 getLockPosition();

	//其他人的锁定点
	Vec2 getOtherLockPosition();

	//选取一个锁定点缓存起来
	void selectLockPoint(Vec2 pos);

	void addLockPointUI();
	
	//--------------------------CollisionNode-------------------------------------
	virtual bool isCanCheck();
	virtual bool isCollision() { return false; }
	virtual bool isUseCollAlgori() { return true; }
	virtual ValueMap dealNodeCollision(CollisionNode *pOther);
	
private:
	Action* initAnimation(std::string fileName,float delay);

	Size getGroupFishContentSize();

	std::vector<Vec2> getCollisionVecByStr(std::string str);

	void fadeIn();

	void fadeOut();
	//加速
	void setAccelerate(float delay);
	//爆炸
	void setBlastEff(float delay);
	//爆金币
	void setBlastCoinEff(float delay);
	//爆光圈
	void setBlastCircleEff(float delay);
	//背景震动
	void setShakeBgEff(float delay);

	void lockPointFlipX();

	void lockPointFlipY();

	

	int mId;
	int mSameFishId;//同类炸弹的id
	int mRoatateType;
	bool mIsFliped;
	int mCurRunFrame;
	int mStartAcceFrame;
	Vec2 mPathOffset;
	Vec2 mLockOffset;
	Vec2 mOtherLockOffset;
	std::vector<Vec3>* mPathPoint;
	std::vector<Vec2> mHandAimPoint;
	std::vector<Sprite*> mSpriteVec;
	std::vector<Sprite*> mDiscVec;
	std::vector<Sprite*> mHaloVec;
	std::vector<Sprite*> mLockVec;

	Sprite* mSpriteShadow;

/*	DeathBase *_deathEffect;*/

	int mBornFrame;
	int mTimelineId;
	int mFishArrayId;
	bool mIsAlive;

	int mFishState = 0;

	bool mIsHit = false;
	int mScore;
	int mDeathEffect;
	string mMusic;
	int mPlayMusicRate;
	int mSpeed;
	int mCoinNum;
	int mFishType;

	bool mIsCallFish;
};


#endif // !__FISH_H__
