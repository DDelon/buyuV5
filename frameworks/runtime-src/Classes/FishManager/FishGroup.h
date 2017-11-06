#ifndef __FISHGROUP_H__
#define __FISHGROUP_H__


#include "cocos2d.h"
#include "FishArray.h"
USING_NS_CC;

class FishGroup : public Node
{
public:
	static FishGroup* craete(int groupId, int curFrame, Node* fisharrLayer, Node* fishLayer);
	bool init(int groupId, int curFrame, Node* fisharrLayer, Node* fishLayer);
	void actionInFrame(int frameIndex);
	void setBornFrame(int frame) { mBornFrame = frame; }
private:
	void doFrame(FishGroupData data);
	bool isArrayAlive();
	Node* mFisharrLayer;
	Node* mFishLayer;
	int mGroupId;
	int mBornFrame;
	std::vector<FishGroupData> mDataVec;


};


#endif