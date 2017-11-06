#ifndef __FISHARRAY_H__
#define __FISHARRAY_H__


#include "cocos2d.h"
#include "Fish.h"
USING_NS_CC;

class FishArray : public Node
{
public:
	static FishArray* craete(int arrId, int timelineId, int curFrame, int arrBornFrame, Node* fishLayer);
	static bool isExist(int arrId, int curFrame, int arrBornFrame);
	bool init(int arrId, int timelineId, int curFrame, int arrBornFrame, Node* parent);
	void actionInFrame(int frameIndex);
	void setBornFrame(int frame) { mBornFrame = frame; }
private:
	void doFrame(FishArrData data);
	Node* mFishLayer;
	int mArrId;
	int mBornFrame;
	int mTimelineId;
	std::vector<FishArrData> mDataVec;

};

#endif // !__FISHARRAY_H__
