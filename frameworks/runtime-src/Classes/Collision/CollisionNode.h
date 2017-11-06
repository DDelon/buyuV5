#ifndef __COLLISION_NODE_H__
#define __COLLISION_NODE_H__
#include "cocos2d.h"
USING_NS_CC;


class CollisionNode : public Node
{
public:
	virtual bool init();

	bool obbCollisionWithNode(CollisionNode *pCollisionNode);

	bool obbCollisionWithAABB(Rect rect);

	bool obbCollisionWithCircle(Vec2 circleCenterPoint, float radius);

	bool obbCollisionWithTest(Vec2 circleCenterPoint, float radius) { return true; }

	Rect &getRect() { return _rect;  }

	std::vector<Vec2> &getPoints() { return _curCollisionPoints;  }

	virtual bool isCollision() { return _isCollision; }

	virtual bool isRemove() { return _isRemove; }

	virtual ValueMap dealNodeCollision(CollisionNode *pOther) = 0;

	//是否使用矩形碰撞
	virtual bool isUseRectCollision() { return _isUseRect;  }

	//是否使用碰撞算法来检测碰撞
	virtual bool isUseCollAlgori() { return true;  }

	float getRadius() { return _radius;  }

	//是否需要被检测碰撞
	virtual bool isCanCheck() { return true;  }

	//自定义判断碰撞 子类写判断逻辑实现
	virtual bool customCollision() { return true;  }
	
	//设置rect
	void resetRect(Rect rect) { _rect = rect; }

	//清除碰撞数据
	virtual void clearCollData();
protected:

	//通过矩形初始化碰撞数据 isUseRect 是否使用矩形碰撞
	virtual void initCollisionPoint(Rect rect, bool isUseRect = true);

	//不规则碰撞点 初始化碰撞数据
	virtual void initCollisionPoint(Rect rect, std::vector<Vec2> points);

	//鱼每帧调用此接口更新碰撞点位置
	virtual void updatePoints();

	//碰撞点x翻转
	virtual void flipX();

	//碰撞点y翻转
	virtual void flipY();

	

private:
	//添加调试模式碰撞区域的ui（LUA_DEBUG == 2调用）
	void addCollisionLayerUI();

	//添加调试模式碰撞点的ui（LUA_DEBUG == 2调用）
	void addCollisionPointsUI();

	//获取碰撞点最远的那个点的圆的半径
	float getRadius(std::vector<Vec2> &points);

	void isInRange(bool &isInCircle, Rect rect);

protected:
	LayerColor *_pCollisionLayer;

	std::vector<Vec2> _curCollisionPoints;
	std::vector<Vec2> _oriCollisionPoints;

	Rect _rect;
	float _radius;

	bool _isCollision;

	bool _isCleaning;

	bool _isUseRect;

	bool _isRemove;	//是否需要被删除
};
#endif