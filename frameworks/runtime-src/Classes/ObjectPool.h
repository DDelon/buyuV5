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

	//�����ش���ĵ����ڵ�
	virtual Node *createPoolSingleNode() = 0;

	//��ȡ�ڵ�
	virtual Node *getPoolSingleNode();

	//�жϽڵ��Ƿ�ʹ��
	virtual bool isUse(Node *pNode) = 0;

	//����ڵ�
	virtual void clearPool() = 0;

protected:
	Array* _nodePool;
};

#endif