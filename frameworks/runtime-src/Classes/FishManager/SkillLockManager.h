#ifndef __SKILLLOCKMANAGER_H__
#define __SKILLLOCKMANAGER_H__
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "Fish.h"

USING_NS_CC;

class SkillLockManager : public Node
{
public:
	static SkillLockManager *getInstance();
	bool init();
	void playLockChangeAim();
	void reset();

	void setLockLayer(Layer* lockLayer) { mLockLayer = lockLayer; }

	void setLuaNode(Node* bindNode, ValueMap data);

	void upDataLockSprAct(float aimPosX, float aimPosY);
	void setLockSpr(bool isShow);

	void startMyLock();
	void stopMyLock();
	void updateLock(float dt);

	void setAimFish(int playerId, Fish* aimFish);
	Fish* getAimFish(int playerId);

	void chooseFishByScore();

	ValueMap setLockData(ValueMap data);
	bool getIsLock(){ return nIsLock; };
private:
	static SkillLockManager *_lockInstance;
	bool nIsLock;
	std::map<int, Fish*> mAimFishMap;
	Layer* mLockLayer;
	Node* mLockNode;
	Sprite* mLockLoop;
	Sprite* mLockArrow;
	std::map<int, Sprite*> mChainMap;
	std::map<int, Action*> mActMap;
};

#endif