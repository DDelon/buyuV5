#include "CustomOBB.h"

std::vector<Vec2> CustomOBB::getAxies()
{
	std::vector<Vec2> axiesVec;
	int vertexSize = _vertexs->size();
	for (int i = 0; i < _vertexs->size(); ++i)
	{
		
		int nextIndex = (i + 1) % vertexSize;
		float subX = (*_vertexs)[i].x - (*_vertexs)[nextIndex].x;
		float subY = (*_vertexs)[i].y - (*_vertexs)[nextIndex].y;

		Vec2 subVec = Vec2(subX, subY);
		Vec2 norVec = subVec.getNormalized();
		axiesVec.push_back(Vec2(-norVec.y, norVec.x));
	}

	return axiesVec;
}

Projection *CustomOBB::getProjection(Vec2 axie)
{
	if (_vertexs->size() == 0)
	{
		return nullptr;
	}
	float min = (*_vertexs)[0].dot(axie);
	float max = min;

	for (int i = 1; i < _vertexs->size(); ++i)
	{
		float temp = (*_vertexs)[i].dot(axie);
		if (temp > max)
		{
			max = temp;
		}
		else if (temp < min)
		{
			min = temp;
		}
	}

	return new Projection(min, max);

}

bool CustomOBB::isCollisionWithOBB(CustomOBB *pOther)
{
	std::vector<Vec2> axie1 = getAxies();
	std::vector<Vec2> axie2 = pOther->getAxies();
	for (int i = 0; i < axie1.size(); ++i)
	{
		Projection *pProj1 = getProjection(axie1[i]);
		Projection *pProj2 = pOther->getProjection(axie1[i]);
		if (pProj1 == nullptr || pProj2 == nullptr)
		{
			delete pProj1;
			delete pProj2;
			return false;
		}
		if (pProj1->overlap(pProj2) == false)
		{
			delete pProj1;
			delete pProj2;
			return false;
		}
		delete pProj1;
		delete pProj2;
	}
	for (int i = 0; i < axie2.size(); ++i)
	{
		Projection *pProj1 = getProjection(axie2[i]);
		Projection *pProj2 = pOther->getProjection(axie2[i]);
		if (pProj1 == nullptr || pProj2 == nullptr)
		{
			delete pProj1;
			delete pProj2;
			return false;
		}
		if (pProj1->overlap(pProj2) == false)
		{
			delete pProj1;
			delete pProj2;
			return false;
		}
		delete pProj1;
		delete pProj2;
	}
	return true;
}

static OBBManager * sOBBinstance = NULL;
OBBManager * OBBManager::getInstance()
{
	if (!sOBBinstance)
	{
		sOBBinstance = OBBManager::create();
		sOBBinstance->retain();
	}
	return sOBBinstance;
}

OBBManager::OBBManager(){}

bool OBBManager::init()
{
	for (int i = 0; i < 5;i++)
	{
		CustomOBB* obb = new CustomOBB();
		mOBBPool.push_back(obb);
	}
	return true;
}

CustomOBB* OBBManager::getOBB()
{
	for (CustomOBB* obb : mOBBPool)
	{
		if (!obb->isUsing())
		{
			obb->signUsed();
			return obb;
		}
	}

	CustomOBB* obbNew = new CustomOBB();
	mOBBPool.push_back(obbNew);
	return obbNew;
}

bool OBBManager::OBBCollisionCheck(std::vector<Vec2> *collisionPoints1, std::vector<Vec2> *collisionPoints2)
{
	if (collisionPoints1->size() == 0 || collisionPoints2->size() == 0)
		return false;
	////CCLOG("-----------------201");
	CustomOBB* obb1 = OBBManager::getInstance()->getOBB();
	CustomOBB* obb2 = OBBManager::getInstance()->getOBB();
	////CCLOG("-----------------202");
	obb1->setCollisionPoints(collisionPoints1);
	obb2->setCollisionPoints(collisionPoints2);
	////CCLOG("-----------------203");
	bool isCollision = obb1->isCollisionWithOBB(obb2);
	////CCLOG("-----------------204");
	obb1->reset();
	////CCLOG("-----------------205");
	obb2->reset();
	////CCLOG("-----------------206");
	return isCollision;
}
