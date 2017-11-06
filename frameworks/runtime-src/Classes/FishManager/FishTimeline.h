#ifndef __FISHTIMELINE_H__
#define __FISHTIMELINE_H__


#include "cocos2d.h"
#include "Fish.h"
USING_NS_CC;

class FishTimeline : public Node
{
public:
	static FishTimeline* craete(int room, int num, bool isServer, int curFrame, Node* fishLayer, Node* fishArrLayer);
	bool init(int arrId, int curFrame, Node* fishLayer, Node* fishArrLayer);
	void actionInFrame(int frameIndex);
	void setBornFrame(int frame) { mBornFrame = frame; }

private:
	void doFrame(TimelineData data);
	int mBornFrame;
	Node* mFishLayer;
	Node* mFishArrLayer;

	std::vector<TimelineData> mDataVec;
};

#endif