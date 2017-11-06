#ifndef __COLLIDER_H__
#define __COLLIDER_H__
#include "cocos2d.h"
USING_NS_CC;

class CollisionNode;

class Collider : public Node
{
public:
	virtual bool init();

public:
	virtual void addCollisionNode(CollisionNode *pCollisionNode);

	virtual void colliderWithCollision(Collider *pCollider);

	virtual std::vector<CollisionNode *> &getCollisionNode() { return _collisionNodeVec; }

	virtual void clearInvalidNode();

	virtual ValueMap dealCollision(CollisionNode *pOther) = 0;

	void clearData();

private:
	//�ϲ�����ValueMap �ڶ����ϵ���һ��
	void mergeValueMap(ValueMap &map1, ValueMap &map2);

protected:
	std::vector<CollisionNode *> _collisionNodeVec;

	float _radius;

};

#endif