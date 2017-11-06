
/**
* @file 	clientsocket.h
* @brief	套接字实现
* @date		2014年3月14日 9:17	
* @by 		首席村支书
* @Email 	wangbing323@163.com
* @QQ		32996758
*********************************************************************/
#ifndef __BaseGameHall__ClientSocket__
#define __BaseGameHall__ClientSocket__

///连接状态
enum emSocketState
{
	SOCKET_STATE_NONE,			//!<没有连接
	SOCKET_STATE_CREATED,		//!<已经创建
	SOCKET_STATE_CONNECTING,	//!<正在连接
	SOCKET_STATE_CONNECTED,		//!<已经连接
	SOCKET_STATE_DISCONNECT,	//!<连接失败
	SOCKET_STATE_CLOSED,		//!<连接已关闭
};


///网络套接字类(客户端)
class DL_NOEXPORT CClientSocket
{
	NO_COPY_CLASS(CClientSocket);
protected:
	///解密结果
	enum
	{
		UNESCAPEMSG_OK,						//!<解压正确
		UNESCAPEMSG_PART,					//!<部分解压
		UNESCAPEMSG_UNTREAD,				//!<退回消息
		UNESCAPEMSG_IGNORE,					//!<忽略消息
		UNESCAPEMSG_ERROR	= static_cast<long>(0x80000001),	//!<	错误
	};

	enum
	{
		MAX_RECIVE_BUFF = MSG_MAX_SIZE,			//!<接收数据缓冲大小
		DEF_CONNECT_TIME_OUT = 15000,			//!<默认超时时间，30000毫秒
		KEEP_ALIVE_TIME_OUT	 = 5,				//!<发送保持心跳包时间间隔
		SOCKET_RECV_TIME_OUT = 8,			//!<套接字连接检测时间,大于该值将判断断线
	};
protected:
	IMessageProcessor*		m_pMessageProcessor;//!<消息处理接口
	emSocketState			m_SocketState;	//!<套接字状态
	DWORD                   m_dwPing;		//!<网络延迟，单位：毫秒
	unsigned int            m_hSocket;		//!<套接字句柄

	char                    m_cBuff[MAX_RECIVE_BUFF*2];//!<缓冲区

	char*                   m_pBuffPrimary;			//!<主缓冲指针
	char*                   m_pBuffBack;			//!<后背缓冲指针
	int                     m_nBuffUsedSize;		//!<缓冲区使用大小
	int						m_nConnectTimeOut;		//!<连接超时时间
	int						m_nConnectTime;			//!<连接时间

	DWORD					m_dwLastRecvData;		//!<最后收到消息时间
	float					m_fTimeForKeepAlive;	//!<保持心跳时间

	bool					m_bSendKeepAlive;		//!<是否发送心跳
	static	std::list<std::pair<CClientSocket*,bool> >	m_listSockets;	//!<套接字链表

	static bool* m_pbCurrentValid;	///<当前更新是否有效标记
	static bool m_bPause;
public:

	/**
	* @brief 构造套接字
	* @param [in] pProcessor 事件处理器回调指针
	*/
	CClientSocket( IMessageProcessor* pProcessor = NULL);
	///析构
	virtual ~CClientSocket();

	/**
	创建套接字对象，使用之前必须先创建
	成功返回true
	*/
	bool CreateSocket(int nAfNet);

	/**
	连接到服务器
	dwServerIP:服务器ip值，主机字节序
	wPort:服务器端口
	返回false连接失败，返回true则等待回调
	*/
	bool ConnectToServer(DWORD dwServerIP,WORD wPort);
	/**
	连接到服务器
	strUrl:服务器的域名或者字符串ip地址
	wPort:服务器端口
	返回false连接失败，返回true则等待回调
	*/
	bool ConnectToUrl(const StringA strUrl, WORD wPort );


	/**
	关闭套接字
	*/
	void CloseSocket(void);

	/**
	* @brief 发送数据
	* @param [in,out] lpHeader 消息包
	* @param [in] dwFlag 发送标记，参考@ref MSG_HEADER_FLAG
	*/
	void SendData(LPMSG_HEADER lpHeader,DWORD dwFlag = MSG_HEADER_FLAG_NONE);

	/**
	* @brief 轮询套接字信息（出发回调）
	* @param [in] dt 上次触发到这次之间的时间间隔,单位：秒
	*/
	static void update(float dt);

	///获得套接字句柄
	unsigned int GetSocket()
	{
		return m_hSocket;
	}
public:
	///获得网络延迟
	DWORD GetPing(void)const
	{
		return m_dwPing;
	}
	///设置事件处理器回调指针
	void SetMessageProcessor(IMessageProcessor* pMessageProcessor)
	{
		m_pMessageProcessor = pMessageProcessor;
	}
	///获得事件处理器回调指针
	IMessageProcessor* GetMessageProcessor(void)const
	{
		return m_pMessageProcessor;
	}
	///设置连接超时时间
	void SetConnectTimeOut(int nTimeOut)
	{
		m_nConnectTimeOut = nTimeOut;
	}
	///获得连接超时时间
	int GetConnectTimeOut(void)const
	{
		return m_nConnectTimeOut;
	}
	///设置是否需要定时更新当前对象
	void SetNeedUpdate(bool bNeed);
protected:

	///分析包（解密、解压）
	virtual void EscapeMessage( LPMSG_HEADER lpHeader);

	///处理包（加密、压缩）
	virtual long UnEscapeMessage( LPMSG_HEADER lpHeader);

protected:
	/**
	* @brief 连接事件
	* @param [in] bOK 是否连接成功
	*/
	virtual void OnEventConnect(bool bOK);
	/**
	* @brief 与服务器连接断开
	* @param [in] nError 断开原因
	*/
	virtual void OnEventClose(int nError);

	/**
	* @brief 有数据需要接收
	*
	*/
	virtual void OnEventReciveData();
	/**
	轮询套接字信息（出发回调）
	*/
	virtual void updateImp( float delta );
	///判断当选循环是否有效
	static bool IsValid(void)
	{
		return m_pbCurrentValid && *m_pbCurrentValid;
	}
public:
	///获得套接字状态
	emSocketState GetSocketState(void)const
	{
		return m_SocketState;
	}
	///设置前后台切换状态
	static void setPauseState(bool bPause)
	{
		m_bPause = bPause;
		if (!bPause)
		{
			std::list<std::pair<CClientSocket*, bool> >::iterator it = m_listSockets.begin();
			for (; it != m_listSockets.end(); ++it)
			{
				if (it->second)
				{
					it->first->m_dwLastRecvData = static_cast<DWORD>(time(NULL));
				}
			}
		} 
	}	
};


#endif /* defined(__BaseGameHall__ClientSocket__) */
