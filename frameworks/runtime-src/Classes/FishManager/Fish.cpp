#include "Fish.h"
#include "ParseGameData.h"
#include "StringUtil.h"
#include "FishFrameManager.h"
#include "LuaCppAdapter.h"
#include "DebugLog.h"
#include "audio/include/SimpleAudioEngine.h"
#include "AudioControl.h"
#include "CoinManager.h"

#define  SHADOW_OFFSETX  20
#define  SHADOW_OFFSETY  -20
Fish* Fish::create()
{
	Fish *pRet = new(std::nothrow) Fish();
	if (pRet && pRet->init())
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

bool Fish::init()
{
	CollisionNode::init();
	mPathOffset = Vec2(0, 0);
	
	mIsAlive = false;
	mPathPoint = NULL;
	mSpriteShadow = NULL;
	mIsCallFish = false;

	return true;
}

void Fish::actionInFrame(int frameIndex)
{
	if (mFishState == FishState::DEATH || mFishState == FishState::PAUSE)
	{
		return;
	}
	if (mTimelineId < 0)
	{
		frameIndex = mCurRunFrame+1;
	}
	else
	{
		frameIndex = (frameIndex - mBornFrame) < 0 ? 0 : (frameIndex - mBornFrame);

	}
	
	if (mPathPoint->size()==0)
	{
		return;
	}
	int curFrame = (mStartAcceFrame == 0 ? frameIndex : frameIndex + (frameIndex - mStartAcceFrame)*mSpeed);
	if (curFrame >= mPathPoint->size() *  PATH_INTERVAL + 30)
	{
		fadeOut();
		//removeFromScreen();
		return;
	}

	
	
	if (curFrame < mPathPoint->size() *  PATH_INTERVAL)
	{
	
		if (frameIndex == 0)
		{
			
			Vec3 point = getPathPoint(frameIndex);
			this->setPosition(Vec2(point.x, point.y));
		}
		else if (frameIndex % PATH_INTERVAL == 0)
		{
			
			this->updatePoints();
			int pathFrame = (mStartAcceFrame == 0 ? frameIndex : frameIndex + (frameIndex - mStartAcceFrame)*mSpeed);
			Vec3 point = getPathPoint(pathFrame);
			this->stopActionByTag(MOVEING_ACTION_TAG);
			if (mFishState == FishState::FREEZE)
			{
				this->setPosition(Vec2(point.x, point.y));
			}
			else
			{
				auto action = MoveTo::create(1.0f / 20.0f * PATH_INTERVAL, Vec2(point.x, point.y));
				action->setTag(MOVEING_ACTION_TAG);
				this->runAction(action);
			}
			float tempRotate = this->getRotation();
			if (mRoatateType==0)
			{
				this->setRotation(point.z + 90);
			}
			else if (mRoatateType == 1)
			{
				this->setRotation(0);
			}
			else if (mRoatateType==2)
			{
				float angle = point.z + 90;
				this->setRotation(angle);
				if (angle > 90 && angle < 270)
				{
					if (!mIsFliped)
					{
						mIsFliped = true;
						for (int i = 0; i < mSpriteVec.size();i++)
						{
							auto sprite = mSpriteVec[i];
							sprite->setFlipY(true);
						}
						for (int i = 0; i < mHaloVec.size(); ++i)
						{
							Vec2 pos = mHaloVec[i]->getPosition();
							pos.y = -pos.y;
							mHaloVec[i]->setPosition(pos);
						}
						flipY();
						lockPointFlipY();
					}
				}
				else
				{
					if (mIsFliped)
					{
						mIsFliped = false;
						for (int i = 0; i < mSpriteVec.size(); i++)
						{
							auto sprite = mSpriteVec[i];
							sprite->setFlipY(false);
						}
						for (int i = 0; i < mHaloVec.size(); ++i)
						{
							Vec2 pos = mHaloVec[i]->getPosition();
							pos.y = -pos.y;
							mHaloVec[i]->setPosition(pos);
						}
						flipY();
						lockPointFlipY();
					}
				}
			}
			else if (mRoatateType == 3)
			{
				this->setRotation(0);
				float angle = point.z + 90;

				if (angle > 90 && angle < 270)
				{
					if (!mIsFliped)
					{
						mIsFliped = true;
						for (int i = 0; i < mSpriteVec.size(); i++)
						{
							auto sprite = mSpriteVec[i];
							sprite->setFlipX(true);
						}
						for (int i = 0; i < mHaloVec.size(); ++i)
						{
							Vec2 pos = mHaloVec[i]->getPosition();
							pos.x = -pos.x;
							mHaloVec[i]->setPosition(pos);
						}
						flipX();
						lockPointFlipX();
					}
				}
				else
				{
					if (mIsFliped)
					{
						mIsFliped = false;
						for (int i = 0; i < mSpriteVec.size(); i++)
						{
							auto sprite = mSpriteVec[i];
							sprite->setFlipX(false);
						}
						for (int i = 0; i < mHaloVec.size(); ++i)
						{
							Vec2 pos = mHaloVec[i]->getPosition();
							pos.x = -pos.x;
							mHaloVec[i]->setPosition(pos);
						}
						flipX();
						lockPointFlipX();
					}
				}

			}
			
		}
		
	}
	mCurRunFrame = frameIndex;

	if (mSpriteShadow != NULL)
	{
		Vec2 pos = this->getPosition() + Vec2(SHADOW_OFFSETX, SHADOW_OFFSETY);
		Vec2 newPos = this->convertToNodeSpaceAR(pos);
		mSpriteShadow->setPosition(newPos);
	}

}

bool Fish::isCanCheck()
{
	return ((mFishState == FishState::DEATH || mFishState == FishState::FADE) ? false : true);
}

ValueMap Fish::dealNodeCollision(CollisionNode *pOther)
{


	return ValueMap();
}

void Fish::lockPointFlipY()
{
	/*mLockOffset.y = -mLockOffset.y;
	for (auto iter : mHandAimPoint)
	{
		iter.y = -iter.y;
	}*/
	for (auto iter = mHandAimPoint.begin(); iter != mHandAimPoint.end(); ++iter)
	{
		iter->y = -iter->y;
	}
	for (auto iter : mLockVec)
	{
		iter->setPositionY(-iter->getPositionY());
	}
	//this->addLockPointUI();
}

void Fish::lockPointFlipX()
{
	/*mLockOffset.x = -mLockOffset.x;
	for (auto iter : mHandAimPoint)
	{
		iter.x = -iter.x;
	}*/
	for (auto iter = mHandAimPoint.begin(); iter != mHandAimPoint.end(); ++iter)
	{
		iter->x = -iter->x;
	}

	for (auto iter : mLockVec)
	{
		iter->setPositionX(-iter->getPositionX());
	}
}

void Fish::selectLockPoint(Vec2 pos)
{

	//DebugLog::getInstance()->log("pos x:" + Value(pos.x).asString() + " pos y:" + Value(pos.y).asString());
	//默认的锁定点也加入这个容器
	if (mHandAimPoint.size() > 1)
	{
		//有手动配置的锁定点
		float minDistance = mHandAimPoint[0].distance(pos);
		Vec2 nearestVec2 = mHandAimPoint[0];
		for (auto iter = mHandAimPoint.begin(); iter != mHandAimPoint.end(); ++iter)
		{
			float distance = (*iter).distance(pos);
			if (distance < minDistance)
			{
				minDistance = distance;
				nearestVec2 = *iter;
			}
		}
		//设置为锁定点
		mLockOffset = nearestVec2;
	}
}

void Fish::accelerate()
{
	mStartAcceFrame = mCurRunFrame;
	mFishState = FishState::ACCELERATE;

	mSpeed = GAMEVALUE_INT("config", "990000018", "data");
	this->fadeOut();
}

Vec3 Fish::getPathPoint(int frame)
{
	
	Size winSize = Director::getInstance()->getWinSize();
	float widthPercent = winSize.width / BASE_RESOLUTION_WIDTH;
	float heightPercent = winSize.height / BASE_RESOLUTION_HEIGHT;

	Vec3 point = (*mPathPoint)[(int)frame / PATH_INTERVAL];
	
	point.x = (point.x + mPathOffset.x)*widthPercent;
	point.y = (point.y + mPathOffset.y)*heightPercent;
	bool isFlip = FishFrameManager::getInstance()->getFilpVal();
	if (isFlip)
	{
		Size winSize = Director::getInstance()->getWinSize();
		point = Vec3(winSize.width - point.x, winSize.height-point.y, point.z-180);
	}
	
	return point;
}

void Fish::setOffset(Vec2 offset)
{
	mPathOffset = offset;
}

void Fish::setFishId(int fishId, int timelineId, int fishArrayId, bool isCallFish)
{
	mSpeed = 1;
	mStartAcceFrame = 0;
	mIsHit = false;
	mFishState = FishState::MOVE;

	mSpriteVec.clear();
	mHandAimPoint.clear();
	mDiscVec.clear();
	mHaloVec.clear();
	mIsFliped = false;
	mIsAlive = true;
	mId = fishId;
	mTimelineId = timelineId;
	mFishArrayId = fishArrayId;
	mIsCallFish = isCallFish;
	
	this->setName(StringUtil::format("%d%d", timelineId, fishArrayId).c_str());

	std::string idStr = IntToString(fishId);
	std::string filename = GAMEVALUE_STRING("fish", idStr, "fish_res");
	//mFishType 6局部炸弹 7闪电鱼 8同类炸弹
	mFishType = GAMEVALUE_INT("fish", idStr, "trace_type");

	mRoatateType = GAMEVALUE_INT("fish", idStr, "rotate_type");
	int zOrder = GAMEVALUE_INT("fish", idStr, "show_layer");
	std::string addHaloSign = GAMEVALUE_STRING("fish", idStr, "halo_res");
	mScore = GAMEVALUE_INT("fish", idStr, "score");
	mDeathEffect = GAMEVALUE_INT("fish", idStr, "death_effect");
	mMusic = GAMEVALUE_STRING("fish", idStr, "music_res");
	mPlayMusicRate = GAMEVALUE_INT("fish", idStr, "music_rate");
	mCoinNum = GAMEVALUE_INT("fish", idStr, "coin_num");

	std::vector<std::string> lockOffsetVec;
	StringUtil::_STR_SPLIT(GAMEVALUE_STRING("fish", idStr, "aim_offset"), ",", lockOffsetVec);
	mOtherLockOffset = Vec2(Value(lockOffsetVec[0]).asInt(), Value(lockOffsetVec[1]).asInt());
	mLockOffset = mOtherLockOffset;

	this->setLocalZOrder(zOrder);
	if ((mFishType == 5 || mFishType == 10 ) && !mIsCallFish)
	{
		ValueMap val;
		val["score"] = mScore;
		val["id"] = mId;
		LuaCppAdapter::getInstance()->callLuaFunc("FishGMF", "CppToLua", "bossComming", val);
	}

	if (mFishType != 4 && mFishType != 8)
	{
		float frameDelay = GAMEVALUE_INT("fish", idStr, "action_anim") / 20.0f;
		float scale = GAMEVALUE_FLOAT("fish", idStr, "res_scale");
		if (filename.size() == 0)
		{
			filename = "fishid11";
		}
		auto sprite = Sprite::createWithSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(filename + "_00.png"));
		auto action = initAnimation(filename, frameDelay);
		sprite->runAction(action);
		sprite->setScale(scale);
		this->addChild(sprite, 10);
		mSpriteVec.push_back(sprite);

		mSpriteShadow = Sprite::create();
		mSpriteShadow->setColor(Color3B::BLACK);
		mSpriteShadow->setOpacity(this->getOpacity() / 3);
		auto act = action->clone();
		act->setTag(STANDING_ACTION_TAG);
		mSpriteShadow->runAction(act);
		mSpriteShadow->setScale(scale);
		mSpriteShadow->setTag(SHADOW_TAG);
		mSpriteShadow->setPosition(Vec2(sprite->getPosition().x + SHADOW_OFFSETX, sprite->getPosition().y + SHADOW_OFFSETY));
		this->addChild(mSpriteShadow, -1);
		
		if (addHaloSign != "0")
		{
			int bossHaloActType = GAMEVALUE_INT("fish", idStr, "halo_type");
			std::vector<std::string> bgNameVec;
			StringUtil::_STR_SPLIT(GAMEVALUE_STRING("fish", idStr, "halo_res"), ";", bgNameVec);
			std::vector<std::string> haloOffsetVec;
			StringUtil::_STR_SPLIT(GAMEVALUE_STRING("fish", idStr, "halo_pos"), ";", haloOffsetVec);
			if (bossHaloActType == 1)
			{
				for (int i = 0; i < bgNameVec.size(); ++i)
				{
					auto spriteBg = Sprite::create("fish/" + bgNameVec[i]);
					std::vector<std::string> offsetVec;
					StringUtil::_STR_SPLIT(haloOffsetVec[0], ",", offsetVec);
					spriteBg->setPosition(Vec2(atof(offsetVec[0].c_str()), atof(offsetVec[1].c_str())));
					this->addChild(spriteBg, 1);
					mHaloVec.push_back(spriteBg);
					if (i == bgNameVec.size() - 1)
					{
						spriteBg->setScaleX(0);
						spriteBg->setScaleY(0);
						auto scale = Sequence::create(ScaleTo::create(1.0, 1), ScaleTo::create(0, 0), NULL);

						auto fadeto = Sequence::create(FadeTo::create(0.8, 204), FadeTo::create(0.2, 0), FadeTo::create(0, 255), NULL);
						auto repeat = RepeatForever::create(Sequence::create(Spawn::create(scale, fadeto, NULL), DelayTime::create(0), NULL));
						repeat->setTag(HALO_ACTION_TAG);
						spriteBg->runAction(repeat);
					}
					
				}
			}
			else if (bossHaloActType == 2)
			{
				for (int i = 0; i < bgNameVec.size(); ++i)
				{
					auto spriteBg = Sprite::create("fish/" + bgNameVec[0]);
					std::vector<std::string> offsetVec;
					StringUtil::_STR_SPLIT(haloOffsetVec[0], ",", offsetVec);
					spriteBg->setPosition(Vec2(atof(offsetVec[0].c_str()), atof(offsetVec[1].c_str())));
					spriteBg->setScale(0.9);
					auto repeat = RepeatForever::create(RotateBy::create(12, 360));
					repeat->setTag(DISC_ACTION_TAG);
					spriteBg->runAction(repeat);
					this->addChild(spriteBg, 1);
					mDiscVec.push_back(spriteBg);
				}
			}
			else
			{
				for (int i = 0; i < bgNameVec.size(); ++i)
				{
					auto spriteBg = Sprite::create("fish/" + bgNameVec[0]);
					std::vector<std::string> offsetVec;
					StringUtil::_STR_SPLIT(haloOffsetVec[0], ",", offsetVec);
					spriteBg->setPosition(Vec2(atof(offsetVec[0].c_str()), atof(offsetVec[1].c_str())));
					this->addChild(spriteBg, 1);
					mHaloVec.push_back(spriteBg);
				}
				
			}
			
			
		}

		mSpriteVec.push_back(mSpriteShadow);
		this->setContentSize(sprite->getContentSize());

		this->initCollisionPoint(this->getBoundingBox(), getCollisionVecByStr(GAMEVALUE_STRING("fish", idStr, "point_info")));
		
	}
	else
	{

		int fishChildrenId = fishId + 90000000;
		std::string fishChildrenIdStr = IntToString(fishChildrenId);
		int fishCount = GAMEVALUE_INT("fishchildren", fishChildrenIdStr, "fishcount");
		vector<std::string> bgIndexVec;
		vector<std::string> bgScaleVec;
		vector<std::string> fishIdVec;
		vector<std::string> fishScaleVec;
		vector<std::string> offsetVec;

		StringUtil::_STR_SPLIT(GAMEVALUE_STRING("fishchildren", fishChildrenIdStr, "bgindex"), ";", bgIndexVec);
		StringUtil::_STR_SPLIT(GAMEVALUE_STRING("fishchildren", fishChildrenIdStr, "bgscale"), ";", bgScaleVec);
		StringUtil::_STR_SPLIT(GAMEVALUE_STRING("fishchildren", fishChildrenIdStr, "fishid"), ";", fishIdVec);
		StringUtil::_STR_SPLIT(GAMEVALUE_STRING("fishchildren", fishChildrenIdStr, "fishscale"), ";", fishScaleVec);
		StringUtil::_STR_SPLIT(GAMEVALUE_STRING("fishchildren", fishChildrenIdStr, "offset"), ";", offsetVec);

		for (int i = 0; i < fishCount;i++)
		{
			std::string childFishIdStr = IntToString(FISH_ID_START + atoi(fishIdVec[i].c_str()));
			mSameFishId = atoi(childFishIdStr.c_str());
			float frameDelay = GAMEVALUE_INT("fish", childFishIdStr, "action_anim") / 20.0f;
			std::string filename = GAMEVALUE_STRING("fish", childFishIdStr, "fish_res");

			auto sprite = Sprite::createWithSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(filename + "_00.png"));
			vector<std::string> offsetXYVec;
			StringUtil::_STR_SPLIT(offsetVec[i], ",", offsetXYVec);
			sprite->setPosition(Vec2(atof(offsetXYVec[0].c_str()),atof(offsetXYVec[1].c_str())));
			sprite->setScale(atof(fishScaleVec[i].c_str()) / 100.0f);
			auto action = initAnimation(filename, frameDelay);
			sprite->runAction(action);
			this->addChild(sprite,10);

			auto spriteBg = Sprite::create(StringUtil::format("fish/fish_com_%s.png", bgIndexVec[i].c_str()));
			spriteBg->setScale(atof(bgScaleVec[i].c_str()) / 100.0f);
			auto actBg = RepeatForever::create(RotateBy::create(1, 72));
			actBg->setTag(DISC_ACTION_TAG);
			spriteBg->runAction(actBg);
			spriteBg->setPosition(Vec2(atof(offsetXYVec[0].c_str()), atof(offsetXYVec[1].c_str())));
			this->addChild(spriteBg, 1);
			mDiscVec.push_back(spriteBg);
			mSpriteVec.push_back(sprite);
		}


		Size size = getGroupFishContentSize();
		this->setContentSize(size);
		this->initCollisionPoint(this->getBoundingBox(), getCollisionVecByStr(GAMEVALUE_STRING("fish", idStr, "point_info")));
	}

	

	std::string handAimStr = GAMEVALUE_STRING("fish", idStr, "hand_aim");
	if (handAimStr.size() > 0)
	{
		//解析手动配置锁定点
		std::vector<std::string> handAmiVec;
		StringUtil::_STR_SPLIT(handAimStr, ";", handAmiVec);
		for (auto iter : handAmiVec)
		{
			if (iter.size() == 0)
			{
				continue;
			}
			std::vector<std::string> amiPos;
			StringUtil::_STR_SPLIT(iter, ",", amiPos);
			mHandAimPoint.push_back(Vec2(Value(amiPos[0]).asFloat(), -Value(amiPos[1]).asFloat()));
		}
		mHandAimPoint.push_back(mLockOffset);
	}

	setIsRed(false);
	

	//this->setPosition(Vec2(this->getContentSize().width / 2, this->getContentSize().height/2));
	/*
		数据添加到lua那边
	*/

	//ValueMap data = getLuaFishPackage();
	
	//LuaCppAdapter::getInstance()->callLuaFunc("FishDataTab", "onMsg", 1, data);
}

void Fish::getEffectedFishes(int bulletRate, ValueVector &fishTab)
{
	fishTab.clear();
	//DebugLog::getInstance()->log("timelineId: " + Value(mTimelineId).asString() + " fishArrayId: " + Value(mFishArrayId).asString());
	std::vector<Fish *> effectedFishes;
	if (mFishType == 6)
	{
		//局部炸弹 范围100
		FishFrameManager::getInstance()->getRangeFish(GAMEVALUE_INT("config", "990000025", "data"), this->getPosition(), effectedFishes);
		
	}
	else if (mFishType == 7)
	{
		//连锁闪电
		
		FishFrameManager::getInstance()->getThunderPool(bulletRate, effectedFishes);
	}
	else if (mFishType == 8)
	{
		//同类炸弹
		FishFrameManager::getInstance()->getSameFishType(mSameFishId, effectedFishes);
		//FishFrameManager::getInstance()->getThunderPool(effectedFishes);
		//FishFrameManager::getInstance()->getRangeFish(100, this->getPosition(), effectedFishes);
		
	}

	for (auto iter : effectedFishes)
	{
		ValueMap fishData;
		fishData["timelineId"] = iter->getTimelineId();
		fishData["fishArrayId"] = iter->getFishArrayId();

		fishTab.push_back(Value(fishData));
	}
}

Size Fish::getGroupFishContentSize()
{
	float minX = mSpriteVec[0]->getPositionX() - mSpriteVec[0]->getContentSize().width / 2;
	float minY = mSpriteVec[0]->getPositionY() - mSpriteVec[0]->getContentSize().height / 2;
	float maxX = mSpriteVec[0]->getPositionX() + mSpriteVec[0]->getContentSize().width / 2;
	float maxY = mSpriteVec[0]->getPositionY() + mSpriteVec[0]->getContentSize().height / 2;

	for (auto iter = mSpriteVec.begin() + 1; iter != mSpriteVec.end(); ++iter)
	{
		float otherMinX = (*iter)->getPositionX() - (*iter)->getContentSize().width / 2;
		float otherMinY = (*iter)->getPositionY() - (*iter)->getContentSize().height / 2;
		float otherMaxX = (*iter)->getPositionX() + (*iter)->getContentSize().width / 2;
		float otherMaxY = (*iter)->getPositionY() + (*iter)->getContentSize().height / 2;

		if (otherMinX < minX)
		{
			minX = otherMinX;
		}
		else if (otherMinY < minY)
		{
			minY = otherMinY;
		}
		else if (otherMaxX > maxX)
		{
			maxX = otherMaxX;
		}
		else if (otherMaxY > maxY)
		{
			maxY = otherMaxY;
		}
	}

	return Size(maxX-minX, maxY-minY);
}

std::vector<Vec2> Fish::getCollisionVecByStr(std::string str)
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
		pointTabVec2.push_back(Vec2(atof(pointStr[0].c_str()), -atof(pointStr[1].c_str())));
	}

	return pointTabVec2;
}
Vec2 Fish::getLockPosition()
{
	if (this == nullptr)
	{
		return Vec2(0, 0);
	}
	return this->convertToWorldSpace(mLockOffset);
}

Vec2 Fish::getOtherLockPosition()
{
	if (this == nullptr)
	{
		return Vec2(0, 0);
	}
	return this->convertToWorldSpace(mOtherLockOffset);
}

void Fish::setPathId(int pathId, int frame)
{
	if (pathId == -1)
	{
		return;
	}
	int id = pathId;
	mPathPoint = FishFrameManager::getInstance()->getPath(id);
	Vec3 point = getPathPoint(frame);
	this->setPosition(Vec2(point.x, point.y));
	this->setRotation(point.z + 90);
	//if (mTimelineId < 0)
	//{
	this->updatePoints();
	//}
	//this->addLockPointUI();
	//<0是召唤鱼
	if (frame == 0 && mTimelineId > 0)
	{
		fadeIn();
	}
	mCurRunFrame = frame;
	if (mRoatateType == 0)
	{
		this->setRotation(point.z + 90);
	}
	else if (mRoatateType == 1 || mRoatateType == 3)
	{
		this->setRotation(0);
	}
	else if (mRoatateType == 2)
	{
		float angle = point.z + 90;
		this->setRotation(angle);
		if (angle > 90 && angle < 270)
		{
			if (!mIsFliped)
			{
				mIsFliped = true;
				for (int i = 0; i < mSpriteVec.size(); i++)
				{
					auto sprite = mSpriteVec[i];
					sprite->setFlipY(true);
				}
				for (int i = 0; i < mHaloVec.size(); ++i)
				{
					Vec2 pos = mHaloVec[i]->getPosition();
					pos.y = -pos.y;
					mHaloVec[i]->setPosition(pos);
				}
				flipY();
				lockPointFlipY();
			}
		}
		else
		{
			if (mIsFliped)
			{
				mIsFliped = false;
				for (int i = 0; i < mSpriteVec.size(); i++)
				{
					auto sprite = mSpriteVec[i];
					sprite->setFlipY(false);
				}
				for (int i = 0; i < mHaloVec.size(); ++i)
				{
					Vec2 pos = mHaloVec[i]->getPosition();
					pos.y = -pos.y;
					mHaloVec[i]->setPosition(pos);
				}
				flipY();
				lockPointFlipY();
			}
		}
	}
	
}

void Fish::removeFromScreen()
{
	if ((mFishType == 5 || mFishType == 10 ) && !mIsCallFish)
	{
		ValueMap val;
		LuaCppAdapter::getInstance()->callLuaFunc("FishGMF", "CppToLua", "bossLeave", val);
	}
	mLockVec.clear();
	this->clearCollData();
	this->setVisible(true);
	this->setPosition(Vec2(-200, -200));
	mPathOffset = Vec2(0, 0);
	mIsAlive = false;
	mSpriteVec.clear();
	mDiscVec.clear();
	mHaloVec.clear();
	mIsFliped = false;
	mTimelineId = 0;
	mFishArrayId = 0;
	mSpriteShadow = NULL;
	mIsCallFish = false;

	this->stopAllActions();
	this->removeAllChildren();
	this->setScale(1);
	mHandAimPoint.clear();

	this->removeFromParent();
	//_collisionPointVec.clear();
}

Action* Fish::initAnimation(std::string fileName, float delay)
{
	auto animation = Animation::create();
	int i = 0;
	while (true)
	{
		auto str = StringUtil::format("%s_%02d.png", fileName.c_str(), i);
		auto *framename = SpriteFrameCache::getInstance()->getSpriteFrameByName(str);
		if (!framename)
		{
			break;
		}
		animation->addSpriteFrame(framename);
		i++;
	}

	animation->setDelayPerUnit(delay);
	auto action = RepeatForever::create(Animate::create(animation));
	action->setTag(STANDING_ACTION_TAG);
	return action;
}

void Fish::setFishState(int state)
{
	if (mFishState == FishState::PAUSE)
	{
		state = FishState::PAUSE;
	}
	mFishState = state;


	switch (state)
	{
	case FishState::MOVE :
		for (int i = 0; i < mSpriteVec.size(); i++)
		{
			auto ForverAnimate = mSpriteVec[i]->getActionByTag(STANDING_ACTION_TAG);
			if (ForverAnimate != nullptr)
			{
				Director::getInstance()->getActionManager()->resumeTarget(mSpriteVec[i]);
			}
			else
			{
				this->reset();
				this->setFishId(mId, mTimelineId, mFishArrayId);
			}
		}
		for (int i = 0; i < mDiscVec.size(); i++)
		{
			auto ForverAnimate = mDiscVec[i]->getActionByTag(DISC_ACTION_TAG);
			if (ForverAnimate != nullptr)
			{
				Director::getInstance()->getActionManager()->resumeTarget(mDiscVec[i]);
			}
			else
			{
				auto actBg = RepeatForever::create(RotateBy::create(1, 72));
				actBg->setTag(DISC_ACTION_TAG);
				mDiscVec[i]->runAction(actBg);
			}
		}
		for (int i = 0; i < mHaloVec.size(); ++i)
		{
			auto ForverAnimate = mHaloVec[i]->getActionByTag(HALO_ACTION_TAG);
			if (ForverAnimate != nullptr)
			{
				Director::getInstance()->getActionManager()->resumeTarget(mHaloVec[i]);
			}
			else
			{
				if (i == mHaloVec.size() - 1)
				{
					auto scale = Sequence::create(ScaleTo::create(1.0, 1), ScaleTo::create(0, 0), NULL);

					auto fadeto = Sequence::create(FadeTo::create(0.8, 204), FadeTo::create(0.2, 0), FadeTo::create(0, 255), NULL);
					auto repeat = RepeatForever::create(Sequence::create(Spawn::create(scale, fadeto, NULL), DelayTime::create(0), NULL));
					mHaloVec[i]->runAction(repeat);
				}
				
			}
		}
		break;
	case FishState::PAUSE:

		break;
	case FishState::DEATH:
	{
		 if (mMusic == "0")
		{
			 break;
	    }
		int isPlay = RandomHelper::random_int(0,100);
		if (isPlay <= mPlayMusicRate)
		{
// 			ValueMap val;
// 			val["music_res"] = mMusic;
// 			 LuaCppAdapter::getInstance()->callLuaFunc("FishGF", "CppToLua", "playEffect", val);

			 string musice = StringUtil::format("sound/%s", mMusic.c_str());
			 AudioControl::getInstance()->playEffect(musice.c_str());
		}
		break;
	}
	case FishState::FREEZE:
		for (int i = 0; i < mSpriteVec.size();i++)
		{
			auto ForverAnimate = mSpriteVec[i]->getActionByTag(STANDING_ACTION_TAG);
			if (ForverAnimate != nullptr)
			{
				Director::getInstance()->getActionManager()->pauseTarget(mSpriteVec[i]);
			}
		}
		for (int i = 0; i < mDiscVec.size(); i++)
		{
			auto ForverAnimate = mDiscVec[i]->getActionByTag(DISC_ACTION_TAG);
			if (ForverAnimate != nullptr)
			{
				Director::getInstance()->getActionManager()->pauseTarget(mDiscVec[i]);
			}
		}
		for (int i = 0; i < mHaloVec.size(); i++)
		{
			auto ForverAnimate = mHaloVec[i]->getActionByTag(HALO_ACTION_TAG);
			if (ForverAnimate != nullptr)
			{
				Director::getInstance()->getActionManager()->pauseTarget(mHaloVec[i]);
			}

		}
		break;
	case START_FREEZE:
		for (int i = 0; i < mSpriteVec.size(); i++)
		{
			mSpriteVec[i]->stopAllActions();
		}
		for (int i = 0; i < mDiscVec.size(); i++)
		{
			mDiscVec[i]->stopAllActions();
		}
		for (int i = 0; i < mHaloVec.size(); i++)
		{
			mHaloVec[i]->stopAllActions();
		}

		break;

	default:
		break;
	}

}

void Fish::reset()
{
	for (auto iter : mSpriteVec)
	{
		this->removeChild(iter);
	}
	
	for (auto iter : mDiscVec)
	{
		this->removeChild(iter);
	}
	for (auto iter : mHaloVec)
	{
		this->removeChild(iter);
	}
}

void Fish::setFishColor()
{
	if (mFishState == DEATH || mIsHit == true)
	{
		return;
	}
	mIsHit = true;
	setIsRed(true);
	float redTime = GAMEVALUE_FLOAT("config", "990000009", "data");
	float delayTime = GAMEVALUE_FLOAT("config", "990000008", "data");
	this->runAction(Sequence::create(DelayTime::create(redTime),
		CallFunc::create([&]()
		{
			this->setIsRed(false);
		}) ,
		DelayTime::create(delayTime),
		CallFunc::create([&]()
		{
			mIsHit = false;
		}),
		nullptr
		));
}

void Fish::addLockPointUI()
{
	float time = GAMEVALUE_INT("config", "990000087", "data");
	for (auto iter : mHandAimPoint)
	{
		Sprite *pLockPoint = Sprite::create("battle/effect/effect_lock_4.png");
		pLockPoint->setPosition(iter);
		pLockPoint->runAction(Sequence::create(DelayTime::create(time), CallFuncN::create([&](Ref *pSender){
			mLockVec.clear();
		}), RemoveSelf::create(), NULL));
		pLockPoint->runAction(RepeatForever::create(Sequence::create(ScaleTo::create(0.25, 0.5), ScaleTo::create(0.25, 1), NULL)));
		pLockPoint->runAction(Sequence::create(DelayTime::create(time-0.25), FadeOut::create(0.25), NULL));
		mLockVec.push_back(pLockPoint);
		this->addChild(pLockPoint, 100);
	}

}

void Fish::setIsRed(bool isRed)
{
	Color3B color = Color3B(255, 255, 255);
	if (isRed)
	{
		color = Color3B(255, 0, 0);
	}
	for (int i = 0; i < mSpriteVec.size(); i++)
	{
		
		int tag = mSpriteVec[i]->getTag();
		if (tag != SHADOW_TAG)
		{
			mSpriteVec[i]->setColor(color);
		}
		
	}
	for (int i = 0; i < mDiscVec.size(); i++)
	{
		mDiscVec[i]->setColor(color);
	}
	for (int i = 0; i < mHaloVec.size(); i++)
	{
		mHaloVec[i]->setColor(color);
	}


}

void Fish::fadeIn()
{
	float fadeTime = GAMEVALUE_FLOAT("config", "990000017", "data");
	mFishState = FishState::FADE;
	for (int i = 0; i < mSpriteVec.size(); ++i)
	{
		if (mSpriteVec[i]->getTag() == SHADOW_TAG)
		{
			float opacity = mSpriteVec[i]->getOpacity();
			mSpriteVec[i]->setOpacity(0);
			mSpriteVec[i]->runAction(Sequence::create(FadeTo::create(fadeTime, opacity), CallFuncN::create([=](Ref *pSender){ mFishState = FishState::MOVE; }), NULL));
		}
		else
		{
			mSpriteVec[i]->setOpacity(0);
			mSpriteVec[i]->runAction(Sequence::create(FadeIn::create(fadeTime), CallFuncN::create([=](Ref *pSender){ mFishState = FishState::MOVE; }), NULL));
		}
	}
}

void Fish::fadeOut()
{
	float fadeTime = GAMEVALUE_FLOAT("config", "990000051", "data");
	for (int i = 0; i < mSpriteVec.size(); ++i)
	{
		mSpriteVec[i]->runAction(Sequence::create(FadeOut::create(fadeTime), CallFuncN::create([=](Ref *pSender){ 
			this->removeFromScreen();
			mFishState = FishState::MOVE; 
		}), NULL));
	}
	for (int i = 0; i < mDiscVec.size(); ++i)
	{
		mDiscVec[i]->runAction(Sequence::create(FadeOut::create(fadeTime), NULL));
	}
	for (int i = 0; i < mHaloVec.size(); i++)
	{
		mHaloVec[i]->runAction(Sequence::create(FadeOut::create(fadeTime), NULL));
	}
}

void Fish::specialFishDeath(Vec2 startPos, int specialType, bool isPlayAni)
{
	this->setFishState(PAUSE);
	if (specialType == 6)
	{
		this->setFishDeath();
	}
	else if (specialType == 7)
	{
		//闪电
		if (isPlayAni == false)
		{
			this->runAction(Sequence::create(DelayTime::create(2.0f), CallFunc::create([=](){ this->setFishDeath(); }), NULL));
			return;
		}

		auto countAngle = [=](Vec2 startPos, Vec2 endPos)->float{
			float valX = endPos.x - startPos.x;
			float valY = endPos.y - startPos.y;

			return Vec2(valX, valY).getAngle();
		};
		Vec2 endPos = this->getPosition();
		float angle = CC_RADIANS_TO_DEGREES(countAngle(startPos, endPos));
		float distance = endPos.distance(startPos);
		Sprite *thunderSprite = Sprite::create("battle/effect/lightning_00.png");
		float height = thunderSprite->getContentSize().height;
		float scaleYVal = distance / height;
		thunderSprite->setAnchorPoint(Vec2(0.5f, 0));
		thunderSprite->setScaleY(scaleYVal);
		thunderSprite->setRotation(90 - angle);
		thunderSprite->setPosition(startPos);

		Sprite *endLightPoint = Sprite::create("battle/effect/lightningball.png");
		endLightPoint->setScale(0.7);
		endLightPoint->setPosition(endPos);


		auto delayFunc = [&](){
			this->setFishDeath();

		};


		auto animation = Animation::create();
		for (int j = 0; j < 2; j++)
		{
			string filename = StringUtil::format("battle/effect/lightning_0%d.png", j);
			animation->addSpriteFrameWithFile(filename);
		}
		float time = 1.3;
		animation->setDelayPerUnit(1.0f / 10.0f);
		ActionInterval *repeatAni = RepeatForever::create(Animate::create(animation));
		thunderSprite->runAction(repeatAni);
		thunderSprite->runAction(Sequence::create(DelayTime::create(time), CallFunc::create(delayFunc), RemoveSelf::create(), NULL));
		endLightPoint->runAction(Sequence::create(DelayTime::create(time), RemoveSelf::create(), NULL));



		this->getParent()->getParent()->addChild(thunderSprite, 554);
		this->getParent()->getParent()->addChild(endLightPoint, 555);
	}
	else if (specialType == 8)
	{
		Vec2 endPos = this->getPosition();
		Sprite *endLightPoint = Sprite::create("battle/effect/lightningball.png");
		endLightPoint->setPosition(endPos);
		endLightPoint->setScale(0.6);
		endLightPoint->runAction(Sequence::create(DelayTime::create(1.3f), RemoveSelf::create(), NULL));
		this->getParent()->getParent()->addChild(endLightPoint, 555);
		this->runAction(Sequence::create(DelayTime::create(1.3f), CallFunc::create([=](){ this->setFishDeath(); }), NULL));
	}
}

void Fish::addThunderStartPoint()
{
	Sprite *startLightPoint = Sprite::create("battle/effect/lightningball.png");
	startLightPoint->setPosition(this->getPosition());
	startLightPoint->runAction(Sequence::create(DelayTime::create(1.3f), RemoveSelf::create(), NULL));
	this->getParent()->getParent()->addChild(startLightPoint, 555);
}

void Fish::setFishDeath()
{	
	setIsRed(false);
	setFishState(FishState::DEATH);

	if (mFishType == 7 || mFishType == 8)
	{
		if (mFishType == 7)
		{
			AudioControl::getInstance()->playEffect("sound/lightning_01.mp3");
		}
		else
		{
			AudioControl::getInstance()->playEffect("sound/capture_01.mp3");
		}
		this->addThunderStartPoint();
	}
	
	else if (mFishType == 6)
	{
		//炸弹鱼死亡的附加爆炸动画
		ValueMap tempData;
		tempData["posX"] = this->getPositionX();
		tempData["posY"] = this->getPositionY();
		LuaCppAdapter::getInstance()->callLuaFunc("FishGMF", "CppToLua", "bombEffect", tempData);
		this->runAction(Sequence::create(DelayTime::create(0.0f), CallFunc::create([=](){ this->setFishDeath(); }), NULL));
		AudioControl::getInstance()->playEffect("sound/bomb_01.mp3");
	}
	else if(mFishType >= 3)
	{
		AudioControl::getInstance()->playEffect("sound/capture_01.mp3");
	}

	//mDeathEffect =4 ;
	switch (mDeathEffect)
	{
	case 1:
		//加速
		this->setAccelerate(0);
		this->runAction(Sequence::create(DelayTime::create(0.7), CallFunc::create([&]()
		{
			removeFromScreen();
		}), nullptr));
		break;

	case 2:
	{
		stopAllActions();
		//爆炸
		setBlastEff(0);

		this->runAction(Sequence::create(ScaleTo::create(0.1, 1.5), RotateBy::create(2.0, 360 * 4), CallFunc::create([&]()
		{
			setScale(1);
			setRotation(0);
			removeFromScreen();
		}), nullptr));

	}
		break;
	case 3:
	{	
		stopAllActions();
		//爆炸
		setBlastEff(0);
		//爆金币
		setBlastCoinEff(0.2);

		this->runAction(Sequence::create(ScaleTo::create(0.1, 1.5), RotateBy::create(2.0, 360 * 4), CallFunc::create([=]()
		{
			setScale(1);
			setRotation(0);
			this->setVisible(false);
		}), DelayTime::create(0.1f), CallFunc::create([=](){removeFromScreen();}), nullptr));
		break;
	}
	case 4:
	{
		stopAllActions();
		setShakeBgEff(0);

		float delay = 1.2f;
		//爆炸
		setBlastEff(0);

		//爆金币
		setBlastCoinEff(0.2);
		setBlastCoinEff(0);

		//爆光圈
		setBlastCircleEff(0.2);

		this->runAction(Sequence::create(ScaleTo::create(0.1, 1.5), RotateBy::create(delay-0.1, 360 * 3), CallFunc::create([&]()
		{
			setScale(1);
			setRotation(0);
			this->setVisible(false);
		}), DelayTime::create(0.1f), CallFunc::create([=](){removeFromScreen(); }), nullptr));
		break;
	}
	case 5:
	{
		stopAllActions();
		setShakeBgEff(0);

		float delay = 1.2f;
		//爆炸
		setBlastEff(0);

		//爆金币
		setBlastCoinEff(0.2);
		setBlastCoinEff(0);

		//爆光圈
		setBlastCircleEff(0.2);

		this->setAccelerate(0);
		this->runAction(Sequence::create(DelayTime::create(0.7), CallFunc::create([&]()
		{
			removeFromScreen();
		}), nullptr));
		break;

	}
	case 6:
	{
		stopAllActions();
		setShakeBgEff(0);

		float delay = 1.2f;
		setBlastEff(0);

		//爆金币
		setBlastCoinEff(0.2);
		setBlastCoinEff(0);

		//爆光圈
		setBlastCircleEff(0.2);

		this->removeFromScreen();
		break;
	}
	case 7:
		stopAllActions();
		runAction(Sequence::create(ScaleTo::create(0.1, 1.5), RotateBy::create(2.0, 360 * 4), CallFunc::create([&]()
		{
			setScale(1);
			setRotation(0);
			removeFromScreen();
		}), nullptr));
		break;
	default:
		break;
	}
}

bool Fish::isSpecialFish()
{
	if (mFishType == 7 || mFishType == 6 || mFishType == 8)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void Fish::setAccelerate(float delay)
{
	for (int i = 0; i < mSpriteVec.size(); i++)
	{
		auto ForverAnimate = mSpriteVec[i]->getActionByTag(STANDING_ACTION_TAG);
		if (ForverAnimate != nullptr)
		{
			mSpriteVec[i]->runAction(Speed::create(static_cast<ActionInterval *>(ForverAnimate), 2));
			mSpriteVec[i]->runAction(Speed::create(static_cast<ActionInterval *>(ForverAnimate), 2));
		}
	}
}

void Fish::setBlastEff(float delay)
{
	//爆炸
	CoinManager::getInstance()->setBlastEff(delay, this->getParent(), this->getPosition());
}

void Fish::setBlastCoinEff(float delay)
{
	//爆金币
	CoinManager::getInstance()->setBlastCoinEff(delay, this->getParent(), this->getPosition());
}

void Fish::setBlastCircleEff(float delay)
{
	//爆光圈
	CoinManager::getInstance()->setBlastCircleEff(delay, this->getParent(), this->getPosition());
}

void Fish::setShakeBgEff(float delay)
{
	CoinManager::getInstance()->setShakeBgEff(delay);
}



