/*!
 * \file HttpManager.h
 * @brief http请求管理器
 * \author wangbing
 * \date 四月 2016
 *
 * @by 王兵
 * @QQ 32996758
 * @Email wangbing323@163.com
 */
#pragma once
#include "curl/curl.h"
class CHttpClient;
///http请求管理类
class CHttpManager:public CClientSocket
{
private:
	CHttpManager();
	~CHttpManager();
public:
	///http请求管理类单例
	static CHttpManager& GetInstance();
	///添加一个HTTP请求对象
	void AddRequest(CURL*,CHttpClient* pClient);
	///移除一个HTTP请求对象
	void RemoveRequest(CHttpClient* pClient);
protected:
	///定时轮询实现
	virtual void updateImp( float delta );
private:
	CURLM*	m_pURLM;
	std::map<CURL*,CHttpClient*> m_mapClient;
};
