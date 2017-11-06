#include "CollisionNode.h"
#include "CustomOBB.h"
#include "AppDelegate.h"
#include "Collision/CollisionManager.h"
#include "DebugLog.h"
#include <mutex>
#include "Bullet.h"


bool CollisionNode::init()
{
	bool bRet = false;

	do 
	{
		CC_BREAK_IF(!Node::init());


		_isUseRect = false;
		bRet = true;
	} while (false);

	return bRet;
}

bool CollisionNode::obbCollisionWithNode(CollisionNode *pCollisionNode)
{
	
	if (_isCleaning || (pCollisionNode == nullptr))
		return false;
	bool isCollision = false;
	if (_isUseRect)
	{
		this->resetRect(this->getBoundingBox());
		isCollision = pCollisionNode->obbCollisionWithAABB(_rect);
	}
	else if (pCollisionNode->isUseRectCollision())
	{
		
		pCollisionNode->resetRect(static_cast<Bullet *>(pCollisionNode)->getBoundingBox());
		isCollision = this->obbCollisionWithAABB(pCollisionNode->getRect());
	}
	else
	{
		Rect &rect = pCollisionNode->getRect();
		bool isInCircle = false;
		isInRange(isInCircle, rect);
		if (isInCircle)
		{
			std::vector<Vec2> &points = pCollisionNode->getPoints();
			isCollision = OBBManager::getInstance()->OBBCollisionCheck(&_curCollisionPoints, &points);
		}

		return isCollision;
	}

	
	return isCollision;
}

bool CollisionNode::obbCollisionWithAABB(Rect rect)
{
	
	if (_curCollisionPoints.empty())
	{
		return false;
	}
	Vec2 leftUpPoint = Vec2(rect.origin.x, rect.origin.y + rect.size.height);
	Vec2 leftDownPoint = Vec2(rect.origin.x, rect.origin.y);
	Vec2 rightUpPoint = Vec2(rect.origin.x + rect.size.width, rect.origin.y + rect.size.height);
	Vec2 rightDownPoint = Vec2(rect.origin.x + rect.size.width, rect.origin.y);
	bool isCollision = false;
	bool isInCircle = false;
	this->isInRange(isInCircle, rect);
	if (isInCircle)	//是否在圆内
	{
		std::vector<Vec2> vertexs;
		vertexs.push_back(leftUpPoint);
		vertexs.push_back(leftDownPoint);
		vertexs.push_back(rightUpPoint);
		vertexs.push_back(rightDownPoint);
		isCollision = OBBManager::getInstance()->OBBCollisionCheck(&_curCollisionPoints, &vertexs);
	}
	return isCollision;
}

bool CollisionNode::obbCollisionWithCircle(Vec2 circleCenterPoint, float radius)
{
	
	float dis = circleCenterPoint.distance(this->getPosition());
	if (dis > _radius + radius)
	{
		return false;
	}
	else
	{
		return true;
	}
}

void CollisionNode::initCollisionPoint(Rect rect, bool isUseRect)
{
	_isUseRect = isUseRect;
	Vec2 nodeVec = rect.origin;
	Vec2 leftUpPoint = Vec2(nodeVec.x, nodeVec.y+rect.size.height);
	Vec2 leftDownPoint = Vec2(nodeVec.x, nodeVec.y);
	Vec2 rightUpPoint = Vec2(nodeVec.x + rect.size.width, nodeVec.y+rect.size.height);
	Vec2 rightDownPoint = Vec2(nodeVec.x + rect.size.width, nodeVec.y);

	std::vector<Vec2> points;

	points.push_back(leftUpPoint);
	points.push_back(leftDownPoint);
	points.push_back(rightDownPoint);
	points.push_back(rightUpPoint);


	this->initCollisionPoint(rect, points);

}

void CollisionNode::initCollisionPoint(Rect rect, std::vector<Vec2> points)
{
	_oriCollisionPoints.clear();
	_oriCollisionPoints = points;
	_rect = rect;
	_radius = getRadius(points);
	_isCollision = false;
	_isCleaning = false;
	_isRemove = false;
	updatePoints();
	//this->addCollisionLayerUI();
	//this->addCollisionPointsUI();

/*#if (LUA_DEBUG == 2)		//调试模式默认开启鱼的碰撞点和碰撞范围
	
	
	
#endif*/
	
}

void CollisionNode::updatePoints()
{
	_isCleaning = true;
	_curCollisionPoints.clear();

	for (auto oriIter : _oriCollisionPoints)
	{
		Vec2 worldPos = this->convertToWorldSpace(oriIter);
		_curCollisionPoints.push_back(worldPos);
	}
	_isCleaning = false;
}

void CollisionNode::flipX()
{
	
	for (auto iter = _oriCollisionPoints.begin(); iter != _oriCollisionPoints.end(); ++iter)
	{
		iter->x = (_rect.size.width - iter->x);
	}
	//this->addCollisionPointsUI();

/*#if (LUA_DEBUG == 2)
	this->addCollisionPointsUI();
#endif*/
}

void CollisionNode::flipY()
{
	for (auto iter = _oriCollisionPoints.begin(); iter != _oriCollisionPoints.end(); ++iter)
	{

		iter->y = (-iter->y);
	}
	//this->addCollisionPointsUI();

/*#if (LUA_DEBUG == 2)
	this->addCollisionPointsUI();
#endif*/
}

float CollisionNode::getRadius(std::vector<Vec2> &points)
{
	float radius = 0;
	for (int pointIndex1 = 0; pointIndex1 < points.size(); ++pointIndex1)
	{
		for (int pointIndex2 = pointIndex1 + 1; pointIndex2 < points.size(); ++pointIndex2)
		{
			float distance = points[pointIndex1].distance(points[pointIndex2]);
			if (distance > radius)
			{
				radius = distance;
			}
		}
	}

	return radius;
}

void CollisionNode::clearCollData()
{
	_curCollisionPoints.clear();
	_oriCollisionPoints.clear();
	_isCollision = true;
	_isUseRect = false;
	_isRemove = true;
	//this->removeChild(_pCollisionLayer);
	//_pCollisionLayer = NULL;
/*#if (LUA_DEBUG == 2)
	
#endif*/
}

void CollisionNode::isInRange(bool &isInCircle, Rect rect)
{
	//判断是否在圆内
	if (_curCollisionPoints.empty())
	{
		return;
	}
	Vec2 leftUpPoint = Vec2(rect.origin.x, rect.origin.y + rect.size.height);
	Vec2 leftDownPoint = Vec2(rect.origin.x, rect.origin.y);
	Vec2 rightUpPoint = Vec2(rect.origin.x + rect.size.width, rect.origin.y + rect.size.height);
	Vec2 rightDownPoint = Vec2(rect.origin.x + rect.size.width, rect.origin.y);

	float rectRadius = leftUpPoint.distance(rightUpPoint) / 2;
	//比较两个碰撞物体的半径 获取最大的半径
	float maxRadius = ((rectRadius > _radius) ? rectRadius : _radius);

	if (_isCleaning)
		return;
	for (int i = 0; i < _curCollisionPoints.size(); ++i)
	{
		Vec2 pos = _curCollisionPoints[i];
		if (leftUpPoint.distance(pos) <= maxRadius ||
			leftDownPoint.distance(pos) <= maxRadius ||
			rightUpPoint.distance(pos) <= maxRadius ||
			rightDownPoint.distance(pos) <= maxRadius)
		{
			isInCircle = true;
			return;
		}
	}

}

void CollisionNode::addCollisionLayerUI()
{
	_pCollisionLayer = LayerColor::create(Color4B(100, 100, 100, 0));
	_pCollisionLayer->setContentSize(_rect.size);
	_pCollisionLayer->setPosition(Vec2(0, 0));
	this->addChild(_pCollisionLayer, 20);
}

void CollisionNode::addCollisionPointsUI()
{
	
	for (auto iter : _oriCollisionPoints)
	{
		LayerColor *pNode = LayerColor::create(Color4B(0, 255, 0, 255));
		pNode->setContentSize(Size(6, 6));
		pNode->setPosition(Vec2(iter.x, iter.y));
		_pCollisionLayer->addChild(pNode);
	}
}