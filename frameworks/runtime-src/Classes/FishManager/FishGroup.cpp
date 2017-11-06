#include "FishGroup.h"
#include "ParseGameData.h"
#include "StringUtil.h"
#include "FishArray.h"
FishGroup* FishGroup::craete(int groupId, int curFrame, Node* fisharrLayer, Node* fishLayer)
{
	FishGroup *pRet = new(std::nothrow) FishGroup();
	if (pRet && pRet->init(groupId, curFrame, fisharrLayer, fishLayer))
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

bool FishGroup::init(int groupId, int curFrame, Node* fisharrLayer, Node* fishLayer)
{
	Node::init();
	mGroupId = groupId;
	mFisharrLayer = fisharrLayer;
	mFishLayer = fishLayer;
	mBornFrame = 0;

	int configId = 330000000 + groupId * 100000;

	while (true)
	{
		string idStr = IntToString(configId);
		if (GAMEVALUE_STRING("fishgroup", idStr, "arrId").empty())
		{
			break;
		}
		int arrBornFrame = GAMEVALUE_INT("fishgroup", idStr, "frame");
		int fishArrayId = GAMEVALUE_INT("fishgroup", idStr, "arrId");
		if (arrBornFrame > curFrame)
		{
			//还未生成的鱼串
			FishGroupData data;
			data.isused = false;
			data.id = configId;
			data.frameIndex = arrBornFrame;
			data.startFrameIndex = arrBornFrame;
			data.fisharrid = fishArrayId;
			data.endFrameIndex = GAMEVALUE_INT("fishgroup", idStr, "endframe");
			mDataVec.push_back(data);
		}
		else
		{
			//已经生成的鱼串
			if (FishArray::isExist(fishArrayId, curFrame, arrBornFrame))
			{
				FishGroupData data;
				data.isused = false;
				data.id = configId;
				data.frameIndex = arrBornFrame;
				data.startFrameIndex = curFrame;
				data.fisharrid = fishArrayId;
				data.endFrameIndex = GAMEVALUE_INT("fishgroup", idStr, "endframe");

				//立即创建
				doFrame(data);
			}
		}
		
		configId++;
	}

	return true;
}

void FishGroup::actionInFrame(int frameIndex)
{
	bool allUsed = true;
	for (std::vector<FishGroupData>::iterator iter = mDataVec.begin(); iter != mDataVec.end(); iter++)
	{
		if (!iter->isused && iter->frameIndex == frameIndex - mBornFrame)
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

void FishGroup::doFrame(FishGroupData data)
{
	FishArray* fishArr = FishArray::craete(data.fisharrid, data.id, data.startFrameIndex, mBornFrame + data.frameIndex, mFishLayer);
	//fishArr->setBornFrame(mBornFrame + data.frameIndex);
	mFisharrLayer->addChild(fishArr);

	//CCLOG("%d: %d =============Array:%d", data.frameIndex, data.id, data.fisharrid);
}
