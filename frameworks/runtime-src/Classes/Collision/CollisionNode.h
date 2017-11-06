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

	//�Ƿ�ʹ�þ�����ײ
	virtual bool isUseRectCollision() { return _isUseRect;  }

	//�Ƿ�ʹ����ײ�㷨�������ײ
	virtual bool isUseCollAlgori() { return true;  }

	float getRadius() { return _radius;  }

	//�Ƿ���Ҫ�������ײ
	virtual bool isCanCheck() { return true;  }

	//�Զ����ж���ײ ����д�ж��߼�ʵ��
	virtual bool customCollision() { return true;  }
	
	//����rect
	void resetRect(Rect rect) { _rect = rect; }

	//�����ײ����
	virtual void clearCollData();
protected:

	//ͨ�����γ�ʼ����ײ���� isUseRect �Ƿ�ʹ�þ�����ײ
	virtual void initCollisionPoint(Rect rect, bool isUseRect = true);

	//��������ײ�� ��ʼ����ײ����
	virtual void initCollisionPoint(Rect rect, std::vector<Vec2> points);

	//��ÿ֡���ô˽ӿڸ�����ײ��λ��
	virtual void updatePoints();

	//��ײ��x��ת
	virtual void flipX();

	//��ײ��y��ת
	virtual void flipY();

	

private:
	//��ӵ���ģʽ��ײ�����ui��LUA_DEBUG == 2���ã�
	void addCollisionLayerUI();

	//��ӵ���ģʽ��ײ���ui��LUA_DEBUG == 2���ã�
	void addCollisionPointsUI();

	//��ȡ��ײ����Զ���Ǹ����Բ�İ뾶
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

	bool _isRemove;	//�Ƿ���Ҫ��ɾ��
};
#endif