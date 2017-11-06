#ifndef __PLAYER_MANAGER_H__
#define __PLAYER_MANAGER_H__

#include "Player.h"

class PlayerManager :public Node
{
public:
	static PlayerManager*  getInstance();
	bool init();
	void reset();

	//当前所在状态
	void setGameState(int gameState){ mGameState = gameState; };
	int getGameState(){ return mGameState; };

	//我的playerId
	void setMyPlayerId(int playerId){ mMyPlayerId = playerId; };
	int getMyPlayerId(){ return mMyPlayerId; };

	//当前房间id
	void setCurGameRoomID(int curGameRoomID);
	int getCurGameRoomID(){ return mCurGameRoomID; };
	//得到当前房间最低炮倍
	int getCurRoomMinRate(){ return mCurRoomMinRate; };

	void addPlayer(Player* player);
	void addPlayer(ValueMap map);
	void addFriendPlayer(ValueMap map);

	bool delPlayer(int playerId);

	void bindUI(ValueMap map, ui::TextBMFont* text, ui::TextBMFont* addText);
	Player* getPlayerById(int playerId);

	//手动设置数据,不刷新界面
	void setPropRealData(int playerId, int propId, int propCount);

	//基本刷新数据   控制“+金币”数字跳跃，更新真实数据，加入飞行数据，延时显示效果
	void refreshPropData(int playerId, int propId, int realCount, int flyingCount,float updateDelayTime = 2.7f);

	//刷新高级道具
	void refreshSeniorPropData(int playerId, int propId, ValueMap data, int userType, float updateDelayTime = 2.7f);

	//刷新普通道具
	void refreshNormalPropData(int playerId, ValueMap data, int userType, float updateDelayTime = 2.7f);

	//删除高级道具
	void clearAllSeniorProp(int playerId, int clearType);

	//金币跳跃增加特效		//加真实数据
	int coinJumpAct(ValueMap data);

	//未确定的数据			//用于+和-缓存未确定数据
	void isSurePropData(ValueMap data);

	//手动接收数据			//用于+和-缓存飞行数据
	void setAddFlyProp(ValueMap data);

	//手动增加数据		//加真实数据
	void addTrueAndFlyProp(ValueMap data);

	//刷新界面
	void refreshUI(PropFlyingRfreshData* data);

	ValueMap getAndSetPlayerData(ValueMap data);
private:
	void refreshUiByEvent(EventCustom* pSender);
	
	std::vector<Player*> mPlayerVec;

	int mMyPlayerId;
	int mCurGameRoomID;
	int mCurRoomMinRate;

	int mGameState; //当前游戏状态   0, 更新界面   1，登录界面   2，大厅    3，游戏内
};

#endif // !__PLAYER_MANAGER_H__
