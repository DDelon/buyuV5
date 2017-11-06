#ifndef __NETS_H__
#define __NETS_H__

#include "cocos2d.h"
USING_NS_CC;

#define NETS_ACTION_TAG 321


class Nets : public Node
{
public:
	virtual bool init();
	void initWithPos(Vec2 pos, int effectId = 0);

	bool isUse() { return _isUse;  }

	CREATE_FUNC(Nets);

	void removeFromScreen();

	void initSprite(std::string resId);

private:
	Action* initAnimation(std::string fileName, float delay);

	void initEffect(int effectId);

	Sprite *_pNetSprite;

	bool _isUse;
};

#endif