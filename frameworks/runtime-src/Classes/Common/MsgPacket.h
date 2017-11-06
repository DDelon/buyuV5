
/**
	@file MsgPacket.h
	@date 2009年3月24日9:26:35
	@brief 网络数据大包定义
	@by 朱心永恒
	@Email wangbing323@163.com
	@QQ 32996758

*/

#pragma once

	/**
	* @brief 分包追加状态 
	*/
	enum packetMsgAddState
	{
		PMAS_FAILED	= -1,	//!<失败,不是一个合法分包
		PMAS_OK		= 0,	//!<成功，包添加成功，继续等待其他分包
		PMAS_FINISHED = 1,	//!<完成，包接收完毕，可以处理
	};

	/**
	* @brief 数据大包.
	* 当发送的数据比较大，超过8000BYTES时使用 
	*/
	class DL_NOEXPORT  CMsgPacket:public CSerialStream
	{
		NO_COPY_CLASS(CMsgPacket);
	public:
		/**
		* @brief 构造对象 
		* @param [in] nSize 预分配缓存大小
		*/
		CMsgPacket(size_t nSize = 0 );
		virtual ~CMsgPacket(void);
	public:
		
		/**
		* @brief  发送前处理
		* @param [in] dwFlag 发送的附带参数，为@ref MSG_HEADER_FLAG 的一个或者多个值的组合
		* @return 成功返回非0
		* @note 对消息包进行发送前必须调用该函数，调用后不能再修改或者追加数据
		* @see CBaseClient::SendData(IN CMsgPacket& ,IN BOOL)
		*/
		bool PreSendData(DWORD dwFlag = MSG_HEADER_FLAG_COMPRESS);

		/**
		* @brief 接收完成,预处理
		* @param [out] pFlag 发送的附带参数,为@ref MSG_HEADER_FLAG 的一个或者多个值的组合
		* @return 成功返回非0
		* @note 消息包接收完毕后处理前必须调用该函数
		*/
		bool RecvFinished( DWORD* pFlag = NULL);

		/**
		* @brief 重新调整包缓存大小
		* @param [in] dwSize 新的缓冲大小 
		* @return 成功返回非0
		*/
		bool ResetSize(DWORD dwSize);

		/**
		* @brief  获得实际数据大小
		* RecvFinished/PreSendData之后才有效
		* @return 返回包实际大小
		*/
		DWORD GetRealSize(void)const
		{
			return m_dwRealSize;
		}

		/**
		* @brief 获得包的唯一标识ID 
		*/
		const GUID& GetGUID(void)const
		{
			return m_guidPacket;
		}

		/**
		* @brief 获得包的消息头 
		*/
		LPMSG_HEADER GetMessageHeader( void )
		{
			return reinterpret_cast<LPMSG_HEADER>(GetStream());
		}

		/**
		* @brief 往当前包里插入一个分包
		* @param [in] pHeader 分包指针
		* @return 返回添加结果
		*/
		packetMsgAddState AddPacketMsg(LPMSG_HEADER pHeader);

		/**
		* @brief  计算存活时间
		* @param [in] dwElapse 上次到这次调用之间的时间差,单位：毫秒
		* @return 返回0则过期,否则有效
		*/
		DWORD TotalLeaveTime(DWORD dwElapse)
		{
			return m_dwLeaveTime<=dwElapse?0:m_dwLeaveTime-=dwElapse;
		}

#ifdef _DEBUG
		bool	m_bPreProcessed;	//!<是否预处理过
#endif
	private:
		char* AllocBuff(size_t nSize);
		void FreeBuff(void);
	protected:
		GUID	m_guidPacket;		//!<包的唯一ID
		DWORD	m_dwLeaveTime;		//!<存留时间，超过时间将销毁
		DWORD	m_dwRealSize;		//!<原始大小
		DWORD	m_dwAllocSize;		//!<分配大小
		DWORD	m_dwPacketCount;	//!<剩余分包数量
		char	m_szBuff[MSG_MAX_SIZE];//!<缓冲大小
	};

