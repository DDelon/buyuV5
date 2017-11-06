/*!
 * \file HttpManager.h
 * @brief http���������
 * \author wangbing
 * \date ���� 2016
 *
 * @by ����
 * @QQ 32996758
 * @Email wangbing323@163.com
 */
#pragma once
#include "curl/curl.h"
class CHttpClient;
///http���������
class CHttpManager:public CClientSocket
{
private:
	CHttpManager();
	~CHttpManager();
public:
	///http��������൥��
	static CHttpManager& GetInstance();
	///���һ��HTTP�������
	void AddRequest(CURL*,CHttpClient* pClient);
	///�Ƴ�һ��HTTP�������
	void RemoveRequest(CHttpClient* pClient);
protected:
	///��ʱ��ѯʵ��
	virtual void updateImp( float delta );
private:
	CURLM*	m_pURLM;
	std::map<CURL*,CHttpClient*> m_mapClient;
};
