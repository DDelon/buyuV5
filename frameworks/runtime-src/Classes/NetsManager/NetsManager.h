#ifndef __NETS_MANAGER_H__
#define __NETS_MANAGER_H__
#include "cocos2d.h"
USING_NS_CC;

class NetsManager : public Node
{
public:
	static NetsManager *getInstance();
	CREATE_FUNC(NetsManager);
	virtual bool init();

	void createNets(std::string resId, Vec2 pos, int effectId = 0);

	void start(Layer *pNetsLayer);

	Size getNetSize();

	void reset();

private:

	void initNetsPool();

	static NetsManager *_pInstance;

	std::map<std::string, Array *> _netsPool;

	Layer *_pNetsLayer;

	
};

#endif