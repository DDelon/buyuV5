#include "FishTimeline.h"
#include "StringUtil.h"
#include "ParseGameData.h"
#include "Fish.h"
#include "FishFrameManager.h"
FishTimeline* FishTimeline::craete(int room, int num, bool isServer, int curFrame, Node* fishLayer, Node* fishArrLayer)
{
	FishTimeline *pRet = new(std::nothrow) FishTimeline();
	int arrId = 320000000 + room*100000 + (isServer ? 9 : 0) * 10000 + num * 1000;//320101000
	if (pRet && pRet->init(arrId, curFrame, fishLayer, fishArrLayer))
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		delete pRet;
		pRet = nullptr;
		return nullptr;
	}
}

bool FishTimeline::init(int arrId, int curFrame, Node* fishLayer, Node* fishArrLayer)
{
	Node::init();
	mFishLayer = fishLayer;
	mFishArrLayer = fishArrLayer;
	mBornFrame = 0;

	while (true)
	{
		string idStr = IntToString(arrId);
		if (GAMEVALUE_STRING("timeline", idStr, "fishid").empty())
		{
			break;
		}
		int bornFrame = GAMEVALUE_INT("timeline", idStr, "frame");
		if (bornFrame > curFrame)
		{
			//还未生成的鱼或鱼串
			TimelineData data;
			data.isused = false;
			data.id = arrId;
			data.fishid = GAMEVALUE_INT("timeline", idStr, "fishid");
			data.frameIndex = GAMEVALUE_INT("timeline", idStr, "frame");
			data.pathId = GAMEVALUE_INT("timeline", idStr, "pathid");
			data.startFrameIndex = data.frameIndex;

			mDataVec.push_back(data);
		}
		else
		{
			//已经生成的鱼串
			int fishId = GAMEVALUE_INT("timeline", idStr, "fishid");
			if (fishId == 100)
			{
				int fishArrayId = GAMEVALUE_INT("timeline", idStr, "pathid");
				int arrBornFrame = GAMEVALUE_INT("timeline", idStr, "frame");
				//鱼组
				if (FishArray::isExist(fishArrayId, curFrame, arrBornFrame))
				{
					TimelineData data;
					data.fishid = fishId;
					data.isused = false;
					data.id = arrId;
					data.frameIndex = arrBornFrame;
					data.startFrameIndex = curFrame;
					data.pathId = fishArrayId;

					//立即创建
					doFrame(data);
				}
			}
			else
			{
				int pathId = GAMEVALUE_INT("timeline", idStr, "pathid");
				int bornFrame = GAMEVALUE_INT("timeline", idStr, "frame");
				std::vector<Vec3>* pathPointVec;
				
				pathPointVec = FishFrameManager::getInstance()->getPath(pathId + 300000000);
				if (bornFrame + pathPointVec->size() * 3 > curFrame)
				{
					//单独鱼
					TimelineData data;
					data.isused = false;
					data.id = arrId;
					data.fishid = fishId;
					data.frameIndex = bornFrame;
					data.pathId = pathId;
					data.startFrameIndex = curFrame;
					doFrame(data);
				}
			}
		}

		
		arrId++;
	}
	log("---------------------------------------mDataVec=%d", mDataVec.size());
	return true;
}

void FishTimeline::actionInFrame(int frameIndex)
{
	bool allUsed = true;
	for (std::vector<TimelineData>::iterator iter = mDataVec.begin(); iter != mDataVec.end(); iter++)
	{
		if (!iter->isused && iter->frameIndex == frameIndex)
		{
			//log("%d", iter->id);
			doFrame(*iter);
			iter->isused = true;
		}
		if (!iter->isused)
		{
			allUsed = false;
		}
	}
	if (allUsed)
	{
		this->runAction(RemoveSelf::create());
	}
}

void FishTimeline::doFrame(TimelineData data)
{
	if (data.fishid==100)
	{
		FishFrameManager::getInstance()->addFishArr(data.pathId, data.id, data.startFrameIndex, data.frameIndex);
		//CCLOG("%d: %d==========Fish Array:%d", data.frameIndex,data.id , data.pathId);
	}
	else
	{
		//Fish* fish = Fish::create(data.fishid, data.pathId);
		Fish* fish = FishFrameManager::getInstance()->bornFish(data.fishid, data.pathId + 300000000, data.id, 0, data.startFrameIndex - data.frameIndex);
		if (fish != nullptr)
		{
			fish->setBornFrame(data.frameIndex);
			if (mFishLayer != nullptr && mFishLayer->getParent() != nullptr)
			{
				mFishLayer->addChild(fish);
			}
			//CCLOG("%d: %d==========Fish :%d", data.frameIndex, data.id, data.fishid);
		}
	}
}
