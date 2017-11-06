#include "CoinManager.h"
#include "StringUtil.h"
#include "audio/include/SimpleAudioEngine.h"
#include "LuaCppAdapter.h"
#include "DebugLog.h"
#include "ParseGameData.h"
#include "AudioControl.h"
#include "Player/PlayerManager.h"
CoinManager *_pInstance = nullptr;

CoinManager * CoinManager::getInstance()
{
	if (_pInstance == nullptr)
	{
		_pInstance = new(std::nothrow) CoinManager();
		if (_pInstance && _pInstance->init())
		{
			_pInstance->retain();

		}
	}
	return _pInstance;
}

bool CoinManager::init()
{
	Node::init();

	this->initCoinPool();
	mCoinLayer = NULL;
	isPlayCoinSound = false;
	mGameType = 0;

	mLaberAction = Sequence::createWithTwoActions(
		DelayTime::create(COIN_ANIMATION_DELAY),
		Spawn::createWithTwoActions(
		CallFunc::create([=](){AudioControl::getInstance()->playEffect("sound/collect_01.mp3"); }),
			Sequence::create(
				Show::create(),ScaleTo::create(0.08f,1.5f),
				DelayTime::create(0.08f), ScaleTo::create(0.08f, 0.9f),
				DelayTime::create(0.08f), ScaleTo::create(0.08f, 1.10f),
				DelayTime::create(0.08f), ScaleTo::create(0.08f, 0.95f),
				DelayTime::create(0.08f), ScaleTo::create(0.08f, 1.00f),
				DelayTime::create(0.08f), Spawn::createWithTwoActions(FadeTo::create(0.5, 0), MoveBy:: create(0.5, Vec2(0, 30))),
				DelayTime::create(0.25f), RemoveSelf::create(),
				FadeTo::create(0, 255),
				NULL)
		));

	mLaberAction->retain();

	return true;
}

void CoinManager::initCoinPool()
{
	mSelfCoinPool = Array::create();
	mSelfCoinPool->retain();
	mOtherCoinPool = Array::create();
	mOtherCoinPool->retain();
	mSelfGoldLabelPool = Array::create();
	mSelfGoldLabelPool->retain();
	mOtherGoldLabelPool = Array::create();
	mOtherGoldLabelPool->retain();

	for (int i = 0; i < COIN_POOL_SIZE;i++)
	{
		Coin* coin = Coin::Create(true);
		coin->setIsUsing(false);
		coin->retain();
		mSelfCoinPool->addObject(coin);
	}

	for (int i = 0; i < COIN_POOL_SIZE * 3; i++)
	{
		Coin* coin = Coin::Create(false);
		coin->setIsUsing(false);
		coin->retain();
		mOtherCoinPool->addObject(coin);
	}

	for (int i = 0; i < COIN_POOL_SIZE/12; i++)
	{
		ui::TextBMFont* label = ui::TextBMFont::create();
		label->setFntFile("fnt/bonus_num_1.fnt");
		label->setScale(0.4f);
		label->setVisible(false);
		label->retain();
		mSelfGoldLabelPool->addObject(label);
	}


	for (int i = 0; i < COIN_POOL_SIZE / 3; i++)
	{
		ui::TextBMFont* label = ui::TextBMFont::create();
		label->setFntFile("fnt/bonus_num_2.fnt");
		label->setScale(0.4f);
		label->setVisible(false);
		label->retain();
		mOtherGoldLabelPool->addObject(label);
	}
}

Coin* CoinManager::getCoin(bool isSelf)
{
	if (isSelf)
	{
		for (int i = 0; i < mSelfCoinPool->count(); i++)
		{
			Coin* coin = dynamic_cast<Coin*>(mSelfCoinPool->objectAtIndex(i));
			if (coin && !coin->isUseing() && !coin->getParent())
			{
				coin->setIsUsing(true);
				return coin;
			}
		}

		//CCLOG("=========create new self coin");
		Coin* coinNew = Coin::Create(true);
		coinNew->setIsUsing(true);
		coinNew->retain();
		mSelfCoinPool->addObject(coinNew);
		return coinNew;
	}
	else
	{
		for (int i = 0; i < mOtherCoinPool->count(); i++)
		{
			Coin* coin = dynamic_cast<Coin*>(mOtherCoinPool->objectAtIndex(i));
			if (coin && !coin->isUseing())
			{
				coin->setIsUsing(true);
				return coin;
			}
		}

		//CCLOG("=========create new other coin");
		Coin* coinNew = Coin::Create(false);
		coinNew->setIsUsing(true);
		coinNew->retain();
		mOtherCoinPool->addObject(coinNew);
		return coinNew;
	}
}

float CoinManager::showGainCoinEffect(int playerId, int chairId, int moneyCount, int coinNum, Vec2 orginPos, bool isSelf, Vec2 endPos, bool isShowAddCount, float delayTime)
{
	float allDelayTime = 0;
	float timeDis = 0.12f;
	float timeShow = 0.83f;
	float timeMove = 0.75f;
	int maxCoinNum = GAMEVALUE_INT("config", "990000047", "data");
	if (!mCoinLayer)
	{
		return allDelayTime;
	}
	if (mPlayCoinNum >= maxCoinNum)
	{
		allDelayTime = delayTime + timeDis + timeShow + timeMove;
		return allDelayTime;
	}
	if (moneyCount>0 && isShowAddCount)
	{
		ui::TextBMFont* label = getGoldLabel(isSelf);
		label->setPosition(orginPos);
		label->setText(StringUtil::format("+%d",moneyCount));
		mCoinLayer->addChild(label);
		//label->runAction(mLaberAction->clone());
		label->setVisible(false);
		label->runAction(Sequence::create(DelayTime::create(delayTime - COIN_ANIMATION_DELAY - COIN_LABEL_ANIMATION_DELAY), mLaberAction->clone(), nullptr));
	}
	
	Vec2 RowColCount = getRowAndColByCount(coinNum);
	Vec2 aimPos = endPos;
	if (aimPos == Vec2(0, 0))
	{
		aimPos = getAimPosByPlayerId(playerId);
		//aimPos = Vec2(aimPos.x*mWinScaleX, aimPos.y*mWinScaleY);
	}
	//log("------aimPos.x=%f------------aimPos.y=%f-------", aimPos.x, aimPos.y);
	for (int i = 0; i < RowColCount.x;i++)
	{
		for (int j = 0; j < RowColCount.y;j++)
		{
			Coin* coin = getCoin(isSelf);
			coin->getCoinSpr()->setVisible(true);
			coin->getCoinSpr()->setOpacity(255);
			coin->setPosition(Vec2(orginPos.x + 50 * i - RowColCount.x / 2 * 50, orginPos.y - 60 * j + RowColCount.y / 2 * 60));
			coin->run();
			coin->setScale(0.3f);
			coin->setVisible(false);
			mCoinLayer->addChild(coin);
			float delaytime_Row = delayTime + timeDis * i;
			coin->runAction(Sequence::create(DelayTime::create(delaytime_Row), Show::create(), ScaleTo::create(0.54, 1),NULL));
			coin->runAction(Sequence::create(DelayTime::create(delaytime_Row), MoveBy::create(0.21, Vec2(0, 88)), NULL));
			coin->runAction(Sequence::create(DelayTime::create(delaytime_Row + 0.21), MoveBy::create(0.20, Vec2(0, -103)), NULL));
			coin->runAction(Sequence::create(DelayTime::create(delaytime_Row + 0.41), MoveBy::create(0.13, Vec2(0, 27)), NULL));
			coin->runAction(Sequence::create(DelayTime::create(delaytime_Row + timeShow), ScaleTo::create(timeMove, 0.7), NULL));
// 			coin->runAction(Sequence::create(DelayTime::create(delaytime_Row + 0.83), MoveTo::create(0.75, aimPos), RemoveSelf::create(),
// 												CallFunc::create([=](){coin->setIsUsing(false); }), NULL));
			auto speedAct = EaseExponentialIn::create(MoveTo::create(timeMove, aimPos));
			if (i== 0 && j == 0)
			{
				auto palyEff = CallFunc::create([=](){
					if ( !isPlayCoinSound)
					{
						isPlayCoinSound = true;
						AudioControl::getInstance()->playEffect("sound/gold_get02.mp3");
						mCoinLayer->runAction(Sequence::create(DelayTime::create(GAMEVALUE_FLOAT("config", "990000027", "data")), CallFunc::create([=](){
							isPlayCoinSound = false;
						}), NULL));
					}
				});
				coin->runAction(Sequence::create(DelayTime::create(delaytime_Row + 1.2), palyEff, NULL));
				
			}
			coin->runAction(Sequence::create(DelayTime::create(delaytime_Row + timeShow), speedAct, RemoveSelf::create(),
				CallFunc::create([=](){coin->setIsUsing(false); }), NULL));
		}
	}
	allDelayTime = delayTime + timeDis + timeShow + timeMove;
	mPlayCoinNum++;

	return allDelayTime;
}

float CoinManager::showGunUpEffect(int playerId, int chairId, int moneyCount, int coinNum)
{
	Vec2 aimPos = getAimPosByPlayerId(playerId);
	aimPos = Vec2(aimPos.x, aimPos.y);
	float delaytime = 0.32;
	float timeUp = 0.08;
	float timeDel = 1.5f;
	float timeDown = 0.28;
	for (int i = 0; i < coinNum; ++i)
	{
		float DelDis = delaytime + 0.02*i;
		Coin* coin = getCoin(true);
		float offsetX = RandomHelper::random_int(-20, 20)*mWinScaleX;
		float offsetY = RandomHelper::random_int(-10, 10)*mWinScaleY;
		coin->setPosition(Vec2(aimPos.x + offsetX, aimPos.y + 140 * mWinScaleY));
		coin->run();
		mCoinLayer->addChild(coin);

		coin->setVisible(true);
		coin->setScale(1);
		auto fateUp = FadeTo::create(timeUp, 255);
		auto moveUp = MoveBy::create(timeUp, Vec2(0, 120 * mWinScaleY + offsetY));

		auto fateDown = FadeTo::create(timeDown, 255*0.5);
		auto moveDown = MoveTo::create(timeDown, aimPos);

		float endDelTime = 0.2;
		auto seq = Sequence::create(DelayTime::create(DelDis), moveUp, DelayTime::create(timeDel), moveDown, DelayTime::create(endDelTime), Hide::create(), nullptr);
		coin->runAction(seq);

		coin->getCoinSpr()->setVisible(true);
		coin->getCoinSpr()->setOpacity(0);
		auto seq2 = Sequence::create(DelayTime::create(DelDis), fateUp, DelayTime::create(timeDel), fateDown, nullptr);
		coin->getCoinSpr()->runAction(seq2);
		if (i == 0)
		{
			coin->runAction(Sequence::create(DelayTime::create(DelDis + timeUp + timeDel), CallFunc::create([=](){AudioControl::getInstance()->playEffect("sound/gold_get02.mp3"); }), NULL));
		}
	}

	float allDelayTime = delaytime + timeUp + timeDel + timeDown;

// 	ValueMap data;
// 	data["playerId"] = playerId;
// 	data["propId"] = 1;
// 	data["propCount"] = moneyCount;
// 	data["delaytime"] = allDelayTime;
// 	data["isAddWinCount"] = true;
// 	LuaCppAdapter::getInstance()->callLuaFunc("FishGF", "CppToLua", "gainGoldData", data);

	return allDelayTime;
}

ui::TextBMFont* CoinManager::getGoldLabel(bool isSelf)
{
	if (isSelf)
	{
		for (int i = 0; i < mSelfGoldLabelPool->count(); i++)
		{
			ui::TextBMFont* label = dynamic_cast<ui::TextBMFont*>(mSelfGoldLabelPool->objectAtIndex(i));
			if (!label->getParent())
			{
				label->setScale(0.4f);
				label->setVisible(false);
				return label;
			}		
		}

		string fileName = "fnt/bonus_num_1.fnt";
		ui::TextBMFont* label = ui::TextBMFont::create();
		label->setFntFile(fileName);
		label->setScale(0.4f);
		label->setVisible(false);
		label->retain();
		mSelfGoldLabelPool->addObject(label);
		return label;

	}
	else
	{
		for (int i = 0; i < mOtherGoldLabelPool->count(); i++)
		{
			ui::TextBMFont* label = dynamic_cast<ui::TextBMFont*>(mOtherGoldLabelPool->objectAtIndex(i));
			if (!label->getParent())
			{
				label->setScale(0.4f);
				label->setVisible(false);
				return label;
			}
		}

		string fileName = "fnt/bonus_num_2.fnt";
		ui::TextBMFont* label = ui::TextBMFont::create();
		label->setFntFile(fileName);
		label->setScale(0.4f);
		label->setVisible(false);
		label->retain();
		mOtherGoldLabelPool->addObject(label);
		return label;
	}
}


Coin* Coin::Create(bool isSelf /*= true*/)
{
	Coin *pRet = new(std::nothrow) Coin(); 
	if (pRet && pRet->init(isSelf))
	{ 
		pRet->autorelease(); 
		return pRet; 
	} 
	else 
	{ 
		delete pRet; 
		pRet = nullptr; 
		return nullptr; 
	} 
}


bool Coin::init(bool isSelf)
{
	Node::init();
	mIsUseing = false;
	mIsSelfCoin = isSelf;
	mSprite = Sprite::create();
	this->addChild(mSprite);
	mAnimation = nullptr;
	this->initAnimation(isSelf);
	return true;
}

bool Coin::initAnimation(bool isSelf)
{
	if (mAnimation != nullptr)
	{
		mAnimation->release();
		mAnimation = nullptr;
	}
	mAnimation = Animation::create();
	int N = rand() % 10;

	int gameType = CoinManager::getInstance()->getGameType();
	//log("------------------------------------------------gameType=%d", gameType);
	for (int i = 0; i < 10; i++)
	{
		int key = (i + N) % 10;
		if (gameType == 1)
		{
			auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(StringUtil::format("friend_coin%d_0%d.png", isSelf ? 1 : 2, key));
			if (frame)
			{
				mAnimation->addSpriteFrame(frame);
			}
		}
		else
		{
			auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(StringUtil::format("game_coin%d_0%d.png", isSelf ? 1 : 2, key));
			if (frame)
			{
				mAnimation->addSpriteFrame(frame);
			}
		}

	}
	mAnimation->setDelayPerUnit(1.0f / 20.0f);
	mAnimation->retain();
	return true;
}

void Coin::run()
{
	this->stopAllActions();
	mSprite->runAction(RepeatForever::create(Animate::create(mAnimation->clone())));

}

cocos2d::Vec2 CoinManager::getRowAndColByCount(int count)
{
	int Row = 0;
	int Col = 0;
	if (count == 2){
		Row = 2;
		Col = 1;
	}
	else if(count == 5)
	{
		Row = 5;
		Col = 1;
	}
	else if (count == 8)
	{
		Row = 4;
		Col = 2;
	}
	else if (count == 12)
	{
		Row = 6;
		Col = 2;
	}
	else if (count == 18)
	{
		Row = 6;
		Col = 3;
	}
	return Vec2(Row,Col);
}

cocos2d::Vec2 CoinManager::getAimPosByChairId(int chairId)
{
	Vec2 pos;
	switch (chairId)
	{
	case 1:
		pos = Vec2(332.43, 40);
		break;
	case 2:
		pos = Vec2(945.05, 40);
		break;
	case 3:
		pos = Vec2(945.05, 680);
		break;
	case 4:
		pos = Vec2(332.43, 680);
		break;
	default:
		pos = Vec2(0,0);
		break;
	}



	return pos;
}

void CoinManager::killBonusFish(int moneyCount, int fishId, int chairId, int palyerId, float delayTime)
{
	if (delayTime >= COIN_ANIMATION_DELAY + COIN_LABEL_ANIMATION_DELAY)
	{
		delayTime = delayTime - COIN_ANIMATION_DELAY - COIN_LABEL_ANIMATION_DELAY;
	}
	Vec2 aimPos = getAimPosByPlayerId(palyerId);
	float dis = 160;
	chairId > 2 ? dis = -160 : dis = 160;
	aimPos = Vec2(aimPos.x, aimPos.y + dis*mWinScaleY);

	Sprite* parentNode = (Sprite*)(mCoinLayer->getChildByTag(chairId));
	if (parentNode == nullptr)
	{
		parentNode = Sprite::create();
		parentNode->setTag(chairId);
		parentNode->setAnchorPoint(Vec2(0, 0));
		parentNode->setPosition(aimPos);
		mCoinLayer->addChild(parentNode, 100);

		auto wheel = Sprite::create("battle/killbonus/bonus_wheel_bg.png");
		parentNode->addChild(wheel);

		auto titleBg = Sprite::create("battle/killbonus/bonus_wheel_title_bg.png");
		titleBg->setPosition(Vec2(0, -66));
		parentNode->addChild(titleBg);

		string titleName = StringUtil::format("battle/killbonus/fishname/fishname_%d.png", fishId - 100000000);
		auto title = Sprite::create(titleName);
		title->setName("fishname");
		title->setPosition(Vec2(0, -66));
		parentNode->addChild(title);

		ui::TextBMFont* label = ui::TextBMFont::create();
		label->setName("bonus_num");
		label->setFntFile("fnt/bonus_num_1.fnt");
		label->setString(StringUtil::format("%d", moneyCount));
		parentNode->addChild(label);

		wheel->runAction(RepeatForever::create(RotateBy::create(1, 360 * 2)));
		label->runAction(RepeatForever::create(Sequence::create(RotateTo::create(0.48, -10), RotateTo::create(0.48, 10), nullptr)));

	}
	else
	{
		string titleName = StringUtil::format("battle/killbonus/fishname/fishname_%d.png", fishId - 100000000);
		auto title = (Sprite*)(parentNode->getChildByName("fishname"));
		title->initWithFile(titleName);

		ui::TextBMFont* label = (ui::TextBMFont*)(parentNode->getChildByName("bonus_num"));
		label->setString(StringUtil::format("%d", moneyCount));

	}
	parentNode->stopAllActions();
	parentNode->setScale(0.2);
	parentNode->setOpacity(0);
	parentNode->setVisible(false);
	auto swapAct = Spawn::create(ScaleTo::create(0.2, 1.2), FadeTo::create(0.2, 100), NULL);
	auto swapAct2 = Spawn::create(ScaleTo::create(0.16, 0), FadeTo::create(0.16, 0), NULL);
	auto call = CallFunc::create([](){AudioControl::getInstance()->playEffect("sound/rolling_02.mp3"); });
	parentNode->runAction(Sequence::create(DelayTime::create(delayTime), call,Show::create(), swapAct, ScaleTo::create(0.12, 1), DelayTime::create(2.5), ScaleTo::create(0.04, 1.2), swapAct2, RemoveSelf::create(), nullptr));

}

bool CoinManager::playCppEffect(int playType, float delayTime, Node* parent, Vec2 pos)
{
	if (playType == 1)
	{
		setBlastEff(delayTime, parent, pos);
	}
	else if (playType == 2)
	{
		setBlastCoinEff(delayTime, parent, pos);
	}
	else if (playType == 3)
	{
		setBlastCircleEff(delayTime, parent, pos);
	}
	else if (playType == 4)
	{
		setShakeBgEff(delayTime);
	}
	else
	{
		
	}
	return true;
}

void CoinManager::setBlastEff(float delay, Node* parent, Vec2 pos)
{
	if (parent == nullptr)
	{
		parent = mCoinLayer;
	}
	//±¬Õ¨
	auto blast = Sprite::create();
	blast->setScale(1.5f);
	blast->setPosition(pos);
	parent->addChild(blast, 201);

	auto animation = Animation::create();

	for (int i = 0; i < 8; i++)
	{
		string filename = StringUtil::format("battle/effect/blast1_0%d.png", i);
		animation->addSpriteFrameWithFile(filename);
	}
	animation->setDelayPerUnit(1.0f / 15.0f);
	blast->runAction(Sequence::create(DelayTime::create(delay), Animate::create(animation), RemoveSelf::create(), NULL));
}

void CoinManager::setBlastCoinEff(float delay, Node* parent, Vec2 pos)
{
	if (parent == nullptr)
	{
		parent = mCoinLayer;
	}
	//±¬½ð±Ò
	auto coin2 = Sprite::create();
	parent->addChild(coin2, 200);
	coin2->setScale(1.5);
	coin2->setPosition(pos);
	auto animation2 = Animation::create();
	int gameType = mGameType;
	//log("------------------setBlastCoinEff------------gameType=%d", gameType);
	for (int i = 0; i < 9; i++)
	{
		if (gameType == 1)
		{
			string filename = StringUtil::format("battle/effect/fl_blast2_0%d.png", i);
			animation2->addSpriteFrameWithFile(filename);
		}
		else
		{
			string filename = StringUtil::format("battle/effect/blast2_0%d.png", i);
			animation2->addSpriteFrameWithFile(filename);
		}
	}
	animation2->setDelayPerUnit(1.0f / 12.0f);
	auto Animate2 = Animate::create(animation2);
	coin2->runAction(Sequence::create(DelayTime::create(delay), Animate2, RemoveSelf::create(), NULL));

}

void CoinManager::setBlastCircleEff(float delay, Node* parent, Vec2 pos)
{
	if (parent == nullptr)
	{
		parent = mCoinLayer;
	}
	//±¬¹âÈ¦
	auto blastCircle = Sprite::create("battle/effect/blast3.png");
	parent->addChild(blastCircle, 200);
	blastCircle->setPosition(pos);
	blastCircle->setScale(2);
	blastCircle->runAction(ScaleTo::create(1.5f, 12.0f));
	blastCircle->runAction(FadeTo::create(0, 255 * 0.3f));
	blastCircle->runAction(FadeTo::create(0.05, 255));
	blastCircle->runAction(Sequence::create(DelayTime::create(delay), FadeTo::create(0.3f, 0), RemoveSelf::create(), NULL));

}

void CoinManager::setShakeBgEff(float delay)
{
	this->runAction(Sequence::create(DelayTime::create(delay), CallFunc::create(
		[&](){
		ValueMap val;
		LuaCppAdapter::getInstance()->callLuaFunc("FishGMF", "CppToLua", "shakeBackground", val);
	}), nullptr));
}

cocos2d::Vec2 CoinManager::getAimPosByPlayerId(int playerId)
{
	Vec2 pos = Vec2(0, 0);
	Player* player = PlayerManager::getInstance()->getPlayerById(playerId);
	if (player != nullptr)
	{
		pos = player->getRoleData()->curPos;
	}
	return pos;
}

void CoinManager::setGameType(int gameType)
{
	mGameType = gameType;
	log("----------setGameType-----------1--------mGameType=%d", mGameType);
	for (int i = 0; i < mSelfCoinPool->count(); i++)
	{
		Coin* coin = dynamic_cast<Coin*>(mSelfCoinPool->objectAtIndex(i));
		if (!coin->getParent())
		{
			coin->initAnimation(true);
		}
	}
	for (int i = 0; i < mOtherCoinPool->count(); i++)
	{
		Coin* coin = dynamic_cast<Coin*>(mOtherCoinPool->objectAtIndex(i));
		if (!coin->getParent())
		{
			coin->initAnimation(false);
		}
	}

}


