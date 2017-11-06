#include "Collider.h"
#include "CollisionNode.h"
#include "DebugLog.h"
#include "Collision/CollisionManager.h"
#include "FishFrameManager.h"
#include "NetsManager.h"
#include "LuaCppAdapter.h"
#include "CollisionManager.h"


bool Collider::init()
{
	bool bRet = false;
	do 
	{
		CC_BREAK_IF(!Node::init());
		bRet = true;
	} while (false);

	return bRet;
}

void Collider::addCollisionNode(CollisionNode *pCollisionNode)
{
	_collisionNodeVec.push_back(pCollisionNode);
}

void Collider::clearInvalidNode()
{
	for (auto iter = _collisionNodeVec.begin(); iter != _collisionNodeVec.end();)
	{
		if ((*iter)->isRemove())
		{
			(*iter)->clearCollData();
			iter = _collisionNodeVec.erase(iter);
		}
		else
		{
			iter++;
		}
	}
}

void Collider::colliderWithCollision(Collider *pCollider)
{
	auto collisionFunc = [&](CollisionNode *iter1, CollisionNode *iter2){
		//删除子弹 撒渔网
		//判断哪些鱼在渔网范围内
		//发送在渔网范围内的鱼的数据给lua

		ValueMap info1 = iter1->dealNodeCollision(iter2);
		ValueMap info2 = iter2->dealNodeCollision(iter1);
		mergeValueMap(info1, info2);
		//选取鱼
		ValueMap info3 = pCollider->dealCollision(iter1);
		mergeValueMap(info1, info3);
		ValueMap info4 = this->dealCollision(iter2);
		mergeValueMap(info1, info4);

		if (info1["isMy"].asBool())
		{
			Director::getInstance()->getScheduler()->performFunctionInCocosThread([=](){
				std::string bulletId = "";
				ValueVector collFishVec;
				auto iter = info1.find("bulletId");
				if (iter != info1.end())
				{
					bulletId = iter->second.asString();
				}

				iter = info1.find("collisionFishTab");
				if (iter != info1.end())
				{
					collFishVec = iter->second.asValueVector();
				}

				CollisionManager::getInstance()->insertFishVec(bulletId, collFishVec);
				LuaCppAdapter::getInstance()->callLuaFunc("FishGMF", "CppToLua", "sendNetCollision", info1);
			});
		}
	};

	//子弹碰撞鱼 参数为鱼碰撞层
	std::vector<CollisionNode *> &copyColliderNodeVec1 = _collisionNodeVec;
	std::vector<CollisionNode *> &copyColliderNodeVec2 = pCollider->getCollisionNode();
	//clock_t beginTime = clock();
	if (copyColliderNodeVec1.size() == 0 || copyColliderNodeVec2.size() == 0)
		return;

	for (int i = 0; i < copyColliderNodeVec1.size(); ++i)
	{
		if (copyColliderNodeVec1[i]->isRemove())
			continue;
		if (copyColliderNodeVec1[i]->isCanCheck())
		{
			for (int j = 0; j < copyColliderNodeVec2.size(); ++j)
			{
				if (copyColliderNodeVec2[j]->isRemove())
					continue;
				if (copyColliderNodeVec2[j]->isCanCheck())
				{

					if (copyColliderNodeVec1[i]->isUseCollAlgori() && copyColliderNodeVec2[j]->isUseCollAlgori())
					{

						if (copyColliderNodeVec1[i]->obbCollisionWithNode(copyColliderNodeVec2[j]))
						{
							collisionFunc(copyColliderNodeVec1[i], copyColliderNodeVec2[j]);
						}
					}
					else
					{
						//锁定子弹检测距离
						if (copyColliderNodeVec1[i]->customCollision() && copyColliderNodeVec2[j]->customCollision())
						{
							collisionFunc(copyColliderNodeVec1[i], copyColliderNodeVec2[j]);
						}
					}
				}
			}
		}
		
	}

	//clock_t endTime = clock();
	//if ((endTime - beginTime) / 1000 * 1000 > 1000)
	//{
	//	log("collision running time %d", (endTime - beginTime) / 1000 * 1000);
	//}
}

void Collider::mergeValueMap(ValueMap &map1, ValueMap &map2)
{

	for (auto iter : map2)
	{
		map1[iter.first] = iter.second;
	}
}

void Collider::clearData()
{
	_collisionNodeVec.clear();
}
