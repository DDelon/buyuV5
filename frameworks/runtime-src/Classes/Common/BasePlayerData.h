/**
* @file 	BasePlayerData.h
* @brief	玩家数据类
* @date		2013年8月9日 12:40	
* @by 		首席村支书
* @Email 	wangbing323@163.com
* @QQ		32996758
*********************************************************************/
#pragma once
///玩家数据类
class DL_NOEXPORT CBasePlayerData
{
	NO_COPY_CLASS(CBasePlayerData);
	friend class CBaseGameClient;
public:
	CBasePlayerData(void);
	virtual ~CBasePlayerData(void);

	///获得用户ID(可能为０)
	DWORD	GetPlayerID(void)const
	{
		return m_dwUserID;
	}

	///获得椅子号
	WORD	GetChairID(void)const
	{
		return m_wChairID;
	}

public:
	///设置玩家指针，这个函数需要在每局游戏开始时自行设置
	virtual void SetPlayer(CBasePlayer* pPlayer);

protected:
	///玩家手里数据
	DWORD			m_dwUserID;

	WORD			m_wChairID;//!<椅子ID
};
