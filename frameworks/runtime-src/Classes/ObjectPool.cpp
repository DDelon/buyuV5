#include "ObjectPool.h"

ObjectPool::ObjectPool()
{

}

ObjectPool::~ObjectPool()
{

}

void ObjectPool::initObjectPool(int poolNum)
{
	_nodePool = Array::create();
	_nodePool->retain();
	for (int i = 0; i < poolNum; i++)
	{
		Node *pNode = createPoolSingleNode();
		pNode->retain();
		_nodePool->addObject(pNode);
	}
}

Node *ObjectPool::getPoolSingleNode()
{
	for (int i = 0; i < _nodePool->count(); i++)
	{
		Node *pNode = dynamic_cast<Node *>(_nodePool->objectAtIndex(i));
		if (pNode && !isUse(pNode))
		{
			return pNode;
		}
	}

	//池对象不够了
	Node *pNode = createPoolSingleNode();
	pNode->retain();
	_nodePool->addObject(pNode);
	return pNode;

}

