/**
* @file 	httpClientSocket.h
* @brief	文件下载套接字（仅支持http协议）
* @date		2014年1月17日 9:08	
* @by 		首席村支书
* @Email 	wangbing323@163.com
* @QQ		32996758
*********************************************************************/
#pragma once
#include "curl/curl.h"
/**
*	@brief 下载类
*	该类支持的lua事件表原型
	@code
	local e={};
	--服务器关闭连接;nErrorCode 错误码
	function e.OnHttpClose(http,nErrorCode){}
	--下载完成
	function e.OnHttpComplete(http){}
	--下载进度 filesize 文件总大小(字节);downloadsize 已下载大小(字节);speed 下载速度(字节/秒)
	function e.OnHttpDataArrival(http,filesize,downloadsize,speed){}
	--下载出错 strError 错误原因
	function e.OnHttpError(http,strError){}
	--获得文件大小 size 文件总大小(字节)
	function e.OnHttpFileSize(http,size){}
*	@endcode
*/

class DL_NOEXPORT CHttpClient 
	:public CBaseUserData
	//,public CClientSocket
{
public:
	///绑定LUA类
	LUACLASS(CHttpClient,CBaseUserData);

	CHttpClient();
	virtual ~CHttpClient(void);
	
	///注册LUA属性和方法
	static bool Register(lua_State* L);

	/**
	* @brief 开始下载
	* @param [in] strUrl 下载地址
	* @param [in] strSaveFileName 保存的文件路径，如果为""，则保存到内存中
	* @param [in] strHeaders 附带请求头，如cookies
	* @param [in] bPostMethed 是否POST方式
	* @param [in] pPostData POST提交的额外数据
	* @param [in] nPostDataSize POST提交的数据长度，单位：字节
	* @return 有错误返回false
	*/
	bool StartDownload( const StringA& strUrl,
		const StringA& strSaveFileName = "" ,
		const StringA& strHeaders = "",
		bool bPostMethed = false, 
		const char* pPostData = NULL,
		unsigned nPostDataSize = 0 );

	/**
	* @brief 开始下载
	* @param [in] strUrl 上传地址
	* @param [in] strFilename 上传的文件路径
	* @param [in] strFieldName 上传的段名，相当于html中元素的name属性的值
	* @param [in] strHeaders 附带请求头，如cookies
	* @return 有错误返回false
	*/
	bool StartUpload(const char* strUrl, const char* strFilename,const char* strFieldName, const char* strHeaders);
	/**
	* @brief 返回当前正在请求的URL地址
	* @note 不一定是 @ref StartDownload 函数传递的Url
	*/
	const StringA& GetUrl(void)const
	{
		return m_strUrl;
	}

	///获得下载的文件大小
	size_t GetFileSize(void)
	{
		return m_nFileSize;
	}

	///获得完成大小
	size_t GetCompleteSize(void)
	{
		return m_nDownloadSize;
	}

	///获得下载速度
	double GetDownloadSpeed(void)
	{
		return m_dblDownloadSpeed;
	}

	///取消下载
	void CancelDownload(void);

	///获得保存的文件路径
	const StringA& GetFileName(void)const
	{
		return m_strSaveFileName;
	}

	/**
	* @brief 获得下载后的内存数据，
	* @note 仅当@ref StartDownload 的strSaveFileName文件名为空时才有可能有数据
	*/
	const std::vector<char>& GetData(void)const
	{
		return m_vRecvData;
	}
	/**
	* @brief 获得下载后的内存数据，
	* @note 仅当@ref StartDownload 的strSaveFileName文件名为空时才有可能有数据
	*/
	std::vector<char>& GetData(void)
	{
		return m_vRecvData;
	}
	/**
	* @brief 获得服务器应答头映射
	*/
	std::map<StringA,StringA>& GetResponeHeaderMap(void)
	{
		return m_mapResponeHeader;
	}
	/**
	* @brief 获得服务器应答头
	* @param [in] strKey 应答头的key
	* @return 如果存在指定的key，则返回头内容，否则返回NULL
	*/
	const char* GetResponeHeader(const char* strKey)const
	{
		std::map<StringA,StringA>::const_iterator it = m_mapResponeHeader.find(strKey);
		return it == m_mapResponeHeader.end()?NULL:it->second.c_str();
	}

	///获得服务器应答码
	DWORD GetResponeCode(void)const
	{
		return m_dwStatusCode;
	}
	///获得CURL句柄
	CURL* GetCURLHandle()
	{
		return m_pCURL;
	}
	///请求完毕
	void OnFinished(int nError);
protected:
	///连接服务器结果,bConnected非0为成功，
	virtual void OnEventConnect(bool bConnected);

	///有数据需要接收
	virtual void OnEventReciveData();

	///连接被关闭,nErrorCode为错误码.
	virtual void OnEventClose(int nErrorCode);

	///计时器函数，http请求不需要心跳
	void OnTimer(){}

	virtual void OnCreateObject(lua_State* pMainState)
	{
		CBaseUserData::OnCreateObject(pMainState);
		CreateEventTable(pMainState);
	}
private:
	void SendRequest(void);//发送请求

	void OnRequestFinished(void);

	void DownloadError(const char* lpError);

	bool SaveRealData(char* pData,DWORD dwLen);

	bool ParseHeader(const  StringA& strUtf8 );

	static int L_StartDownload(lua_State* L);

	static int L_GetHeaders(lua_State* L);

	static int L_GetData(lua_State* L);

	static int L_GetMsgData(lua_State* L);

	static int L_GetRawData(lua_State* L);
private:
	static size_t WriteHttpData(void* pBuff,size_t nSize,size_t nCount,void* pData); 
	static size_t WriteHeaderData(void* pBuff,size_t nSize,size_t nCount,void* pData);

	static size_t ProcessHttp(void* pData,double dblTotalToDownload,double dblnowDownloaded,double,double);
private:
	LUAGET(url,GetUrl);
	LUAGET(speed,GetDownloadSpeed);
	LUAGET(filename,GetFileName);
	LUAGET(code,GetResponeCode);
	LUAGET(size,GetFileSize);
	LUAGET(cursize,GetCompleteSize);
	LUADATAGET(dowloading,m_bDownloading);

	double		m_dblDownloadSpeed;	//下载速度
	size_t		m_nFileSize;		//文件总大小
	size_t		m_nDownloadSize;	//文件下载大小
		
	FILE*		m_hFileHandle;			//写入文件句柄
	StringA		m_strUrl;

	StringA		m_strSaveFileName;
	StringA		m_strRequestHeader;
	StringA		m_strResponeHeader;		//应答头内容
	//String		m_strStatusMsg;			//应答状态

	std::map<StringA,StringA>	m_mapResponeHeader;//应答内容

	std::vector<char>	m_vPostData;	//需要POST的数据
	std::vector<char>	m_vRecvData;	//接收的数据

	
	DWORD		m_dwTimeLastTotal;		//最后计时时间（计算速度)
	DWORD		m_dwTotalSize;			//累计下载字节数（计算速度）
	DWORD		m_dwStatusCode;			//应答码
	bool		m_bPostMethed;			//是否POST
	bool		m_bGetHeader;		//是否已经获得文件头信息
	bool		m_bDownloading;		//是否正在下载
	CURL*		m_pCURL;
	curl_slist*	m_pHeaderList;
};
