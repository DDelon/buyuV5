#ifndef __OBJECT_POOL_H__
#define __OBJECT_POOL_H__
#include "cocos2d.h"
USING_NS_CC;

class ObjectPool
{
protected:
	ObjectPool();
	virtual ~ObjectPool();

	virtual void initObjectPool(int poolNum);

	//创建池储存的单个节点
	virtual Node *createPoolSingleNode() = 0;

	//获取节点
	virtual Node *getPoolSingleNode();

	//判断节点是否被使用
	virtual bool isUse(Node *pNode) = 0;

	//清除节点
	virtual void clearPool() = 0;

protected:
	Array* _nodePool;
};

#endif