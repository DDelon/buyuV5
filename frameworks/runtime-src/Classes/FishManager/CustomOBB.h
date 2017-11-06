#ifndef __CUSTOM_OBB_H__
#define __CURTOM_OBB_H__
#include "cocos2d.h"
USING_NS_CC;

class Projection
{
public:
	Projection(float min, float max) : _min(min), _max(max) {}
	~Projection() {}

public:
	float getMin() { return _min;  }
	float getMax() { return _max;  }
	bool overlap(Projection *pOther) {
		if (_min > pOther->getMax())
		{
			return false;
		}

		if (_max < pOther->getMin())
		{
			return false;
		}

		return true;
	}

private:
	float _min;
	float _max;
};


class CustomOBB
{
public:
	CustomOBB(){ _vertexs = NULL; mIsUsing = false; }

	//CustomOBB(std::vector<Vec2> *vertexs) : _vertexs(vertexs) {}
	~CustomOBB() { _vertexs = NULL; }
	
	bool isCollisionWithOBB(CustomOBB *pOther);
	void setCollisionPoints(std::vector<Vec2> *points) { _vertexs = points; }
	void signUsed() { mIsUsing = true; }
	bool isUsing(){ return mIsUsing; }
	void reset() { _vertexs = NULL; mIsUsing = false;}
private:
	std::vector<Vec2> getAxies();
	Projection *getProjection(Vec2 axie);
	std::vector<Vec2> *_vertexs;

	bool mIsUsing;
};

class OBBManager :public Ref
{
public:
	static OBBManager *getInstance();
	static bool OBBCollisionCheck(std::vector<Vec2> *collisionPoints1, std::vector<Vec2> *collisionPoints2);
private:
	OBBManager();
	CREATE_FUNC(OBBManager);
	bool init();
	CustomOBB* getOBB();

	std::vector<CustomOBB*> mOBBPool;
};

#endif