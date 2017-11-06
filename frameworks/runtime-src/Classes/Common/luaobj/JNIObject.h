/**
* @file 	JNIObject.h
* @brief	JNI环境对象
* @date		2015年1月14日 9:41	
* @by 		首席村支书
* @Email 	wangbing323@163.com
* @QQ		32996758
*********************************************************************/
#pragma once

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include <jni.h>

/**
* @brief JNI环境
* @note 提供对JNIEnv的封装
*/
class DL_NOEXPORT CEnv
{
public:
	CEnv();

	~CEnv();

	///重载运算符转换为JNIEnv*
	JNIEnv* operator ->()
	{
		return m_pEnv;
	}

	///设置JAVA虚拟机指针
	static void SetJavaVM(JavaVM* pVM)
	{
		m_pJavaVM = pVM;
	}

	///获得JAVA虚拟机指针
	static JavaVM* GetJavaVM(void)
	{
		return m_pJavaVM;
	}

	///获得java类对象
	jclass GetClass(const char* strClassName);
private:
	JNIEnv* m_pEnv;
	bool    m_bAttached;
	static JavaVM*	m_pJavaVM;
};
#endif