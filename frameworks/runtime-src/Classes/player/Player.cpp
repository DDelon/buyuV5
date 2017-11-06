#include "Player.h"
#include "StringUtil.h"
#include "PlayerManager.h"
#include "LuaCppAdapter.h"
#include "DebugLog.h"

void Player::shoot(int propId, int rate)
{
	PropData propData = mRoleData->getPropData(propId);
	if (propData.realCount >= rate)
	{
		useProp(propId, rate);
	}
}

void Player::useProp(int propId,int count)
{
	if (count > 0)
	{
		mRoleData->getPropData(propId).realCount -= count;

		PropFlyingRfreshData refreshData;
		refreshData.playerId = mRoleData->playerId;
		refreshData.propId = propId;
		refreshData.flyingCount = 0;
		//CCLOG("-------------useProp id:%d count:%d",propId,count);
		PlayerManager::getInstance()->refreshUI(&refreshData);
	}
}

void Player::cannonRateChange(int rate)
{
	if (rate > 0 && mRoleData->maxGunRate != -1)
	{
		mRoleData->currentGunRate = rate;
	}
}

bool Player::init()
{
	Node::init();
	mRoleData = RoleData::create();
	this->addChild(mRoleData);

	mAddGoldText = NULL;
	mGoldText = NULL;
	mGemText = NULL;


	mScoreText = NULL;
	mAddScoreText = NULL;
	mBulletText = NULL;

	return true;
}

void Player::refreshGoldShow(int showCount, int addCount)
{
	//printf("------------refreshGoldShow-playerId=%d,showCount=%d,addCount=%d--------\n", getRoleData()->playerId, showCount, addCount);
	//破产解除
	if (getRoleData()->getIsBankup() == true && showCount >0)
	{
		printf("-cahnge--isBankup=false-");
		getRoleData()->setIsBankup(false);
		ValueMap val;
		val["playerId"] = getRoleData()->playerId;
		val["isBankup"] = false;
		LuaCppAdapter::getInstance()->callLuaFunc("FishGMF", "CppToLua", "OnPlayerBankup", val);
	}

	//判断钱是否低于房间最低分
	int myPlayerId = PlayerManager::getInstance()->getMyPlayerId();
	if (myPlayerId == getRoleData()->playerId)
	{
		int curRoomMinRate = PlayerManager::getInstance()->getCurRoomMinRate();
		if (showCount > curRoomMinRate && getRoleData()->currentGunRate < curRoomMinRate)
		{
			//炮倍回复
			ValueMap val;
			val["playerId"] = getRoleData()->playerId;
			val["gunRate"] = curRoomMinRate;
			LuaCppAdapter::getInstance()->callLuaFunc("FishGMF", "CppToLua", "gunRateRevert", val);
		}
	}

// 	//
// 	if (mGoldText != NULL && myPlayerId == getRoleData()->playerId)
// 	{
// 		string num = mGoldText->getString();
// 		if (num != IntToString(showCount))
// 		{
// 			string str = StringUtil::format("------------refreshGoldShow-playerId=%d,trueCoin=%d,showCount=%d,addCount=%d--------", myPlayerId, getRoleData()->getGlod().realCount, showCount, addCount);
// 			DebugLog::getInstance()->log(str);
// 		}
// 	}
// 	//

	if (addCount > 0)
	{
		if (mGoldText != NULL)
		{
			mGoldText->stopAllActions();
			mGoldText->setScale(1);
			mGoldText->runAction(Sequence::createWithTwoActions(ScaleTo::create(0.1, 1.7), ScaleTo::create(0.3, 1)));
		}
		if (mAddGoldText != NULL)
		{
			int tmpData = reinterpret_cast<long>(mAddGoldText->getUserData());
			//CCLOG("-refreshGoldShow--addCount=%d", addCount + tmpData);
			//CCLOG("refreshGoldShow3");
			mAddGoldText->setString(StringUtil::format("+%d", addCount + tmpData));
			mAddGoldText->setUserData((void*)(addCount + tmpData));
			mAddGoldText->stopAllActions();
			mAddGoldText->setOpacity(255);
			mAddGoldText->setScale(0.7f);
			mAddGoldText->runAction(Sequence::create(CallFunc::create([=](){
				mAddGoldText->setUserData((void*)0);
			}), Show::create(), DelayTime::create(0.8f), FadeTo::create(0.2, 0),

				NULL));
		}
	}

	if (mGoldText != NULL)
	{
		mGoldText->setString(IntToString(showCount));
	}
}

//显示水晶数据ui
void Player::refreshGemShow(int showCount, int addCount)
{
	int myPlayerId = PlayerManager::getInstance()->getMyPlayerId();
	if (myPlayerId == getRoleData()->playerId && showCount >= 0)
	{
		//printf("-------------------change--refreshGemShow-showCount=%d--------addCount=%d\n", showCount, addCount);
		ValueMap val;
		val["playerId"] = getRoleData()->playerId;
		val["newCrystal"] = showCount;
		LuaCppAdapter::getInstance()->callLuaFunc("FishGMF", "CppToLua", "updataGemUI", val);
	}

	if (mGemText != NULL)
	{
		if (addCount > 0)
		{
			mGemText->stopAllActions();
			mGemText->setScale(1);
			mGemText->runAction(Sequence::createWithTwoActions(ScaleTo::create(0.1, 1.7), ScaleTo::create(0.3, 1)));
		}
		mGemText->setString(IntToString(showCount));
	}
}

void Player::setGoldTextView(ui::TextBMFont* view)
{
	mGoldText = view; 
}

void Player::setAddGoldTextTextView(ui::TextBMFont* view)
{
	 mAddGoldText = view; 
	 if (mAddGoldText!=NULL) (mAddGoldText->setUserData((void*)0)); 
}

void Player::setMoneyUI(int propId, ui::TextBMFont* text, ui::TextBMFont* addText)
{

	if (propId == GOLD)
	{
		if (mGoldText != NULL)
		{
			mGoldText->stopAllActions();
			mGoldText = NULL;
		}
		if (mGoldText != NULL)
		{
			mAddGoldText->stopAllActions();
			mAddGoldText = NULL;
		}
		mGoldText = text;
		mAddGoldText = addText;
		if (mAddGoldText != NULL) (mAddGoldText->setUserData((void*)0));
		getRoleData()->refreshPropData(GOLD, getRoleData()->getPropData(GOLD).realCount, 0, 0);
	}
	else if (propId == GEM)
	{
		if (mGemText != NULL)
		{
			mGemText->stopAllActions();
			mGemText = NULL;
		}
		mGemText = text;
		getRoleData()->refreshPropData(GEM, getRoleData()->getPropData(GEM).realCount, 0, 0);
	}
	else if (propId == SCORE)
	{
		if (mScoreText != NULL)
		{
			mScoreText->stopAllActions();
			mScoreText = NULL;
		}
		if (mAddScoreText != NULL)
		{
			mAddScoreText->stopAllActions();
			mAddScoreText = NULL;
		}
		mScoreText = text;
		mAddScoreText = addText;
		if (mAddScoreText != NULL) (mAddScoreText->setUserData((void*)0));
		getRoleData()->refreshPropData(SCORE, getRoleData()->getPropData(SCORE).realCount, 0, 0);
	}
	else if (propId == BULLET)
	{
		if (mGemText != NULL)
		{
			mBulletText->stopAllActions();
			mBulletText = NULL;
		}
		mBulletText = text;
		getRoleData()->refreshPropData(BULLET, getRoleData()->getPropData(BULLET).realCount, 0, 0);
	}
}

void Player::refreshPropShow(int propId, int showCount, int addCount /*= 0*/, int propItemId /*= 0*/, ValueMap data /*= ValueMap()*/)
{
	//printf("-----------refreshPropShow--------propId=%d------showCount=%d------------\n", propId, showCount);
	ValueMap sendData;
	sendData["playerId"] = getRoleData()->playerId;
	sendData["propId"] = propId;
	sendData["propCount"] = showCount;
	sendData["realCount"] = getRoleData()->getPropData(propId).realCount;
	sendData["mGameState"] = PlayerManager::getInstance()->getGameState();
	sendData["propItemId"] = propItemId;
	sendData["seniorData"] = data;
	LuaCppAdapter::getInstance()->callLuaFunc("FishGMF", "CppToLua", "updataPropUI", sendData);


}

//更新积分
void Player::refreshScoreShow(int showCount, int addCount /*= 0*/)
{
	if (addCount > 0)
	{
		if (mScoreText != NULL)
		{
			mScoreText->stopAllActions();
			mScoreText->setScale(1);
			mScoreText->runAction(Sequence::createWithTwoActions(ScaleTo::create(0.1, 1.7), ScaleTo::create(0.3, 1)));
		}
		if (mAddScoreText != NULL)
		{
			int tmpData = reinterpret_cast<long>(mAddScoreText->getUserData());
			//CCLOG("-refreshGoldShow--addCount=%d", addCount + tmpData);
			//CCLOG("refreshGoldShow3");
			mAddScoreText->setString(StringUtil::format("+%d", addCount + tmpData));
			mAddScoreText->setUserData((void*)(addCount + tmpData));
			mAddScoreText->stopAllActions();
			mAddScoreText->setOpacity(255);
			mAddScoreText->setScale(0.7f);
			mAddScoreText->runAction(Sequence::create(CallFunc::create([=](){
				mAddScoreText->setUserData((void*)0);
			}), Show::create(), DelayTime::create(0.8f), FadeTo::create(0.2, 0),

				NULL));
		}
	}


	if (mScoreText != NULL)
	{
		string scoreStr = mScoreText->getString();
		int score = atoi(scoreStr.c_str());
		if (score != showCount)
		{
			//刷新排行
			ValueMap val;
			val["playerId"] = getRoleData()->playerId;
			val["score"] = showCount;
			LuaCppAdapter::getInstance()->callLuaFunc("FishGMF", "CppToLua", "resetRankList", val);

		}
		mScoreText->setString(IntToString(showCount));
	}

}

//更新子弹个数
void Player::refreshBulletShow(int showCount, int addCount /*= 0*/)
{
	if (addCount > 0)
	{
		if (mBulletText != NULL)
		{
			mBulletText->stopAllActions();
			mBulletText->setScale(1);
			mBulletText->runAction(Sequence::createWithTwoActions(ScaleTo::create(0.1, 1.7), ScaleTo::create(0.3, 1)));
		}
	}

	if (mBulletText != NULL)
	{
		mBulletText->setString(IntToString(showCount));
	}
}
