#ifndef __COINMANAGER_H__
#define __COINMANAGER_H__
#include "cocos2d.h"
#include "ui/CocosGUI.h"
USING_NS_CC;
#define COIN_POOL_SIZE 120
#define COIN_ANIMATION_DELAY 0.2f
#define COIN_LABEL_ANIMATION_DELAY 0.8f

class Coin : public Node
{
public:
	static Coin* Create(bool isSelf = true);
	bool init(bool isSelf);
	bool initAnimation(bool isSelf);
	bool isSelfCoin() { return mIsSelfCoin; }
	bool isUseing(){ return mIsUseing; }


	void run();
	void setIsUsing(bool isUsing) { mIsUseing = isUsing; }

	Sprite* getCoinSpr(){ return mSprite; }

private:
	Animation* mAnimation;
	bool mIsUseing;
	bool mIsSelfCoin;
	Sprite* mSprite;
};


class CoinManager : public Node
{
public:
	static CoinManager *getInstance();
	bool init();
	void initCoinPool();
	void setCoinLayer(Layer* layer) { mCoinLayer = layer; }
	float showGainCoinEffect(int palyerId, int chairId, int moneyCount, int coinNum, Vec2 orginPos, bool isSelf, Vec2 endPos = Vec2(0, 0), bool isShowAddCount = true, float delayTime = COIN_ANIMATION_DELAY + COIN_LABEL_ANIMATION_DELAY);
	void setWinScle(float x, float y) { mWinScaleX = x; mWinScaleY = y; }
	void resetPlayCoinNum() { mPlayCoinNum = 0; }

	//��ȡ�����Ч   ���ڣ��ȼý�
	float showGunUpEffect(int palyerId, int chairId, int moneyCount, int coinNum);
	//ɱ��������
	void killBonusFish(int moneyCount, int fishId, int chairId, int palyerId,float delayTime = 0);

	//���Žӿ�  1.��ը   2.�����   3.����Ȧ   4.������
	bool playCppEffect(int playType, float delayTime, Node* parent, Vec2 pos);
	//��ը
	void setBlastEff(float delay,Node* parent,Vec2 pos);
	//�����
	void setBlastCoinEff(float delay, Node* parent, Vec2 pos);
	//����Ȧ
	void setBlastCircleEff(float delay, Node* parent, Vec2 pos);
	//������
	void setShakeBgEff(float delay);

	void setGameType(int gameType);
	int getGameType(){ return mGameType; };

private:
	Coin* getCoin(bool isSelf);
	Vec2 getRowAndColByCount(int count);
	Vec2 getAimPosByChairId(int chairId);
	ui::TextBMFont* getGoldLabel(bool isSelf);
	Vec2 getAimPosByPlayerId(int playerId);

	Layer* mCoinLayer;
	Array* mSelfCoinPool;
	Array* mOtherCoinPool;
	Array* mSelfGoldLabelPool;
	Array* mOtherGoldLabelPool;
	Action* mLaberAction;

	float mWinScaleX;
	float mWinScaleY;

	bool isPlayCoinSound;
	int mPlayCoinNum;

	int mGameType;
};

#endif