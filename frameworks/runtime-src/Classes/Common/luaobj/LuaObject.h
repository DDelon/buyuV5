/**
* @file 	LuaObject.h
* @brief	C <==> LUA 交互封装
* @date		2015年10月1日 10:48	
* @by 		首席村支书
* @Email 	wangbing323@163.com
* @QQ		32996758
*********************************************************************/
#pragma once

class CBaseUserData;

#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
///IOS的VA_LIST实现跟其他平台不同
#define VALIST va_list&
#else
///IOS的VA_LIST实现跟其他平台不同
#define VALIST va_list&
#endif
#define _tostring(id)  #id ///<将一个变量转换成字符串
#define REGLUAGLOBAL(L,v) RawSet((L),_tostring(v),v,LUA_GLOBALSINDEX)	///<定义一个全局变量
#define REGLUAFIELD(L,v) RawSet((L),_tostring(v),v)						///<定义一个局部变量

///lua栈，内部维护一个新的LUA状态机
class DL_NOEXPORT CLuaThread
{
public:
	///通过一个状态机创建一个新的状态机
	CLuaThread(lua_State* pMainThread = NULL);
		///重载->运算符，实现转换为状态机指针
	lua_State* operator ->()
	{
		return m_pNewThread;
	}

	///重载转换运算符，实现转换为状态机指针
	operator lua_State* ()
	{
		return m_pNewThread;
	}

	///从状态机中弹出指定个数的变量
	void Pop(int nCount = 1)
	{
		lua_pop(m_pNewThread,nCount);
	}

	///模板实现压入任意类型变量
	template<typename T>
	void Push(T v)
	{
		Push(m_pNewThread,v);
	}

	///获得全局变量
	void GGet(const char* strName)
	{
		lua_getglobal(m_pNewThread,strName);
	}

	///析构
	~CLuaThread()
	{
		Reset();
	}
	
	
	///将指定位置的变量转换bool类型
	bool toBool(int nIndex)
	{
		return lua_toboolean(m_pNewThread,nIndex)!=0;
	}
	///将指定位置的变量转换字符串类型
	const char* toString(int nIndex)
	{
		return luaL_checkstring(m_pNewThread,nIndex);
	}
	///将指定位置的变量转换Utf16字符串
	StringW toUtf16(int nIndex)
	{
		return CHelper::Utf8ToUtf16(luaL_checkstring(m_pNewThread,nIndex));
	}
	///将指定位置的变量转换int
	int toInteger(int nIndex)
	{
		return luaL_checkint(m_pNewThread,nIndex);
	}
	///将指定位置的变量转换DWORD
	DWORD toDword(int nIndex)
	{
		return static_cast<DWORD>(luaL_checkint(m_pNewThread,nIndex));
	}
	///将指定位置的变量转换float
	float toFloat(int nIndex)
	{
		return static_cast<float>(luaL_checknumber(m_pNewThread,nIndex));
	}
	///将指定位置的变量转换double
	double toDouble(int nIndex)
	{
		return luaL_checknumber(m_pNewThread,nIndex);
	}
	///将指定位置的变量转换long long ,有精度丢失
	long long toLonglong(int nIndex)
	{
		return static_cast<long long>(luaL_checkint(m_pNewThread,nIndex));
	}

	///将指定位置的变量转换WORD
	WORD toWord(int nIndex)
	{
		return static_cast<WORD>(luaL_checkint(m_pNewThread,nIndex));
	}
	///将指定位置的变量转换BYTE
	BYTE toByte(int nIndex)
	{
		return static_cast<BYTE>(luaL_checkint(m_pNewThread,nIndex));
	}

	///将指定位置的变量转换char
	char toChar(int nIndex)
	{
		return static_cast<char>(luaL_checkint(m_pNewThread,nIndex));
	}

	///将指定位置的变量转换UserData
	CBaseUserData* toUserData(int nIndex);

	///将指定位置的变量转换void*
	void* toPoint(int nindex)
	{
		return lua_touserdata(m_pNewThread,nindex);
	}

	///释放状态机
	void Reset()
	{
		if(m_pNewThread)
		{
#ifdef _DEBUG
			int nTop = lua_gettop(m_pNewThread);
			assert(nTop == m_nThreadPos);
#endif
			lua_settop(m_pNewThread, m_nThreadPos);
			//lua_unref(m_pNewThread,m_nThreadRef);
			//lua_gc(m_pNewThread,LUA_GCCOLLECT,0);
		}
	}
private:
	lua_State* m_pNewThread;
	int			m_nThreadPos;
	//int			m_nThreadRef;//线程引用
	//int		   m_nThreadPos;
};

/**
* @brief 执行一个LUA函数
* @param [in] L 状态机指针
* @param [in] nArgs 栈里面参数个数
* @param [in] nReturns 期待的返回值个数
* @return 成功返回true，有错误返回false
*/
DL_NOEXPORT bool Call(lua_State* L,int nArgs = 0,int nReturns = 0);

///压入UTF16字符串到状态机
DL_NOEXPORT inline void Push(lua_State* L, const char16_t* strValue)
{
	lua_pushstring(L, CHelper::Utf16ToUtf8(strValue).c_str());
}
///压入char字符数组（字符串)到状态机
template<size_t s>
DL_NOEXPORT inline void Push(lua_State* L,const char (&szStr)[s])
{
	lua_pushstring(L,szStr);
}
///压入UTF16字符数组(ut16字符串)到状态机
template<size_t s>
DL_NOEXPORT inline void Push(lua_State* L, const char16_t(&szStr)[s])
{
	lua_pushstring(L, CHelper::Utf16ToUtf8(szStr).c_str());
}
///压入utf16字符串到状态机
DL_NOEXPORT inline void Push(lua_State* L,const StringW& strValue)
{
	lua_pushstring(L,CHelper::Utf16ToUtf8(strValue).c_str());
}
///压入字符串到状态机
DL_NOEXPORT inline void Push(lua_State* L,const StringA& strValue)
{
	lua_pushlstring(L,strValue.c_str(),strValue.length());
}
///压入bool值到状态机
DL_NOEXPORT inline void Push(lua_State* L,bool bValue)
{
	lua_pushboolean(L,bValue);
}
///压入DWORD值到状态机
DL_NOEXPORT inline void Push(lua_State* L,DWORD dwValue)
{
	lua_pushnumber(L,dwValue);
}
///压入int值到状态机
DL_NOEXPORT inline void Push(lua_State* L,int nValue)
{
	lua_pushinteger(L,nValue);
}
///压入bool值到状态机
DL_NOEXPORT inline void Push(lua_State* L,float fValue)
{
	lua_pushnumber(L,fValue);
}
///压入double值到状态机
DL_NOEXPORT inline void Push(lua_State* L,double dbValue)
{
	lua_pushnumber(L,dbValue);
}
///压入C函数到状态机
DL_NOEXPORT inline void Push(lua_State* L,lua_CFunction fnValue)
{
	lua_pushcfunction(L,fnValue);
}
///压入long值到状态机
DL_NOEXPORT inline void Push(lua_State* L,long lValue)
{
	lua_pushinteger(L,lValue);
}
///压入字符串到状态机
DL_NOEXPORT inline void Push(lua_State* L,const char* strValue)
{
	lua_pushstring(L,strValue);
}
///压入lightuserdata到状态机
DL_NOEXPORT inline void Push(lua_State* L,void* pData)
{
	lua_pushlightuserdata(L,pData);
}

///压入值到状态机
template<typename tp>
DL_NOEXPORT inline void Push(lua_State* L,const tp tpValue)
{
	lua_pushnumber(L,static_cast<lua_Number>(tpValue));
}
///压入CBaseUserData指针到状态机
template<typename tp>
DL_NOEXPORT inline void Push(lua_State* L,tp*& tpValue)
{
	if(tpValue)
		tpValue->PushSelf(L);
	else
		lua_pushnil(L);
}
///压入longlong 到状态机
DL_NOEXPORT inline void Push(lua_State* L,long long llValue)
{
	lua_pushnumber(L,static_cast<lua_Number>(llValue));
}
///压入系统时间到状态机
DL_NOEXPORT void Push(lua_State* L,const SYSTEMTIME& tm);
///强制从状态机指定位置获取系统时间
DL_NOEXPORT void Check(lua_State* L,int nIndex,SYSTEMTIME& tm);
///强制从状态机指定位置获取utf16字符串
DL_NOEXPORT inline void Check(lua_State* L,int nIndex,StringW& strValue)
{
	const char* lpText = luaL_checkstring(L,nIndex);
	strValue = CHelper::Utf8ToUtf16(lpText);
}
///强制从状态机指定位置获取字符串
DL_NOEXPORT inline void Check(lua_State* L,int nIndex,StringA& strValue)
{
	strValue = luaL_checkstring(L,nIndex);
}
///强制从状态机指定位置获取字符串
DL_NOEXPORT inline void Check(lua_State* L,int nIndex,const char* & ppValue)
{
	ppValue = luaL_checkstring(L,nIndex);
}
///强制从状态机指定位置获取bool值
DL_NOEXPORT inline void Check(lua_State* L,int nIndex,bool& bValue)
{
	bValue = lua_toboolean(L,nIndex)!=0;
}
///强制从状态机指定位置获取int值
DL_NOEXPORT inline void Check(lua_State* L,int nIndex,int & nValue)
{
	nValue = luaL_checkint(L,nIndex);
}
///强制从状态机指定位置获取DWORD值
DL_NOEXPORT inline void Check(lua_State* L,int nIndex,DWORD & uValue)
{
	uValue =static_cast<DWORD>(luaL_checknumber(L,nIndex));
}
///强制从状态机指定位置获取long值
DL_NOEXPORT inline void Check(lua_State* L,int nIndex,long& lValue)
{
	lValue = luaL_checkint(L,nIndex);
}
///强制从状态机指定位置获取long long值
DL_NOEXPORT inline void Check(lua_State* L,int nIndex,long long& llValue)
{
	llValue = static_cast<long long>(luaL_checknumber(L,nIndex));
}
///强制从状态机指定位置获取userdata
DL_NOEXPORT inline void Check(lua_State* L,int nIndex,void*& pData)
{
	if(lua_isuserdata(L,nIndex))
		pData = lua_touserdata(L,nIndex);
	else
		luaL_error(L,"invalid type;need userdata");
}
///强制从状态机指定位置获取CBaseUserData*
DL_NOEXPORT void Check(lua_State* L,int nIndex,CBaseUserData** ppUserData);
///强制从LUA状态机中获取一个全局变量值
template<typename tp>
DL_NOEXPORT void CheckGlobal(lua_State* L,const char* strName,tp& tValue)
{
	lua_getglobal(L,strName);
	Check(L,-1,tValue);
	lua_pop(L,1);
}
///强制从LUA状态机中指定位置获取一个整型值
template<typename tp>
DL_NOEXPORT inline void Check(lua_State* L,int nIndex,tp& tpValue)
{
	tpValue = static_cast<tp>(luaL_checknumber(L,nIndex));
}
/**
* @brief 设置table或者userdata的成员
* @tparam tp1 key的类型
* @tparam tp2 value的类型
* @param [in] L 状态机指针
* @param [in] tpkey 属性名
* @param [in] tpValue 属性值
* @param [in] nIndex table 或者userdata的位置
*/
template<typename tp1,typename tp2>
DL_NOEXPORT inline void RawSet(lua_State* L,const tp1& tpkey,const tp2& tpValue,int nIndex = -3)
{
	Push(L,tpkey);
	Push(L,tpValue);
	lua_rawset(L,nIndex);
}

/**
* @brief LUA对象操作
*/
class DL_NOEXPORT CLuaObject
{
public:
	///LUA方法注册
	static bool Register(lua_State* L);

	///LUA方法反注册
	static void UnRegister(lua_State* L);
	
	///LUA异常处理函数
	static int OnScriptError(lua_State* L);
	///LUA打印函数
	static int L_Print(lua_State* L);
	///LUA异常退出拦截函数
	static void  OnScriptAtPanic(lua_State* L);
	/**
	* @brief 执行一个LUA文件
	* @param [in] L 状态机指针
	* @param [in] strFileName 文件路径
	* @param [in] fmt 参数和返回值格式化字符串
	* @param [in,out] ... 参数和返回值
	* @return 成功执行文件，则返回true，否则返回false
	*/
	static bool DoFile(lua_State* L,const char* strFileName,const char* fmt = NULL,...);

public:
	/**
	* @brief 执行一个LUA全局函数
	* @param [in] fnName LUA中的全局的函数名
	* @param [in] fmt 参数和返回值格式化字符串
	* @param [in,out] ... 参数和返回值
	* @return 成功执行，则返回true
	*/
	static bool CallGlobal(const char* fnName,const char* fmt,...);
	/**
	* @brief 执行一个LUA函数,函数必须位于LUA栈顶
	* @param [in] L lua状态机指针，LUA函数必须位于状态机栈顶
	* @param [in] fmt 参数和返回值格式化字符串
	* @param [in,out] ... 参数和返回值
	* @return 成功执行，则返回true
	*/
	inline static bool CallFun(lua_State* L,const char* fmt,...)
	{
		va_list vl;
		va_start(vl,fmt);
		bool bRet = CallFunV(L,fmt,vl);
		va_end(vl);
		return bRet;
	}
	/**
	* @brief 执行一个LUA函数,函数必须位于LUA栈顶
	* @param [in] L lua状态机指针，LUA函数必须位于状态机栈顶
	* @param [in] fmt 参数和返回值格式化字符串
	* @param [in] vl 参数和返回值列表
	* @return 成功执行，则返回true
	*/
	static bool CallFunV(lua_State* L, const char* fmt, VALIST vl);

	/**
	* @brief 执行一个LUA成员,函数必须位于LUA栈顶
	* @param [in] L lua状态机指针
	* @param [in] nIndex lua对象(table或者userdata)所在栈的位置
	* @param [in] strMethod 方法名
	* @param [in] fmt 参数和返回值格式化字符串
	* @param [in,out] ... 参数和返回值列表
	* @return 成功执行，则返回true
	*/
	static bool CallMethod(lua_State* L,int nIndex,const char* strMethod,const char* fmt,...)
	{
		va_list vl;
		va_start(vl,fmt);
		bool bRet = CallMethodV(L,nIndex,strMethod,fmt,vl);
		va_end(vl);
		return bRet;
	}
	/**
	* @brief 执行一个LUA成员,函数必须位于LUA栈顶
	* @param [in] L lua状态机指针
	* @param [in] nIndex lua对象(table或者userdata)所在栈的位置
	* @param [in] strMethod 方法名
	* @param [in] fmt 参数和返回值格式化字符串
	* @param [in] vl 参数和返回值列表
	* @return 成功执行，则返回true
	*/
	static bool CallMethodV(lua_State* L,int nIndex,const char* strMethod,const char* fmt,VALIST vl)
	{
		lua_getfield(L,nIndex,strMethod);
		if(!lua_isfunction(L,-1))
		{
			lua_pop(L,1);
			return false;
		}

		return CallFunV(L,fmt,vl);
	}
	
	/**
	* @brief 根据格式化字符串格式化参数
	* @param [in] strFmt 格式化字符串
	* @param [out] nRetCount 返回值数量
	* @param [out] nArgCount 参数个数
	* @param [in] L 状态机指针
	* @param [in,out] vl 参数和返回值列表
	* @return 返回偏移过的va_list*
	*/
	static VALIST InitLuaCallParams(const char* &strFmt, int &nRetCount, int &nArgCount, lua_State* L, VALIST vl);
	/**
	* @brief 根据格式化字符串格式化参数
	* @param [out] nRetCount 返回值数量
	* @param [in] strFmt 格式化字符串
	* @param [in] L 状态机指针
	* @param [in,out] vl 参数和返回值列表
	* @return 返回偏移过的va_list*
	*/
	static VALIST CheckLuaCallRet(int nRetCount, const char* strFmt, lua_State* L, VALIST vl);

	/**
	* @brief 解码wld格式的数据
	* @param [in] pData 数据块指针
	* @param [in] nSize 数据块长度
	* @param [out] vData 解码后的数据内容（如果成功)
	* @return 解码成功返回true,否则返回false
	*/
	static bool DecodeLuaData(unsigned char* pData,int nSize,std::vector<unsigned char>& vData);

	/**
	* @brief LUA加载器
	* @note 该加载器必须加载在cocos2d-x自带的加载器之前，否则无效(cocos2d-x自带加载器会拦截所有数据处理)
	*/
	static int L_Loader(lua_State* L);

	/**
	* @brief hook lua的dofile函数
	*
	*/
	static int L_DoFile(lua_State* L);

	/**
	* @brief hook lua的loadfile函数
	*
	*/
	static int L_LoadFile(lua_State* L);

	static lua_State* GetMainState()
	{
		return m_pMainState;
	}
private:
	static lua_State*		m_pMainState;
	static int				m_nStateRef;
};

