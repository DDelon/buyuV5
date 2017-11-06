/**
* @file 	BaseConnectManager.h
* @brief	网络连接的管理器
* @date		2014年3月18日 13:19	
* @by 		首席村支书
* @Email 	wangbing323@163.com
* @QQ		32996758
*********************************************************************/
#pragma once

///网络连接的管理器
class DL_NOEXPORT CBaseConnectManager
	:public CBaseUserData
	,public IMessageProcessor
{
	NO_COPY_CLASS(CBaseConnectManager);

public:
	///LUA类绑定
	LUACLASS(CBaseConnectManager,CBaseUserData);

	///消息处理函数
	typedef void (CBaseConnectManager::*FNMSGPROCESSOR)(LPMSG_HEADER);

	///lUA注册函数
	static bool Register(lua_State* L);

private:
	/**
	* @brief 发送数据 
	*/
	static int L_SendData(lua_State* L);
	//virtual void OnCreateObject(lua_State* pMainState);
public:
	///连接管理器构造
	CBaseConnectManager(void);
	///连接管理器析构
	virtual ~CBaseConnectManager(void);

	/**
	* @brief 管理器初始化
	* @return 成功返回true,否则返回false
	* @note 该函数会触发LUA中Initialize事件,lua事件原型
	*/
	virtual bool Initialize(void);

	/**
	* @brief 管理器关闭
	* @note 该函数会触发LUA中ShutDown事件
	*/
	virtual void Shutdown(void);

	/**
	* @brief 网络消息处理
	* @param [in] lpHeader 消息内容
	* @return 成功处理该消息应当返回true,否则应当返回false
	* @note 该函数会出发LUA中的事件，LUA中有两次处理机会，首先调用下面函数原型
	* @code
		e[msgid]=function(manager,msg)
			--msgid 消息ID
			--manager CBaseConnectManager对象
			--msg CLuaMsgHeader 对象
			--return 返回true将直接返回C++这边OnProcessMessage函数返回
		end
	  @endcode
	* @note 如果对应的事件表中没有找到lpHeader对应的ID的函数或者函数返回false,则继续出发LUA中的e.OnProcessMessage
	* @note 如果e.OnProcessMessage仍然返回false,则查找注册的C++消息处理函数
	*/
	virtual bool OnProcessMessage(msg_Header* lpHeader );

	/**
	* @sa IMessageProcessor::OnConnect
	*/
	virtual void OnConnect(bool bConnect );

	/**
	* @brief 与服务器连接断开
	* @param [in] nErrorCode 错误码
	*/
	virtual void OnSocketClose( int nErrorCode );

	/**
	* @brief 发送数据
	* @param [in,out] lpHeader 消息包
	* @param [in] dwFlag 发送标记，参考@ref MSG_HEADER_FLAG
	*/
	virtual void SendData(LPMSG_HEADER lpHeader,DWORD dwFlag = MSG_HEADER_FLAG_NONE);

	///删除自己
	virtual void DeleteSelf(lua_State * L = NULL);
public:
	/**
	* @brief 注册一个消息处理函数
	* @param [in] wMsgID 消息ID
	* @param [in] fn 消息处理函数
	*/
	void RegisterMsgProcess(WORD wMsgID,FNMSGPROCESSOR fn)
	{
		m_pfnMsgProcess[wMsgID] = fn;
	}

	///获得指定ID的消息处理函数
	FNMSGPROCESSOR GetRegisterMsgProcess(WORD wMsgID)
	{
		return m_pfnMsgProcess[wMsgID];
	}
	///获得最后一个数据大包
	CMsgPacket* GetLastMsgPacket(void)
	{
		return m_pLastMsgPacket;
	}
private:	
	static int L_Initialize(lua_State* L)
	{
		CBaseConnectManager* pThis = GetThis(L,1);
		Push(L,pThis->Initialize());
		return 1;
	}

	static int L_ShutDown(lua_State* L)
	{
		CBaseConnectManager* pThis = GetThis(L,1);
		pThis->Shutdown();
		return 0;
	}
protected:
	/**
	* @brief 添加一个包消息
	* @param [in] lpHeader 消息包
	* @return 如果一个包接收完整了，则返回包指针，否则返回NULL
	*/
	CMsgPacket* AddPacketMsg(LPMSG_HEADER lpHeader);
protected:
	std::map<GUID,CMsgPacket*>			m_mapPacket;		///<数据大包映射
	FNMSGPROCESSOR						m_pfnMsgProcess[0x10000];///<消息处理函数
	CMsgPacket*							m_pLastMsgPacket;	///<最后处理的数据大包
};

