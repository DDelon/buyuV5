/**
* @file 	BaseGameClient.h
* @brief	游戏基类
* @date		2014年3月19日 18:00	
* @by 		首席村支书
* @Email 	wangbing323@163.com
* @QQ		32996758
*********************************************************************/
#pragma once

class CBaseRoomManager;
class CBaseGameClient;
class CAudioClient;

///游戏客户端基类
class DL_NOEXPORT CBaseGameClient :
	public CBaseConnectManager
{
	friend class CBaseRoomManager;
	NO_COPY_CLASS(CBaseGameClient);

public:
	/**
	* @brief 构造
	* @param [in] pRoomScene 房间管理器指针
	*/
	CBaseGameClient(CBaseRoomManager* pRoomScene);

	///析构
	virtual ~CBaseGameClient();

public: //虚函数
	/**
	* @brief 游戏初始化 
	*/
	virtual bool Initialize(void);

	/**
	* @brief 游戏反初始化 
	*/
	virtual void Shutdown(void);

	///删除自己
    virtual void DeleteSelf(void)
    {
        LuaRelease();
    }
	/**
	* @brief 玩家进入游戏界面 
	* @param [in] pPlayer 玩家指针
	* @param bSelf 是否是玩家自己
	*/
	virtual void OnPlayerJoin(CBasePlayer* pPlayer,bool bSelf);

	/**
	* @brief 玩家离开游戏
	* @param [in] pPlayer 离开的玩家指针
	*/
	virtual void OnPlayerLeave(CBasePlayer* pPlayer);

	/**
	* @brief 游戏重置（换桌引起） 
	*/
	virtual void OnGameReset(void){}

	/**
	* @brief 系统消息
	* @param [in] msgType 消息类型
	* @param [in] strMsg 消息内容
	*/
	virtual void PrintMessage(printMessageType msgType,const StringA& strMsg);

	/**
	* @brief 使用道具成功应答
	* @param [in] dwFromUser 使用者ID
	* @param [in] dwToUser 目标用户ID
	* @param [in] dwPropID 道具ID
	*/
	virtual void OnPropSuccess(DWORD dwFromUser,DWORD dwToUser,DWORD dwPropID);//
	/**
	* @brief   使用道具失败
	* @param [in] strError 失败原因
	*/
	virtual void OnPropFailure(const StringW& strError);
public: //一般不用重写的虚函数
	/**
	* @brief 创建玩家，重写该函数可创建派生于CBasePlayer的玩家对象 
	*/
	virtual CBasePlayer* CreatePlayer();

	/**
	* @brief 销毁玩家
	*/
	virtual void DestroyPlayer(CBasePlayer* pPlayer);

	/**
	* @brief 通知停止并强行关闭游戏 
	*/
	virtual void ShutdownGame(void);


	void SendData(LPMSG_HEADER lpHeader,DWORD dwFlag  = MSG_HEADER_FLAG_NONE );

	/**
	* @brief 创建游戏场景 
	*/
	virtual Scene* CreateGameScene(void);

	/**
	* @brief 创建玩家数据对象 
	*/
	virtual CBasePlayerData* CreaetePlayerData(void)
	{
		return new CBasePlayerData;
	}

	///设置是否是队伍模式的快速加入方式进入游戏
	void OnHallTeamRoomJoinQuickly( bool bJoinQuickly );


	///队伍模式快速加入？？？？
	virtual void OnTeamRoomQuickJoin(bool bQuickJoin)
	{
		UNREF(bQuickJoin);
	}

public:	//比赛相关

	///比赛报名成功（失败客户端将关闭，不走该函数，启动报名请调用ChangeDesk函数)
	virtual void OnPKJoinReply(){}

	/**
	* @brief 更新比赛房间内玩家数量
	* @param [in] wInRooms 参与该比赛的总玩家数
	* @param [in] wMaxPlayer 当前比赛分组需要多少人才能开始
	* @param [in] wInGroup 当前分组内人数
	*/
	virtual void OnPKUpdatePlayers(WORD wInRooms,WORD wMaxPlayer,WORD wInGroup)
	{
		UNREF(wInRooms);
		UNREF(wMaxPlayer);
		UNREF(wInGroup);
	}

	/**
	* @brief 比赛轮次更新通知
	* @param [in] wPKState 轮次ID
	*/
	virtual void OnPKRoundOver(WORD wPKState)
	{
		UNREF(wPKState);
	}

	/**
	* @brief 比赛结束（获奖或者被淘汰)
	* @param [in] wRankIndex 当前名次（从1开始）
	* @param [in] vAwards 奖励物品，为空则没有任何奖励)
	*/
	virtual void OnPKAward(WORD wRankIndex,const std::vector<std::pair<DWORD,long long> >& vAwards)
	{
		UNREF(wRankIndex);
		UNREF(vAwards);
	}

	/**
	* @brief 等待其他桌结束
	* @param [in] wDeskCount 剩余桌子数
	*/
	virtual void OnPKWaitDeskGameOver( WORD wDeskCount)
	{
		UNREF(wDeskCount);
	}

	/**
	* @brief 比赛信息
	* @param [in] strMsg 比赛消息内容 
	*/
	virtual void OnPKMessage(const StringA& strMsg)
	{
		UNREF(strMsg);
	}

	/**
	* @brief 比赛排名
	* @param [in] dwPlayerID 用户ID
	* @param [in] wRankIndex 用户名次（从1开始)
	*/
	virtual void OnPKRankIndex(DWORD dwPlayerID,WORD wRankIndex)
	{
		UNREF(dwPlayerID);
		UNREF(wRankIndex);
	}
public:	//方法
	/**
	* @brief 根据用户ID查找用户
	* @param dwUserID 用户ID
	* @return 如果该桌存在指定ID的用户则返回用户指针，否则返回NULL
	*/
	CBasePlayer* FindPlayerByID(DWORD dwUserID);

	/**
	* @brief 根据座位号获得玩家指针（绝对座位号，服务器座位号） 
	*/
	CBasePlayer* GetPlayerByChair(WORD wChairID)
	{
		assert(wChairID<m_RoomInfo.wPlayersPerDesk);
		return m_vPlayers[wChairID];
	}
	
	/**
	* @brief 根据座位号获得玩家数据 
	*/
	CBasePlayerData* GetPlayerData(WORD wChairID)
	{
		assert(wChairID<m_RoomInfo.wPlayersPerDesk);
		return m_vPlayerData[wChairID];
	}

	/**
	* @brief 获得玩家自己指针 
	*/
	CBasePlayer* GetPlayerSelf(void)const
	{
		return m_pPlayerSelf;
	}
	/**
	* @brief 玩家起立 
	*/
	void StandUP(void);

	/**
	* @brief 离开房间
	*/
	void ExitRoom(void);
    
	///设置房间管理器指针
	void SetRoomScene(CBaseRoomManager* pRoomScene);
	/**
	* @brief 玩家换桌，仅防作弊房间有效 
	*/

	void ChangeDesk(void);

	/**
	* @brief 获得用户VIP等级
	* @param [in] dwVIPExperience VIP经验值
	* @return 返回VIP等级，如果返回-1，则不是VIP
	*/
	static int GetVIPLevel(DWORD dwVIPExperience);

	///根据声望值获得声望级别
	static int GetPrestigeLevel( DWORD dwPrestige );

	///绝对座位转为UI座位
	WORD ChairToUI(WORD wChair)
	{
		assert(m_pPlayerSelf);
		return static_cast<WORD>((wChair+m_RoomInfo.wPlayersPerDesk-m_pPlayerSelf->GetChair())%m_RoomInfo.wPlayersPerDesk);
	}

	///UI座位转为绝对座位
	WORD UIToChair(WORD wUI)
	{
		assert(m_pPlayerSelf);
		return static_cast<WORD>((wUI+m_pPlayerSelf->GetChair())%m_RoomInfo.wPlayersPerDesk);
	}

	/**
	* @brief 打开商城 
	* @deprecated 游戏中没有商城，该函数已废弃
	*/
	DEPRECATED("this function will  not support in the future .(by wangbing)")
	void ShowStore(void);

	/**
	* @brief 显示游戏关卡	
	* @deprecated 因长龙任务关闭，该函数已废弃
	*/
	DEPRECATED("this function will  not support in the future .(by wangbing)")
	void ShowGameMission(void);

	/**
	* @brief 获得游戏关卡ID
	* @note 如果返回0xFFFFffff，则当前关卡暂时不可用或者没有关卡信息
	* @deprecated 因长龙任务关闭，该函数已废弃
	*/
	DEPRECATED("this function will  not support in the future .(by wangbing)")
	DWORD GetGameMissionID(void)const;

	/**
	* @brief 获得游戏关卡当前数据值	
	* @deprecated 因长龙任务关闭，该函数已废弃
	*/
	DEPRECATED("this function will  not support in the future .(by wangbing)")
	DWORD GetGameMissionValue(void)const;

	/**
	* @brief 获得游戏关卡完成所需的数据值	
	* @note 返回空则没有关卡信息
	* @deprecated 因长龙任务关闭，该函数已废弃
	*/
	DEPRECATED("this function will  not support in the future .(by wangbing)")
	const tagGameMission* GetCurrentGameMissionInfo(void)const;

	/**
	* @brief 领取救济金应答
	* @param [in] llNewMoney 领取后玩家身上的钱
	* @param [in] llGiftValue 领取的钱
	* @param [in] nGiftCount 奖励的次数
	* @param [in] nLeftCount 剩余的奖励次数
	* @warning 单词拼写错误 by wangbing
	*/
	virtual void OnGiftMoneyReplay(LONGLONG llNewMoney, LONGLONG llGiftValue, int nGiftCount, int nLeftCount);

	/**
	* @brief 通知玩家身上的钱反生变化
	* @deprecated 该函数已经不再支持，请重写@ref OnUpdatePlayerInfo 实现
	*/
	DEPRECATED("this function will not support in the future,use OnUpdatePlayerInfo instead of it")
	virtual void OnSynchronousUserGameInfo(USERGAMEINFO& userGameInfo,DWORD dwUserId,long long userMoney);

	/**
	* @brief 通知玩家身上的钱反生变化
	* @param [in] pPlayer 发生变化的玩家指针
	*/
	virtual void OnUpdatePlayerInfo(CBasePlayer* pPlayer)
	{
		UNREF(pPlayer);
	}


	/**
	* @brief 游戏配置
	* @param [in] stream 游戏配置流，仅该函数有效，函数调用完毕该对象将销毁
	*/
	virtual void OnGameConfig(CSerialStream& stream)
	{
		UNREF(stream);
	}
	///把消息压入消息链
	void DelaySendData(MSG_HEADER* pMsg);
	///询问消息链中是否有消息
	bool IsHaveMessage();
	///解压消息链中的一条消息
	bool DispatchDelayMessage();
	///清除消息链
	void CleanMessge();

public://属性

	///获得游戏属性
	const GAMEINFO& GetGameInfo(void)const
	{
		return m_GameInfo;
	}

	///获得房间属性
	const ROOMINFO& GetRoomInfo(void)const
	{
		return m_RoomInfo;
	}

	///判断是否是指定的房间类型
	bool IsRoomType(DWORD dwRoomType)const
	{
		return (m_RoomInfo.dwRoomType & dwRoomType)!=0;
	}

	///判断房间的调度模式
	DWORD GetRoomMode(void)const
	{
		return GET_ROOM_MODE(m_RoomInfo.dwRoomType);
	}
    ///加入游戏的方式
    void SetGameStartType(int type)
    {
        m_iGameStartType = type;
    }
	///获得游戏开始类型???（没看懂 by wangbing)
    int  GetGameStartType()
    {
        return m_iGameStartType;
    }

	///设置当前登陆帐号的session
	void SetGUIID(const std::string& strToken,WORD wHallServerID)
	{
		m_StrGUIID = strToken;
		m_wHallServerID = wHallServerID;
	}

	///获得当前登陆帐号的session
	const std::string& GetGUID()
	{
		return m_StrGUIID;
	}

	///获得当前所属的大厅服务器ID
	WORD GetHallServerID(void)const
	{
		return m_wHallServerID;
	}
	/**
	* @brief 发送使用道具
	* @deprecated 该函数不再支持，请用@ref UseProp 替代
	*/
	DEPRECATED("this function will not support in the future,use UseProp instead of it")
	void Sendmsg_room_use_prop(DWORD dwTarget,DWORD	dwPropID);
	
	/**
	* @brief 发送使用道具
	* @param [in] pPlayerTarget 目标用户指针（对谁使用)
	* @param [in] dwPropID 道具ID
	*/
	void UseProp(CBasePlayer* pPlayerTarget,DWORD dwPropID);

	///获得玩家积分级别信息
	static const StringA& GetLevel( long long _num,int & iLevel);

	/**
	* @brief 领取救济金
	*
	*/
	void GiftMoney(void);



	///使用key获取命令行 int 类型值
	int GetRoomCommand(const char* pKey, int defVal)
	{
		const char * pVal = GetRoomCommand(pKey, static_cast<const char*>(NULL));
		return pVal ? atoi(pVal) : defVal;
	}
	///使用key获取命令行 double 类型值
	double GetRoomCommand(const char* pKey, double defVal)
	{
		const char * pVal = GetRoomCommand(pKey, static_cast<const char*>(NULL));
		return pVal ? atof(pVal) : defVal;
	}
	///使用key获取命令行 string 类型值
	const char* GetRoomCommand(const char* key, const char* strDefault = NULL)
	{
		std::map<StringA, StringA>::iterator it = m_mapCmdParams.find(key);
		return it != m_mapCmdParams.end() ? (it->second.c_str()) : strDefault;
	}
	void SetRoomKey(std::string& roomKey)
	{
		m_strRoomKey = roomKey;
	}

	void SetOwner(DWORD ownerPalyer)
	{
		m_dwOwner = ownerPalyer;
	}
	const std::string& GetRoomKey()
	{
		return m_strRoomKey;
	}
	///是否是房主
	DWORD GetOwner()
	{
		return m_dwOwner;
	}
	/**
	* @brief 操作玩家附属属性
	* @param [in] llValue ,要操作的玩家属性值,正加负减
	* @param [in] cbIndex 数据索引ID,目前只支持[0,5)
	*/
	void OpPlayerProperty(LONGLONG llValue, BYTE cbIndex = 0); 

	/**
	* @brief 操作玩家日常附属属性
	* @param [in] llValue ,要操作的玩家属性值,正加负减
	* @param [in] cbIndex 数据索引ID,目前只支持[0,5)
	*/
	void OpPlayerPropertyDaily(LONGLONG llValue, BYTE cbIndex = 0); 

protected:
	/**
	* @brief 播放语音文件
	* @param [in] strSession 语音文件session
	* @return 
	*/
	bool PlayerUserAudio(const char* strSession);


	/**
	* @brief 启动下载语音文件
	* @param [in] vData 下载保存的数据
	* @param [in] strSession 文件的编号ID
	* @param [in] dwUserID 文件上传者的ID
	*/
	void DownloadAudioFile(std::vector<BYTE>& vData, const char* strSession, DWORD dwUserID);

	/**
	* @brief 删除一个已经上传的文件
	* @param [in] strSession 文件的编号ID
	* @note 只能删除自己上传的文件
	*/
	void DeleteAudioFile(const char* strSession);

	///检测下一个语音文件	
	void CheckNextAudioFile();

	///检测缓存文件过期时间
	void CheckAudioFileCacheTime(void);

public:
	/**
	* @brief 启动上传语音文件
	* @param [in] filename 上传文件的绝对路径
	*/
	void UploadAudioFile(const char* filename);

	/**
	* @brief 复位语音下载上传客户端（重置连接）
	* @warning 该函数不能在语音回调函数里面直接或者间接调用
	*/
	void ResetAudioClient();

	///通知声音文件播放完毕
	void OnPlayUserAudioOver(const char* strSession);

public:
	/**
	* @brief 播放玩家语音回调
	* @param [in] pPlayer 说话玩家指针
	* @param [in] strSession 语音会话ID
	* @param [in] filename 语音文件路径
	* @param [in] dwTimeLen 语音时长,单位:毫秒
	* @return 成功返回true,失败返回false
	* @note 该函数应当在界面初始化完毕后才返回true
	*/
	virtual bool OnPlayerUserAudio(CBasePlayer* pPlayer, const char* strSession, const char* filename, DWORD dwTimeLen)
	{
		UNREF(pPlayer);
		UNREF(strSession);
		UNREF(filename);
		UNREF(dwTimeLen);
		return false;
	}

	/**
	* @brief 上传声音文件成功
	* @param [in] guidSession 服务器给文件分配的编号ID
	* @param [in] dwTimeLen 声音文件时长，单位：毫秒
	* @warning 不应当在该函数里直接或者间接调用@ref ResetAudioClient
	*/
	virtual void OnUploadAudioFile(const GUID& guidSession, DWORD dwTimeLen);

	/**
	* @brief 上传声音文件失败
	* @param [in] strErr 错误原因，仅简单描述错误原因，不应当呈现给用户
	* @warning 不应当在该函数里直接或者间接调用@ref ResetAudioClient
	*/
	virtual void OnUploadAudioFileFailed(const char* strErr);

	/**
	* @brief 广播其他玩家上传声音文件
	* @param [in] pPlayer 上传玩家指针
	* @param [in] guidSession 声音文件ID
	* @param [in] dwTime 声音文件时长，单位：毫秒
	*
	*/
	virtual void OnPlayerUploadAudio(CBasePlayer* pPlayer, const GUID& guidSession, DWORD dwTime);
	/**
	* @brief 下载声音文件成功
	* @param [in] strSession 下载文件的编号ID
	* @warning 不应当在该函数里直接或者间接调用@ref ResetAudioClient
	*/
	virtual void OnDownloadAudioFile(const char* strSession);


	/**
	* @brief 下载文件失败
	* @param [in] strguidSession 下载文件的编号ID
	* @param [in] strErr 错误原因，仅简单描述错误原因，不应当呈现给用户
	* @warning 不应当在该函数里直接或者间接调用@ref ResetAudioClient
	*/
	virtual void OnDownloadAudioFileFailed(const char* strguidSession, const char* strErr);
private:
	void OnMsgCommonUploadAudioNotify(msg_Header* lpHeader);
	void OnMsgCommonGameConfig(msg_Header* lpHeader);
private:
	static std::map<StringA,FNCreateGameClient>& GetGameCreatorMap();
public:
	///方法和属性注册
	static bool Register(lua_State*){return true;}
	/**
	* @brief 添加一个游戏客户端创建者
	* @param [in] fnCreator 创建函数
	* @param [in] strShortName 游戏缩写名
	* @return 成功返回true
	*/
	static bool AddGameClientCreator(FNCreateGameClient fnCreator,const StringA& strShortName);
protected:
	CBaseRoomManager*					m_pRoomScene;		//!<房间场景
	GAMEINFO							m_GameInfo;			//!<游戏信息
	ROOMINFO							m_RoomInfo;			//!<房间信息
	std::vector<CBasePlayer*>			m_vPlayers;			//!<玩家指针
	std::vector<CBasePlayerData*>		m_vPlayerData;		///<玩家数据
	CBasePlayer*						m_pPlayerSelf;		//!<玩家自己指针
	CAudioClient*						m_pAudioClient;		//!<语音客户端对象
	//unsigned int                        m_nAudioSound;		///<语音文件播放索引
	//std::string                         m_strAudioFileName;	///<语音文件名

    int                                 m_iGameStartType;	///<游戏启动类型，是否组队？

	std::string							m_StrGUIID;			//!<当前登陆的回话ID

	std::map<StringA, StringA>			m_mapCmdParams;		///<命令行映射

	DWORD								m_dwOwner;			//!<桌子所有者
	StringA								m_strRoomKey;		//!<桌子密码(房间密码)

	WORD								m_wHallServerID;	//!<大厅服务器ID
	bool								m_bCreateScene;		//!<是否有创建场景

	struct tagUserAudioInfo
	{
		StringA strSession;			//语音ID
		std::vector<BYTE> vData;	//语音数据
		DWORD   dwUserID;			//用户ID
		DWORD   dwTimeLen;			//语音长度
		DWORD	dwTimeLeave;		//存留时间，从下载开始，超过10秒则移除
		bool    bDownloaded;  //是否下载
	};

	//std::vector<tagUserAudioInfo>       m_vUserAudioInfo;   ///<用户语音信息


	//std::vector<tagUserAudioInfo>       m_vUserAudioInfo_Cache;   ///<用户语音缓冲

	std::list<StringA>					m_lstAudioPlay;	///<播放列表
	std::list<StringA>					m_lstAudoDownload;	///<下载列表
	std::map<StringA, tagUserAudioInfo>	m_mapAudioInfo;///<语音列表
private:

	///游戏创建函数 key=游戏缩写名,value=fun
	static std::map<StringA,FNCreateGameClient>	m_mapFnCreateGameClient;
};

/**
* @def REGGAMECLIENTCREATOR
* @brief 游戏创建者注册
* @param cls 游戏类名
* @param shortname 游戏缩写名
*/
#define REGGAMECLIENTCREATOR(cls,shortname) \
class __lua_##cls :public cls\
{\
public:\
	LUACLASS(__lua_##cls,cls);\
	__lua_##cls():cls(NULL){}\
	static CBaseGameClient* __Creator(lua_State* L,CBaseRoomManager* pRoomManager)\
	{\
		CBaseGameClient* pGameClient = __lua_##cls::NewObject(L);\
		if(pGameClient)\
		{\
			pGameClient->SetRoomScene(pRoomManager);\
			pGameClient->LuaAddRef(L);\
			lua_pop(L,1);\
		}\
		return pGameClient;\
	}\
};\
LUACLASSIMP(__lua_##cls);\
extern bool _g_reg_result_lua_##cls =CBaseGameClient::AddGameClientCreator(&__lua_##cls::__Creator,shortname);
