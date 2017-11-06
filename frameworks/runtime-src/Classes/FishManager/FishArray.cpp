#include "FishArray.h"
#include "ParseGameData.h"
#include "StringUtil.h"
#include "Fish.h"
#include "FishFrameManager.h"
FishArray* FishArray::craete(int arrId, int timelineId, int curFrame, int arrBornFrame, Node* parent)
{
	FishArray *pRet = new(std::nothrow) FishArray();
	if (pRet && pRet->init(arrId, timelineId, curFrame, arrBornFrame, parent))
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

bool FishArray::isExist(int arrId, int curFrame, int arrBornFrame)
{
	int configId = 310000000 + arrId * 1000;
	while (true)
	{
		string idStr = IntToString(configId);
		if (GAMEVALUE_STRING("fisharray", idStr, "fishid").empty())
		{
			break;
		}
		std::vector<Vec3>* pathPointVec;
		int fishId = GAMEVALUE_INT("fisharray", idStr, "fishid");
		//相对于鱼串的帧
		int fishBornFrame = GAMEVALUE_INT("fisharray", idStr, "frame");
		int pathId = GAMEVALUE_INT("fisharray", idStr, "trace");
		pathPointVec = FishFrameManager::getInstance()->getPath(pathId + 300000000);
		if (arrBornFrame + fishBornFrame + pathPointVec->size()*3 > curFrame)
		{
			//这条鱼还在屏幕上
			return true;
		}

		configId++;
	}
	return false;
}

bool FishArray::init(int arrId, int timelineId, int curFrame, int arrBornFrame, Node* parent)
{
	Node::init();
	mFishLayer = parent;
	mBornFrame = arrBornFrame;
	mTimelineId = timelineId;	//生成这条鱼串的timelineid
	int configId = 310000000 + arrId * 1000;
	while (true)
	{
		string idStr = IntToString(configId);
		if (GAMEVALUE_STRING("fisharray", idStr, "fishid").empty())
		{
			break;
		}
		int fishBornFrame = GAMEVALUE_INT("fisharray", idStr, "frame");
		if (fishBornFrame + mBornFrame > curFrame)
		{
			//还不用生成的鱼
			FishArrData data;
			data.isused = false;
			data.id = configId;
			data.fishid = GAMEVALUE_INT("fisharray", idStr, "fishid");
			data.frameIndex = fishBornFrame;
			data.startFrameIndex = fishBornFrame;
			data.pathId = GAMEVALUE_INT("fisharray", idStr, "trace");
			data.offsetX = GAMEVALUE_FLOAT("fisharray", idStr, "offsetx");
			data.offsetY = GAMEVALUE_FLOAT("fisharray", idStr, "offsety");
			mDataVec.push_back(data);
		}
		else
		{
			string idStr = IntToString(configId);
			if (GAMEVALUE_STRING("fisharray", idStr, "fishid").empty())
			{
				break;
			}
			std::vector<Vec3>* pathPointVec;
			int fishId = GAMEVALUE_INT("fisharray", idStr, "fishid");
			//相对于鱼串的帧
			int pathId = GAMEVALUE_INT("fisharray", idStr, "trace");
			pathPointVec = FishFrameManager::getInstance()->getPath(pathId + 300000000);
			if (mBornFrame + fishBornFrame + pathPointVec->size()*3 > curFrame)
			{
				FishArrData data;
				data.isused = false;
				data.id = configId;
				data.fishid = fishId;
				data.frameIndex = fishBornFrame;
				data.startFrameIndex = curFrame - mBornFrame;
				data.pathId = GAMEVALUE_INT("fisharray", idStr, "trace");
				data.offsetX = GAMEVALUE_FLOAT("fisharray", idStr, "offsetx");
				data.offsetY = GAMEVALUE_FLOAT("fisharray", idStr, "offsety");
				//这条鱼还在屏幕上
				doFrame(data);
			}
		}
		configId++;
	}

	return true;
}

void FishArray::actionInFrame(int frameIndex)
{
	bool allUsed = true;
	for (std::vector<FishArrData>::iterator iter = mDataVec.begin(); iter != mDataVec.end(); iter++)
	{
		if (!iter->isused &&iter->frameIndex == frameIndex - mBornFrame)
		{
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

void FishArray::doFrame(FishArrData data)
{
	//Fish* fish = Fish::create(data.fishid, data.pathId);
	Fish* fish = FishFrameManager::getInstance()->bornFish(data.fishid, data.pathId + 300000000, mTimelineId, data.id, data.startFrameIndex - data.frameIndex, Vec2(data.offsetX, data.offsetY));
	if (fish != nullptr)
	{
		fish->setBornFrame(mBornFrame + data.frameIndex);
		//Node *pFishNode = static_cast<Node *>(fish);
		mFishLayer->addChild(fish);
	}
	
}
