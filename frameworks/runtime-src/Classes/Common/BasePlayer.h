/**
* @file 	BasePlayer.h
* @brief	游戏玩家指针
* @date		2014年3月19日 18:02	
* @by 		首席村支书
* @Email 	wangbing323@163.com
* @QQ		32996758
*********************************************************************/
#pragma once
///游戏玩家指针
class DL_NOEXPORT CBasePlayer
{
public:
	CBasePlayer(void);
	virtual ~CBasePlayer(void);

	/**
	* @brief 得到用户座位号(绝对座位号或者服务器上座位号）
	*/
	WORD GetChair(void)
	{
		return m_UserGameInfo.wChairID;
	}

	/**
	* @brief 获得昵称(utf16编码)
	*/
	const char16_t* GetNickNameUtf16(void)const
	{
		return reinterpret_cast<const char16_t*>(m_UserInfo.szNickName);
	}

	/**
	* @brief 获得昵称（utf8编码) 
	*/
	StringA GetNickName()const;


	/**
	* @brief 获得用户ID 
	*/
	DWORD GetID(void)const
	{
		return m_UserInfo.dwUserID;
	}

	/**
	* @brief 获得用户性别，true:男；false:女 
	*/
	bool GetPlayerSex(void)const
	{
		return m_UserInfo.bSex!=0;
	}
	/**
	* @brief 返回玩家是否是旁观状态
	*/
	bool IsLookOn(void)const
	{
		return m_UserGameInfo.byLookOn!=0;
	}
	/**
	* @brief 设置玩家是否是旁观状态
	*/
	void SetLookOn(bool bLookOn)
	{
		m_UserGameInfo.byLookOn = bLookOn;
	}

	/**
	* @brief 获得用户图像ID，为0xFFFFffff则为自定义头像 
	*/
	DWORD GetAvatarID(void)const
	{
		return m_UserInfo.dwAvatarID;
	}

	/**
	* @brief 获得用户声望值 
	*/
	DWORD GetPrestige(void)const
	{
		return m_UserInfo.dwPrestige;
	}

	/**
	* @brief 获得用户金钱数 
	*/
	LONGLONG GetMoney(void)const
	{
		return m_UserInfo.llUserMoney;
	}

	/**
	* @brief 判断用户是否是VIP 
	*/
	bool IsVIP (void) const
	{
		return m_UserInfo.byUserFrom>=USER_TYPE_NORMAL;
	}

	/**
	* @brief 判断用户是否准备状态 
	*/
	bool IsReady(void)const
	{
		return m_UserGameInfo.byReady!=0;
	}
	
	/**
	* @brief 设置玩家是否准备
	*/
	void SetReady(bool bReady)
	{
		m_UserGameInfo.byReady = bReady;
	}


	/**
	* @brief 获得用户积分值 
	*/
	LONGLONG GetScore(void)const
	{
		return m_UserGameInfo.llScore;
	}

	/**
	* @brief 获得用户总游戏局数 
	*/
	DWORD GetGameCount(void)const
	{
		return m_UserGameInfo.dwWinCount
			+m_UserGameInfo.dwLostCount
			+m_UserGameInfo.dwDrawCount
			+m_UserGameInfo.dwFleeCount;
	}

	/**
	* @brief 获得用户赢局数 
	*/
	DWORD GetWinCount(void)const
	{
		return m_UserGameInfo.dwWinCount;
	}

	/**
	* @brief 获得用户输局数 
	*/
	DWORD GetLostCount(void)const
	{
		return m_UserGameInfo.dwLostCount;
	}

	/**
	* @brief 获得用户平局数 
	*/
	DWORD GetDrawCount(void)const
	{
		return m_UserGameInfo.dwDrawCount;
	}

	/**
	* @brief 获得用户逃跑局数 
	*/
	DWORD GetFleeCount(void)const
	{
		return m_UserGameInfo.dwFleeCount;
	}

	/**
	* @brief 为用户绑定一个自定义数据指针 
	*/
	void SetUserData(void* pUserData)
	{
		m_pUserData = pUserData;
	}

	/**
	* @brief 获得为用户绑定的自定义数据指针 
	*/
	void* GetUserData(void)const
	{
		return m_pUserData;
	}

	/**
	* @brief 设置附带任务数据,该数据在当前游戏内共享
	* @param [in] dwKey 数据索引,目前索引只能是[0,31]之间
	* @param [in] dwValue 为索引指定的值
	* @note 任务数据存储在玩家游戏数据的cbTaskData中，可绕开该函数直接使用cbTaskData操作数据 
	*/
	void SetTaskData(DWORD dwKey,DWORD dwValue)
	{
		assert(dwKey<static_cast<DWORD>(GAME_EXTAND_DATA_LEN/sizeof(DWORD)));
		reinterpret_cast<DWORD*>(m_UserGameInfo.cbTaskData)[dwKey] = dwValue;
	}

	/**
	* @brief 获得附带任务数据,该数据在当前游戏内共享
	* @param [in] dwKey 数据索引,目前索引只能是[0,31]之间
	* @return 返回指定的任务数据值
	* @note 任务数据存储在玩家游戏数据的cbTaskData中，可绕开该函数直接使用cbTaskData操作数据
	*/
	DWORD GetTaskData(DWORD dwKey)const
	{
		assert(dwKey<static_cast<DWORD>(GAME_EXTAND_DATA_LEN/sizeof(DWORD)));
		return reinterpret_cast<const DWORD*>(m_UserGameInfo.cbTaskData)[dwKey];
	}

	/**
	* @brief 设置附带任务数据,该数据在当前游戏内共享
	* @param [in] dwIndex 数据索引,目前索引只能是[0,127]之间
	* @param [in] cbValue 为索引指定的值
	* @note 任务数据存储在玩家游戏数据的cbTaskData中，可绕开该函数直接使用cbTaskData操作数据
	*/
	void SetTaskByte(DWORD dwIndex, BYTE cbValue)
	{
		assert(dwIndex<GAME_EXTAND_DATA_LEN );
		m_UserGameInfo.cbTaskData[dwIndex] = cbValue;
	}

	/**
	* @brief 获得附带任务数据,该数据在当前游戏内共享
	* @param [in] dwIndex 数据索引,目前索引只能是[0,127]之间
	* @return 返回指定的任务数据值
	* @note 任务数据存储在玩家游戏数据的cbTaskData中，可绕开该函数直接使用cbTaskData操作数据
	*/
	DWORD GetTaskByte(DWORD dwIndex)const
	{
		assert(dwIndex<GAME_EXTAND_DATA_LEN );
		return m_UserGameInfo.cbTaskData[dwIndex];
	}

	/**
	* @brief 获得比赛数据附带数据,该数据在当前游戏内共享（每月清理一次)
	* @param [in] dwIndex 数据索引，目前索引只能在[0,15]之间
	* @param [in] dwValue 设置新的数据
	* @note 比赛数据存储在玩家游戏数据的cbMatchData中，cbMatchData后64字节保留，只能访问前64字节
	*/
	void SetMatchData(DWORD dwIndex, DWORD dwValue)
	{
		assert(dwIndex<static_cast<DWORD>(GAME_EXTAND_DATA_LEN / sizeof(DWORD)));
		reinterpret_cast<DWORD*>(m_UserGameInfo.cbMacthData)[dwIndex] = dwValue;
	}

	/**
	* @brief 设置比赛数据附带数据,该数据在当前游戏内共享(每月清理一次)
	* @param [in] dwIndex 数据索引，目前索引只能在[0,31]之间,其中[16,31]为底层使用
	* @return 返回指定的比赛数据
	* @note 比赛数据存储在玩家游戏数据的cbMatchData中，cbMatchData后64字节保留（只读访问)
	*/
	DWORD GetMatchData(DWORD dwIndex)const
	{
		assert((dwIndex)<static_cast<DWORD>(GAME_EXTAND_DATA_LEN / sizeof(DWORD)));
		return reinterpret_cast<const DWORD*>(m_UserGameInfo.cbMacthData)[dwIndex];
	}
	/**
	* @brief 获得比赛数据附带数据,该数据在当前游戏内共享（每月清理一次)
	* @param [in] dwIndex 数据索引，目前索引只能在[0,127]之间
	* @param [in] cbValue 设置新的数据
	* @note 比赛数据存储在玩家游戏数据的cbMatchData中，cbMatchData后64字节保留，只能访问前64字节
	*/
	void SetMatchByte(DWORD dwIndex, BYTE cbValue)
	{
		assert(dwIndex<GAME_EXTAND_DATA_LEN);
		m_UserGameInfo.cbMacthData[dwIndex] = cbValue;
	}

	/**
	* @brief 设置比赛数据附带数据,该数据在当前游戏内共享(每月清理一次)
	* @param [in] dwIndex 数据索引，目前索引只能在[0,127]之间,其中[64,127]为底层使用
	* @return 返回指定的比赛数据
	* @note 比赛数据存储在玩家游戏数据的cbMatchData中，cbMatchData后64字节保留（只读访问)
	*/
	DWORD GetMatchByte(DWORD dwIndex)const
	{
		assert(dwIndex<GAME_EXTAND_DATA_LEN);
		return m_UserGameInfo.cbMacthData[dwIndex];
	}

	///获得用户基本信息
	USERINFO& GetUserInfo(void)
	{
		return m_UserInfo;
	}

	///获得用户游戏信息
	USERGAMEINFO& GetUserGameInfo(void)
	{
		return m_UserGameInfo;
	}
	///获得用户基本信息
	const USERINFO& GetUserInfo(void)const
	{
		return m_UserInfo;
	}
	///获得用户游戏信息
	const USERGAMEINFO& GetUserGameInfo(void)const
	{
		return m_UserGameInfo;
	}
protected:
	USERINFO		m_UserInfo;				//!<玩家基本信息
	USERGAMEINFO	m_UserGameInfo;			//!<玩家游戏信息
	void*			m_pUserData;			//!<自定义数据指针
};

