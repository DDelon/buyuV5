#include "Collision.h"
#include "StringUtil.h"
#include "CustomOBB.h"
bool Collision::init()
{
	bool bRet = false;

	do 
	{
		CC_BREAK_IF(!Node::init());

		mCollisioning = false;
		mUpdateing = false;
		_maxCollisionRange = 0.0f;
		bRet = true;
	} while (false);

	return bRet;
}

void Collision::initCollisionPoint(Size size, std::vector<Vec2> points)
{

	_pCollisionLayer = Node::create();
	_pCollisionLayer->setContentSize(size);
	_pCollisionLayer->setPosition(Vec2(-size.width / 2, -size.height/2));
	this->addChild(_pCollisionLayer, 20);
	_orginCollisionPointVec.clear();

	for (auto iter : points)
	{
		/*LayerColor *pNode = LayerColor::create(Color4B(255, 0, 0, 255));
		pNode->setContentSize(Size(2, 2));
		pNode->setPosition(Vec2(iter.x +size.width / 2, iter.y +size.height / 2));
		_pCollisionLayer->addChild(pNode);*/
		_orginCollisionPointVec.push_back(Vec2(iter.x + size.width / 2, iter.y + size.height / 2));
	}
	if (points.size()>1)
	{
		for (int startPoinIndex = 0; startPoinIndex < points.size() - 1; startPoinIndex++)
		{
			for (int endPoinIndex = startPoinIndex+1; endPoinIndex< points.size(); endPoinIndex++)
			{
				float distance = points[startPoinIndex].distance(points[endPoinIndex]);
				if (distance>_maxCollisionRange)
				{
					_maxCollisionRange = distance;
				}
			}
		}
		_maxCollisionRange /= 2.0f;
	}
}

bool Collision::obbCollisionWithAABB(Rect rect)
{
	if (_collisionPointVec.empty())
	{
		return false;
	}
	if (mUpdateing)
	{
		return false;
	}
	mCollisioning = true;
	bool isCollision = false;

	Vec2 leftUpPoint = Vec2(rect.origin.x, rect.origin.y + rect.size.height);
	Vec2 leftDownPoint = Vec2(rect.origin.x, rect.origin.y);
	Vec2 rightUpPoint = Vec2(rect.origin.x + rect.size.width, rect.origin.y + rect.size.height);
	Vec2 rightDownPoint = Vec2(rect.origin.x + rect.size.width, rect.origin.y);

	bool inRange = false;
	float rangeDistance = _maxCollisionRange;
	float rectRange = 0.5f*leftUpPoint.distance(rightDownPoint);
	if (_maxCollisionRange < rectRange)
	{
		rangeDistance = rectRange;
	}
	
	for (int i = 0; i < _collisionPointVec.size(); i++)
	{
		Vec2 point = _collisionPointVec[i];
		if (leftUpPoint.distance(point) <= rangeDistance
			|| leftDownPoint.distance(point) <= rangeDistance
			|| rightUpPoint.distance(point) <= rangeDistance
			|| rightDownPoint.distance(point) <= rangeDistance
			)
		{
			inRange = true;
			break;
		}
	}

	if (inRange)
	{
		std::vector<Vec2> *pVertexs = new std::vector<Vec2>();
		pVertexs->push_back(leftUpPoint);
		pVertexs->push_back(leftDownPoint);
		pVertexs->push_back(rightUpPoint);
		pVertexs->push_back(rightDownPoint);

		/*CustomOBB *pOBB1 = new CustomOBB(&_collisionPointVec);
		CustomOBB *pOBB2 = new CustomOBB(pVertexs);
		isCollision = pOBB1->isCollisionWithOBB(pOBB2);
		delete pOBB1;
		delete pOBB2;*/

		isCollision = OBBManager::getInstance()->OBBCollisionCheck(&_collisionPointVec, pVertexs);
		delete pVertexs;
	}


	mCollisioning = false;
	return isCollision;
}

bool Collision::obbCollisionWithCircle(Vec2 circleCenterPoint, float radius)
{

	/*Vec2 fishCenter = this->getPosition();
	float maxFishRadius = fishCenter.distance(_collisionPointVec[0]);
	for (int i = 1; i < _collisionPointVec.size(); ++i)
	{
		float dist = fishCenter.distance(_collisionPointVec[i]);
		if (dist > maxFishRadius)
		{
			maxFishRadius = dist;
		}
	}

	//两个圆心距离
	float circleCenterDist = circleCenterPoint.distance(fishCenter);
	if (circleCenterDist < (radius + maxFishRadius))
	{
		return true;
	}
	else
	{
		return false;
	}*/
	//把园变成矩形
	
	Vec2 ori(circleCenterPoint.x - radius, circleCenterPoint.y - radius);
	Size size(radius * 2, radius * 2);
	Rect rect(ori, size);
	return obbCollisionWithAABB(rect);

}

std::vector<Vec2> Collision::getCollisionVecByStr(std::string str)
{
	std::vector<Vec2> pointTabVec2;
	std::vector<std::string> pointTabStr;
	StringUtil::_STR_SPLIT(str, ";", pointTabStr);

	for (auto iter : pointTabStr)
	{
		if (iter.size() == 0)
		{
			break;
		}
		std::vector<std::string> pointStr;
		StringUtil::_STR_SPLIT(iter, ",", pointStr);
		pointTabVec2.push_back(Vec2(atof(pointStr[0].c_str()), atof(pointStr[1].c_str())));
	}

	return pointTabVec2;
}

void Collision::updateCollisionPoint()
{
	if (mCollisioning)
	{
		return;
	}
	mUpdateing = true;
	_collisionPointVec.clear();
	//Vector<Node *> nodes = _pCollisionLayer->getChildren();

	for (auto iter : _orginCollisionPointVec)
	{
		Vec2 worldPos = _pCollisionLayer->convertToWorldSpace(iter);
		_collisionPointVec.push_back(worldPos);
	}
	mUpdateing = false;
}

void Collision::collisionPointFlipY()
{
	float width = _pCollisionLayer->getContentSize().width;
	
	Vector<Node *> nodes = _pCollisionLayer->getChildren();
	for (auto iter : nodes)
	{
		Vec2 pos = iter->getPosition();
		pos.x = width - pos.x;

		iter->setPosition(pos);
	}
}

void Collision::collisionPointFlipX()
{
	float height = _pCollisionLayer->getContentSize().height;

	Vector<Node *> nodes = _pCollisionLayer->getChildren();
	for (auto iter : nodes)
	{
		Vec2 pos = iter->getPosition();
		pos.y = height - pos.y;

		iter->setPosition(pos);
	}
}
