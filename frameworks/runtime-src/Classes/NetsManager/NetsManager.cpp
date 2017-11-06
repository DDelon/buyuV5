#include "NetsManager.h"
#include "Nets.h"
#include "ParseGameData.h"
NetsManager *NetsManager::_pInstance = nullptr;
NetsManager *NetsManager::getInstance()
{
	if (!_pInstance)
	{
		_pInstance = NetsManager::create();
		_pInstance->retain();
	}
	return _pInstance;
}

bool NetsManager::init()
{
	bool bRet = false;

	do 
	{
		CC_BREAK_IF(!Node::init());

		
		

		bRet = true;
	} while (false);

	return bRet;
}

void NetsManager::start(Layer *pNetsLayer)
{
	_pNetsLayer = pNetsLayer;
	this->initNetsPool();
	
}

void NetsManager::initNetsPool()
{

	TableMap& tm = ParseGameData::getInstance()->getTabMap("cannonoutlook");
	TableMap::iterator it = tm.begin();
	while (it != tm.end()) {
		FieldMap fm = it->second;
		if (fm["id"].asString().empty())
		{
			it++;
			continue;
		}
		int i = fm["id"].asInt();
		log("cannon out look id:%d size:%d", i, tm.size());
		_netsPool[Value(i).asString()] = Array::create();
		_netsPool[Value(i).asString()]->retain();
		for (int j = 0; j < 5; ++j)
		{

			Nets* nets = Nets::create();
			nets->initSprite(Value(i).asString());
			nets->retain();
			_netsPool[Value(i).asString()]->addObject(nets);
			_pNetsLayer->addChild(nets);
		}
		it++;
	}
}

void NetsManager::createNets(std::string resId, Vec2 pos, int effectId)
{
	for (int i = 0; i < _netsPool[resId]->count(); i++)
	{
		Nets *nets = dynamic_cast<Nets *>(_netsPool[resId]->objectAtIndex(i));
		if (nets && !nets->isUse())
		{
			nets->initWithPos(pos, effectId);
			return;
		}
	}

	Director::getInstance()->getScheduler()->performFunctionInCocosThread([=](){
		if (_pNetsLayer == nullptr)
		{
			return;
		}
		Nets *nets = Nets::create();
		nets->initSprite(resId);
		nets->retain();
		nets->initWithPos(pos);
		_pNetsLayer->addChild(nets);
		_netsPool[resId]->addObject(nets);
	});
}

Size NetsManager::getNetSize()
{
	SpriteFrame *pNetFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName("net_1_00.png");
	Size size = pNetFrame->getOriginalSize();
	return size;
}

void NetsManager::reset()
{
	_pNetsLayer = nullptr;
}
