#ifndef __COLLISION_H__
#define __COLLISION_H__
#include "cocos2d.h"
USING_NS_CC;

class Collision : public Node
{
public:
	bool init();


	bool obbCollisionWithAABB(Rect rect);

	bool obbCollisionWithCircle(Vec2 circleCenterPoint, float radius);

	const std::vector<Vec2> &getCollisionPointVec() { return _collisionPointVec;  }
protected:
	void initCollisionPoint(Size size, std::vector<Vec2> points);
	void updateCollisionPoint();
	void collisionPointFlipY();
	void collisionPointFlipX();

	


	std::vector<Vec2> getCollisionVecByStr(std::string str);
	Node *_pCollisionLayer;
	std::vector<Vec2> _collisionPointVec;
	std::vector<Vec2> _orginCollisionPointVec;
	float _maxCollisionRange;
	bool mCollisioning;
	bool mUpdateing;
};

#endif