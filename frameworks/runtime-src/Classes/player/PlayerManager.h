#ifndef __PLAYER_MANAGER_H__
#define __PLAYER_MANAGER_H__

#include "Player.h"

class PlayerManager :public Node
{
public:
	static PlayerManager*  getInstance();
	bool init();
	void reset();

	//��ǰ����״̬
	void setGameState(int gameState){ mGameState = gameState; };
	int getGameState(){ return mGameState; };

	//�ҵ�playerId
	void setMyPlayerId(int playerId){ mMyPlayerId = playerId; };
	int getMyPlayerId(){ return mMyPlayerId; };

	//��ǰ����id
	void setCurGameRoomID(int curGameRoomID);
	int getCurGameRoomID(){ return mCurGameRoomID; };
	//�õ���ǰ��������ڱ�
	int getCurRoomMinRate(){ return mCurRoomMinRate; };

	void addPlayer(Player* player);
	void addPlayer(ValueMap map);
	void addFriendPlayer(ValueMap map);

	bool delPlayer(int playerId);

	void bindUI(ValueMap map, ui::TextBMFont* text, ui::TextBMFont* addText);
	Player* getPlayerById(int playerId);

	//�ֶ���������,��ˢ�½���
	void setPropRealData(int playerId, int propId, int propCount);

	//����ˢ������   ���ơ�+��ҡ�������Ծ��������ʵ���ݣ�����������ݣ���ʱ��ʾЧ��
	void refreshPropData(int playerId, int propId, int realCount, int flyingCount,float updateDelayTime = 2.7f);

	//ˢ�¸߼�����
	void refreshSeniorPropData(int playerId, int propId, ValueMap data, int userType, float updateDelayTime = 2.7f);

	//ˢ����ͨ����
	void refreshNormalPropData(int playerId, ValueMap data, int userType, float updateDelayTime = 2.7f);

	//ɾ���߼�����
	void clearAllSeniorProp(int playerId, int clearType);

	//�����Ծ������Ч		//����ʵ����
	int coinJumpAct(ValueMap data);

	//δȷ��������			//����+��-����δȷ������
	void isSurePropData(ValueMap data);

	//�ֶ���������			//����+��-�����������
	void setAddFlyProp(ValueMap data);

	//�ֶ���������		//����ʵ����
	void addTrueAndFlyProp(ValueMap data);

	//ˢ�½���
	void refreshUI(PropFlyingRfreshData* data);

	ValueMap getAndSetPlayerData(ValueMap data);
private:
	void refreshUiByEvent(EventCustom* pSender);
	
	std::vector<Player*> mPlayerVec;

	int mMyPlayerId;
	int mCurGameRoomID;
	int mCurRoomMinRate;

	int mGameState; //��ǰ��Ϸ״̬   0, ���½���   1����¼����   2������    3����Ϸ��
};

#endif // !__PLAYER_MANAGER_H__
