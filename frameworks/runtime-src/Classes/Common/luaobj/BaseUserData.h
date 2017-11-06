/**
* @file 	BaseUserData.h
* @brief	自定义LUA userdata关联类
* @date		2014年12月26日 11:03	
* @by 		首席村支书
* @Email 	wangbing323@163.com
* @QQ		32996758
*********************************************************************/
#pragma once

#include "LuaObject.h"

///注册LUA类
#define LUACLASS		USERDATA_PROP
///LUA类实现（创建一个同名的全局LUA表)
#define LUACLASSIMPT(TypeName)		USERDATA_PROP_IMP_(TypeName);\
static bool g__register_result_##TypeName = CBaseUserData::RegisterUserDataClass(#TypeName,&TypeName::Register,lua_CFunction(&TypeName::L_NewUserData<TypeName>));

///LUA类实现(不创建LUA表)
#define LUACLASSIMP(TypeName)		USERDATA_PROP_IMP_(TypeName);\
static bool g__register_result_##TypeName = CBaseUserData::RegisterUserDataClass(#TypeName,&TypeName::Register,NULL);

///LUA类函数关联(无返回值)
#define LUAMETHOD		USERDATA_METHOD
///LUA类函数关联(有返回值)
#define LUAMETHODRETURN USERDATA_METHOD_RETURN
///根据C++函数关联LUA类的属性（读写)
#define LUAPROP			USERDATA_GETPUT
///根据C++函数关联LUA类的属性（读)
#define LUAGET			USERDATA_GET
///根据C++函数关联LUA类的属性（写)
#define LUAPUT			USERDATA_PUT
///关联LUA类的属性实现（读写)
#define LUAPROPIMP		USERDATA_GETPUTFUN
///关联LUA类的属性实现（读)
#define LUAGETIMP		USERDATA_GETFUN
///关联LUA类的属性实现（写)
#define LUAPUTIMP		USERDATA_PUTFUN
///根据变量关联LUA类的属性（读写，变量必须是左值)
#define LUADATA			USERDATA_DATA
///根据变量关联LUA类的属性（读)
#define LUADATAGET		USERDATA_DATA_GET

///关联LUA类的属性实现（读写)
#define LUADATAIMP		LUAPROPIMP
///属性为LUA中的引用值
#define LUAREFPROP		USERDATA_REF

/**
* @brief 重命名注册到Lua中的方法
* @param o 旧的方法名
* @param n 新的方法名
*/
#define METHODRENAME(o,n)\
	m_mapMethod[#n]=m_mapMethod[#o];\
	m_mapMethod.erase(#o);
/**
* @brief 重命名注册到Lua中的属性
* @param o 旧的属性名
* @param n 新的属性名
* @note 读写属性都将被重命名
*/
#define PROPRENAME(o,n) {\
	std::map<StringA,FNPROPERTY>::iterator it=m_mapGet.find(#o);\
	if( it != m_mapGet.end){\
		FNPROPERTY& fn = it->second;\
		m_mapGet.erase(it);\
		m_mapGet[#n] = fn;}\
	it = m_mapPut.find(#o);\
	if( it !=m_mapPut.end){\
		FNPROPERTY& fn=it->second;\
		m_mapPut.erase(it);\
		m_mapPut[#n] = fn;}

/**
* @brief 注册LUA类，该类将在LUA中可用
* @param __TypeName 类名，该类必须直接或者间接从@ref CBaseUserData 派生
* @param _parent 父类名
*/
#define USERDATA_PROP(__TypeName,_parent) \
	friend void Push(lua_State* L,__TypeName* p)	\
	{\
		if(p)									\
			p->PushSelf(L);						\
		else									\
			lua_pushnil(L);						\
	}											\
	friend void Check(lua_State* L,int nIndex,__TypeName*& va)\
	{\
		CBaseUserData* p;\
		Check(L,nIndex,&p);\
		if(p) va = dynamic_cast<__TypeName*>(p);\
		else va=0;\
	}\
	virtual	 const char* GetClassName()\
	{\
		return #__TypeName;\
	}\
	private:\
	typedef int (__TypeName::*FNPROPERTY) (lua_State* );\
	typedef int (*FNMETHOD) (lua_State* L);\
	typedef void (__TypeName::*L_FPCALLBACKV)();\
	static std::map<StringA,FNPROPERTY> m_mapGet;\
	static std::map<StringA,FNPROPERTY> m_mapPut;\
	static std::map<StringA,CBaseUserData::tagMethodInfo> m_mapMethod;\
	public:\
	static __TypeName* NewObject(lua_State* L)\
	{\
		CBaseUserData::L_NewUserData<__TypeName>(L);\
		return GetThis(L,-1);\
	}\
	static __TypeName* GetThis(lua_State* L,int nIndex)\
	{\
		return dynamic_cast<__TypeName*>(CBaseUserData::GetThis(L,nIndex));\
	}\
	virtual int OnPropertyGet(lua_State* L,const StringA& strKey)\
	{\
		std::map<StringA,FNPROPERTY>::iterator itProp = m_mapGet.find(strKey);\
		if(itProp != m_mapGet.end())\
			return (this->*itProp->second)(L);\
		else \
		{\
			std::map<StringA,tagMethodInfo>::iterator itMethod = m_mapMethod.find(strKey);\
			if(itMethod != m_mapMethod.end())\
			{\
				lua_pushlightuserdata(L,&itMethod->second);\
				lua_pushcclosure(L,itMethod->second.fnMethod,1);\
				return 1;\
			}\
		}\
		return _parent::OnPropertyGet(L,strKey);\
	}\
	virtual int OnPropertyPut(lua_State* L,const StringA& strKey)\
	{\
		std::map<StringA,FNPROPERTY>::iterator it = m_mapPut.find(strKey);\
		if(it != m_mapPut.end())\
			return (this->*it->second)(L);\
		return _parent::OnPropertyPut(L,strKey);\
	}
/**
* @brief 注册LUA类的实现
* @param TypeName 类名，该类必须直接或者间接从@ref CBaseUserData 派生
* @internal
*/
#define USERDATA_PROP_IMP_(TypeName) \
	std::map<StringA,TypeName::FNPROPERTY> TypeName::m_mapGet;\
	std::map<StringA,TypeName::FNPROPERTY> TypeName::m_mapPut;\
	std::map<StringA,CBaseUserData::tagMethodInfo> TypeName::m_mapMethod;
	//typedef void (TypeName::*DEFLUACALLFUN)();


/**
* @brief 生成一个Lua属性函数（读取)
* @param propname lua中属性名
* @param fname 当前类的成员函数
*/
#define USERDATA_GET(propname,fname) int _get##propname(lua_State* L)\
	{\
	Push(L,fname());return 1;\
	}
/**
* @brief 生成一个Lua属性函数（赋值)
* @param propname lua中属性名
* @param fname 当前类的成员函数
* @param type fname的返回值类型
*/
#define USERDATA_PUT(propname,fname,type) int _put##propname(lua_State* L)\
	{\
	type va;		\
	Check(L,3,va);	\
	fname(va);		\
	return 0;		\
	}
/**
* @brief 生成一个Lua属性函数（读取和赋值)
* @param propname lua中属性名
* @param fnGet 当前类的成员函数(读取函数)
* @param fnPut 当前类的成员函数(赋值函数)
* @param type fnPut的返回值类型
*/
#define USERDATA_GETPUT(propname,fnGet,fnPut,type)\
	USERDATA_GET(propname,fnGet);\
	USERDATA_PUT(propname,fnPut,type)

/**
* @brief 生成一个Lua属性函数,直接访问类成员数据（只读）
* @param propname 属性名
* @param data 类成员变量
*/

#define USERDATA_DATA_GET(propname,data) \
	int _get##propname(lua_State* L)\
	{\
		Push(L,data);return 1;\
	}
/**
* @brief 生成一个Lua属性函数,直接访问类成员数据（读写）
* @param propname 属性名
* @param data 类成员变量
*/
#define USERDATA_DATA(propname,data) \
	USERDATA_DATA_GET(propname,data) \
	int _put##propname(lua_State* L)\
	{\
		Check(L,3,data);return 0;\
	}

/**
* @brief 生成一个Lua属性函数,直接访问一个lua_ref保存的变量（读写）
*/
#define USERDATA_REF(propname,ref)\
	int _get##propname(lua_State* L)\
	{\
		if(ref!=LUA_NOREF)\
			lua_getref(L,ref);\
		else\
			lua_pushnil(L);\
		return 1;\
	}\
	int _put##propname(lua_State* L)\
	{\
		if(ref!=LUA_NOREF)		\
			lua_unref(L,ref);	\
		lua_pushvalue(L,3);\
		ref = luaL_ref(L,LUA_REGISTRYINDEX);\
		return 0;\
	}
/**
* @brief 绑定lua属性实现（只读属性）
* @param cls C++类名
* @param propname lua属性名
*/
#define USERDATA_GETFUN(cls,propname)\
	m_mapGet[#propname] = FNPROPERTY(&cls::_get##propname);
/**
* @brief 绑定lua属性实现（只写属性）
* @param cls C++类名
* @param propname lua属性名
*/
#define USERDATA_PUTFUN(cls,propname);\
	m_mapPut[#propname] = FNPROPERTY(&cls::_put##propname);
/**
* @brief 绑定lua属性实现（读写属性）
* @param cls C++类名
* @param propname lua属性名
*/
#define USERDATA_GETPUTFUN(cls,propname)\
	USERDATA_GETFUN(cls,propname);\
	USERDATA_PUTFUN(cls,propname);


#ifdef WIN32
/**
* @brief 绑定一个不带返回值的C++方法到Lua中,lua可以用同名方法访问到C++中这个方法
* @param cls C++类名
* @param methodname C++函数名
* @param ... 参数类型
* @note 不是所有函数都支持
*/
#define USERDATA_METHOD(cls,methodname,...) USERDATA_METHODIMP(cls,methodname,cls,__VA_ARGS__)

/**
* @brief 绑定一个带返回值的C++方法到Lua中,lua可以用同名方法访问到C++中这个方法
* @param cls C++类名
* @param methodname C++函数名
* @param ... 参数类型和返回值类型，最有一个为返回值类型
*/
#define USERDATA_METHOD_RETURN(cls,methodname,...) USERDATA_METHOD_RETURNIMP(cls,methodname,cls,__VA_ARGS__)

///绑定一个不带返回值的C++方法到Lua中(实现)
#define USERDATA_METHODIMP(cls,methodname,...)\
	m_mapMethod[ #methodname ] =CBaseUserData::tagMethodInfo(&CBaseUserData::L_RegisterMethod <__VA_ARGS__ >,L_FPCALLBACK((void (cls::*)())(&cls::methodname)) );
///绑定一个带返回值的C++方法到Lua中(实现)
#define USERDATA_METHOD_RETURNIMP(cls,methodname,...)\
	m_mapMethod[ #methodname ] = CBaseUserData::tagMethodInfo(&CBaseUserData::L_RegisterMethod_Return < __VA_ARGS__ >,L_FPCALLBACK( (void (cls::*)())(&cls::methodname)));

#else

#define Lua_RegisterMethodP0(fn,p0,...)					&fn< p0 > 
#define Lua_RegisterMethodP1(fn,p0,p1,...)				&fn< p0,p1 > 
#define Lua_RegisterMethodP2(fn,p0,p1,p2,...)			&fn< p0,p1,p2 >
#define Lua_RegisterMethodP3(fn,p0,p1,p2,p3,...)		&fn< p0,p1,p2,p3 >
#define Lua_RegisterMethodP4(fn,p0,p1,p2,p3,p4,...)		&fn< p0,p1,p2,p3,p4 >
#define Lua_RegisterMethodP5(fn,p0,p1,p2,p3,p4,p5,...)	&fn< p0,p1,p2,p3,p4,p5>

#define USERDATA_METHOD(...) USERDATA_METHODIMP(__VA_ARGS__,0)
#define USERDATA_METHOD_RETURN(...) USERDATA_METHOD_RETURNIMP(__VA_ARGS__,0)
#define Lua_GetRegisterFun(_0,_1,_2,_3,_4,_5,RegFun,...) RegFun
#define USERDATA_METHODIMP(cls,methodname,...) \
	m_mapMethod[ #methodname ] = CBaseUserData::tagMethodInfo(Lua_GetRegisterFun(__VA_ARGS__,Lua_RegisterMethodP5,Lua_RegisterMethodP4,Lua_RegisterMethodP3,Lua_RegisterMethodP2,Lua_RegisterMethodP1,Lua_RegisterMethodP0,0)(CBaseUserData::L_RegisterMethod,cls,__VA_ARGS__,0) \
	,(L_FPCALLBACK)(void (cls::*)())(&cls::methodname));

#define USERDATA_METHOD_RETURNIMP(cls,methodname,...) \
	m_mapMethod[ #methodname ] = CBaseUserData::tagMethodInfo(Lua_GetRegisterFun(__VA_ARGS__,Lua_RegisterMethodP5,Lua_RegisterMethodP4,Lua_RegisterMethodP3,Lua_RegisterMethodP2,Lua_RegisterMethodP1,Lua_RegisterMethodP0)(CBaseUserData::L_RegisterMethod_Return,cls,__VA_ARGS__,0) \
	,(L_FPCALLBACK)(void (cls::*)())(&cls::methodname));

#endif

/**
* @brief 绑定到LUA中类的基类
* @note 派生类通过 @ref LUACLASS 绑定后，可直接在LUA中访问已经注册的属性和方法，LUA中也可添加一个事件表，来接收派生类的事件通知
*/
class DL_NOEXPORT CBaseUserData//:public CLuaObject
{

public:
	/**
	* @brief 析构函数
	* @note 该类一般情况都是创建在lua_newuserdata返回的内存中，所以这种情况必须在gc时手动调用析构函数
	*/
	virtual ~CBaseUserData();
	/**
	* @brief 构造函数
	* @note 如果在LUA中分配，则目前必须走不带参数的构造函数
	*/
	CBaseUserData();

	///不带参数的和返回值的成员函数变量，这个变量用来做中间转换类型用，以避开编译器警告
	typedef void (CBaseUserData::*L_FPCALLBACK)();

	///LUA属性函数，属性都要间接创建这个函数才能供LUA调用
	typedef int(CBaseUserData::*FNPROPERTY)(lua_State* L);
	///LUA方法函数
	typedef lua_CFunction FNMETHOD;

	///注册函数，每个 CBaseUserData 的派生类，如果需要添加属性或者方法，都应该重写该函数并在函数中注册
	typedef bool (*REGISTERFUN)(lua_State* L);

	///LUA方法结构
	struct tagMethodInfo
	{
		FNMETHOD	 fnMethod;		//!<lua回调函数指针
		L_FPCALLBACK fnCallBack;	//!<C++函数地址
		tagMethodInfo(){}
		/**
		* @brief 参数构造
		* @param [in] m LUA回调函数
		* @param [in] c C++函数
		*/
		tagMethodInfo(FNMETHOD m,L_FPCALLBACK c=0):fnMethod(m),fnCallBack(c){}
	};

	///类注册信息
	struct tagRegisterClassInfo
	{
		lua_CFunction	fnNew;	//!<lua中New函数对应的C++函数地址
		REGISTERFUN		fnRegister;//!<类的Register函数地址
	};
public:
	/**
	* @brief 属性查询函数
	* @param [in] L lua状态机指针
	* @param [in] strKey 属性名
	* @return 返回值代表压入lua状态机中数据个数
	* @note lua在访问属性时将触发该函数
	*/
	virtual int OnPropertyGet(lua_State* L,const StringA& strKey);

	/**
	* @brief 属性赋值函数
	* @param [in] L lua状态机指针
	* @param [in] strKey 属性名
	* @return 返回值代表压入lua状态机中数据个数
	* @note lua在设置属性时将触发该函数
	*/
	virtual int OnPropertyPut(lua_State* L,const StringA& strKey);

	/**
	* @brief 获得回调结构信息
	*/
	static L_FPCALLBACK L_GetRegisterCallBack(lua_State* L)
	{
		tagMethodInfo* pInfo = reinterpret_cast<tagMethodInfo*>(lua_touserdata(L,lua_upvalueindex(1)));
		return pInfo?pInfo->fnCallBack:NULL;
		//return lua_touserdata(L,lua_upvalueindex(1));
	}
	///获得当前类的类名
	virtual	 const char* GetClassName()
	{
		return "CBaseUserData";
	}

	/**
	* @brief 方法实现，原型为 void cls:f()
	* @tparam cls C++类名
	* @param [in] L lua状态机指针
	* @return 压入状态机数据数量
	*/
	template<class cls>
	static int L_RegisterMethod(lua_State* L)
	{
		cls* pObj = dynamic_cast<cls*>(GetThis(L,1));
		if(pObj == NULL)
			luaL_error(L,"can't convert class!!");
		
		typedef void (cls::*FNTEMP)();

		FNTEMP fn = FNTEMP(L_GetRegisterCallBack(L));
		(pObj->*fn)();
		return 0;
	}
	
	/**
	* @brief 方法实现，原型为 TResult cls:f()
	* @tparam cls C++类名
	* @tparam TResult C++函数返回值
	* @param [in] L lua状态机指针
	* @return 压入状态机数据数量
	*/
	template<class cls,class TResult>
	static int L_RegisterMethod_Return(lua_State* L)
	{
		cls* pObj = dynamic_cast<cls*>(GetThis(L,1));
		if(pObj == NULL)
			luaL_error(L,"can't convert class!!");

 		typedef TResult (CBaseUserData::*FNCALL)();
		typedef TResult (cls::*FNTEMP)();
 		FNTEMP fn = (FNTEMP)(FNCALL)(L_GetRegisterCallBack(L));
 
 		Push(L,(pObj->*fn)());
		return 1;
	}
	/**
	* @brief 方法实现，原型为 void cls:f(T1)
	* @tparam cls C++类名
	* @tparam T1 C++函数第一个参数类型
	* @param [in] L lua状态机指针
	* @return 压入状态机数据数量
	*/
	template<class cls,class T1>
	static int L_RegisterMethod(lua_State* L)
	{
		cls* pObj = dynamic_cast<cls*>(GetThis(L,1));
		if(pObj == NULL)
			luaL_error(L,"can't convert class!!");

		typedef void (CBaseUserData::*FNTEMP)(T1);
		typedef void (cls::*FNCALL)(T1);
		FNCALL fn = (FNCALL)(FNTEMP)(L_GetRegisterCallBack(L));
		T1 va;
		Check(L,2,va);
		(pObj->*fn)(va);
		return 0;
	}
	/**
	* @brief 方法实现，原型为 TResult cls:f(T1)
	* @tparam cls C++类名
	* @tparam T1 C++函数第一个参数类型
	* @tparam TResult C++函数返回值
	* @param [in] L lua状态机指针
	* @return 压入状态机数据数量
	*/
	template<class cls,class T1,class TResult>
	static int L_RegisterMethod_Return(lua_State* L)
	{
		cls* pObj = dynamic_cast<cls*>(GetThis(L,1));
		if(pObj == NULL)
			luaL_error(L,"can't convert class!!");

		typedef TResult (CBaseUserData::*FNTMP)(T1);
		typedef TResult (cls::*FNCALL)(T1);

		FNCALL fn = (FNCALL)(FNTMP)(L_GetRegisterCallBack(L));

		T1 va;
		Check(L,2,va);
		Push(L,(pObj->*fn)(va));
		return 1;
	}
	/**
	* @brief 方法实现，原型为 void cls:f(T1,T2)
	* @tparam cls C++类名
	* @tparam T1 C++函数第一个参数类型
	* @tparam T2 C++函数第二个参数类型
	* @param [in] L lua状态机指针
	* @return 压入状态机数据数量
	*/
	template<class cls,class T1,class T2>
	static int L_RegisterMethod(lua_State* L)
	{
		cls* pObj = dynamic_cast<cls*>(GetThis(L,1));
		if(pObj == NULL)
			luaL_error(L,"can't convert class!!");

		typedef void (CBaseUserData::*FNTMP)(T1,T2);
		typedef void (cls::*FNCALL)(T1,T2);
		FNCALL fn = (FNCALL)(FNTMP)(L_GetRegisterCallBack(L));

		T1 va;
		Check(L,2,va);
		T2 va2;
		Check(L,3,va2);

		(pObj->*fn)(va,va2);
		return 0;
	}
	/**
	* @brief 方法实现，原型为 TResult cls:f(T1,T2)
	* @tparam cls C++类名
	* @tparam T1 C++函数第一个参数类型
	* @tparam T2 C++函数第二个参数类型
	* @tparam TResult C++函数返回值
	* @param [in] L lua状态机指针
	* @return 压入状态机数据数量
	*/
	template<class cls,class T1,class T2,class TResult>
	static int L_RegisterMethod_Return(lua_State* L)
	{
		cls* pObj = dynamic_cast<cls*>(GetThis(L,1));
		if(pObj == NULL)
			luaL_error(L,"can't convert class!!");

		typedef TResult (CBaseUserData::*FNTMP)(T1,T2);
		typedef TResult (cls::*FNCALL)(T1,T2);
		FNCALL fn = (FNCALL)(FNTMP)(L_GetRegisterCallBack(L));

		T1 va;
		Check(L,2,va);
		T2 va2;
		Check(L,3,va2);
		Push(L,(pObj->*fn)(va,va2));
		return 1;
	}
	/**
	* @brief 方法实现，原型为 void cls:f(T1,T2,T3)
	* @tparam cls C++类名
	* @tparam T1 C++函数第一个参数类型
	* @tparam T2 C++函数第二个参数类型
	* @tparam T3 C++函数第三个参数类型
	* @param [in] L lua状态机指针
	* @return 压入状态机数据数量
	*/
	template<class cls,class T1,class T2,class T3>
	static int L_RegisterMethod(lua_State* L)
	{
		cls* pObj = dynamic_cast<cls*>(GetThis(L,1));
		if(pObj == NULL)
			luaL_error(L,"can't convert class!!");

		typedef void (CBaseUserData::*FNTMP)(T1,T2,T3);
		typedef void (cls::*FNCALL)(T1,T2,T3);
		FNCALL fn = (FNCALL)(FNTMP)(L_GetRegisterCallBack(L));
		T1 va;
		Check(L,2,va);
		T2 va2;
		Check(L,3,va2);
		T3 va3;
		Check(L,4,va3);
		(pObj->*fn)(va,va2,va3);
		return 0;
	}
	/**
	* @brief 方法实现，原型为 TResult cls:f(T1,T2,T3)
	* @tparam cls C++类名
	* @tparam T1 C++函数第一个参数类型
	* @tparam T2 C++函数第二个参数类型
	* @tparam T3 C++函数第三个参数类型
	* @tparam TResult C++函数返回值
	* @param [in] L lua状态机指针
	* @return 压入状态机数据数量
	*/
	template<class cls,class T1,class T2,class T3,class TResult>
	static int L_RegisterMethod_Return(lua_State* L)
	{
		cls* pObj = dynamic_cast<cls*>(GetThis(L,1));
		if(pObj == NULL)
			luaL_error(L,"can't convert class!!");

		typedef TResult (CBaseUserData::*FNTMP)(T1,T2,T3);
		typedef TResult (cls::*FNCALL)(T1,T2,T3);
		FNCALL fn = (FNCALL)(FNTMP)(L_GetRegisterCallBack(L));
		T1 va;
		Check(L,2,va);
		T2 va2;
		Check(L,3,va2);
		T3 va3;
		Check(L,4,va3);
		Push(L,(pObj->*fn)(va,va2,va3));
		return 1;
	}
	/**
	* @brief 方法实现，原型为 void cls:f(T1,T2,T3,T4)
	* @tparam cls C++类名
	* @tparam T1 C++函数第一个参数类型
	* @tparam T2 C++函数第二个参数类型
	* @tparam T3 C++函数第三个参数类型
	* @tparam T4 C++函数第四个参数类型
	* @param [in] L lua状态机指针
	* @return 压入状态机数据数量
	*/
	template<class cls,class T1,class T2,class T3,class T4>
	static int L_RegisterMethod(lua_State* L)
	{
		cls* pObj = dynamic_cast<cls*>(GetThis(L,1));
		if(pObj == NULL)
			luaL_error(L,"can't convert class!!");

		typedef void (CBaseUserData::*FNTMP)(T1,T2,T3,T4);
		typedef void (cls::*FNCALL)(T1,T2,T3,T4);
		FNCALL fn = (FNCALL)(FNTMP)(L_GetRegisterCallBack(L));

		T1 va;
		Check(L,2,va);
		T2 va2;
		Check(L,3,va2);
		T3 va3;
		Check(L,4,va3);
		T4 va4;
		Check(L,5,va4);
		(pObj->*fn)(va,va2,va3,va4);
		return 0;
	}
	/**
	* @brief 方法实现，原型为 TResult cls:f(T1,T2,T3,T4)
	* @tparam cls C++类名
	* @tparam T1 C++函数第一个参数类型
	* @tparam T2 C++函数第二个参数类型
	* @tparam T3 C++函数第三个参数类型
	* @tparam T4 C++函数第四个参数类型
	* @tparam TResult C++函数返回值
	* @param [in] L lua状态机指针
	* @return 压入状态机数据数量
	*/
	template<class cls,class T1,class T2,class T3,class T4,class TResult>
	static int L_RegisterMethod_Return(lua_State* L)
	{
		cls* pObj = dynamic_cast<cls*>(GetThis(L,1));
		if(pObj == NULL)
			luaL_error(L,"can't convert class!!");

		typedef TResult (CBaseUserData::*FNTMP)(T1,T2,T3,T4);
		typedef TResult (cls::*FNCALL)(T1,T2,T3,T4);
		FNCALL fn = (FNCALL)(FNTMP)(L_GetRegisterCallBack(L));
		T1 va;
		Check(L,2,va);
		T2 va2;
		Check(L,3,va2);
		T3 va3;
		Check(L,4,va3);
		T4 va4;
		Check(L,5,va4);
		Push(L,(pObj->*fn)(va,va2,va3,va4));
		return 1;
	}

protected:
	/**
	* @brief 重载new 函数，只支持从lua_newuserdata分配的内存创建对象
	*/
	static void* operator new(size_t ,void* p)
	{
		return p;
	}
	/**
	* @brief 重载new函数，禁止从系统分配对象
	*/
	inline static void* operator new(size_t)throw()
	{
		return NULL;
	}

	/**
	* @brief 对象创建完毕后，将调用该函数
	* @param [in] pMainState lua状态机指针
	* @note 因为构造函数内无法使用m_pMainState变量，所以提供该虚函数替代构造函数
	*/
	virtual void OnCreateObject(lua_State* pMainState)
	{
		m_pMainState = pMainState;
	}
public:
	///构造一个方法对象
	static inline tagMethodInfo MakeMethod(FNMETHOD m,L_FPCALLBACK c=0)
	{
		return tagMethodInfo(m,c);
	}

	///重写delete ，什么都不做，因为lua_newuserdate出来的对象由lua自己回收
	inline static void operator delete(void* ,void* )
	{

	}
	///重写delete ，什么都不做，因为不允许使用默认的new方法
	inline static void operator delete(void*)
	{

	}

	/**
	* @brief 将LUA中传递的值转换为CBaseUserData*
	* @param [in] L lua状态机指针
	* @param [in] nIndex 值所在LUA状态机中的索引
	* @return 返回CBaseUserData指针,不会返回NULL
	* @note 如果值不是一个有效的CBaseUserData对象，将直接将异常抛给LUA
	*/
	static CBaseUserData* GetThis(lua_State* L,int nIndex)
	{
		return *reinterpret_cast<CBaseUserData**>(luaL_checkudata(L,nIndex,m_strMetatable));
		//return reinterpret_cast<CBaseUserData**>(pData);
	}
	/**
	* @brief 注册方法和属性
	* @param [in] L lua状态机指针
	* @return bool 成功返回true,否则返回false
	* @note 每个派生类都应该实现一个改函数，即使没有属性或方法需要添加
	*/
	static bool Register(lua_State* L);

	/**
	* @brief 获得CBaseUserData对象的元表
	* @return 元表名
	* @note CBaseUserData和其派生类公用一个元表
	*/
	static const char* GetMetaTableName(void)
	{
		return m_strMetatable;
	}

	/**
	* @brief 添加一个注册到LUA中的类
	* @param [in] strClassName 类名
	* @param [in] fnReg 类的Register函数地址
	* @param [in] fnNew lua中New函数对应的C++函数指针
	* @return 注册成功返回true,返回false则该类已经注册
	*/
	static bool RegisterUserDataClass(const StringA& strClassName,REGISTERFUN fnReg,lua_CFunction fnNew)
	{
		tagRegisterClassInfo info = {fnNew,fnReg};
		return GetRegisterMap().insert(std::make_pair(strClassName,info)).second;
	}

	/**
	* @brief 创建一个CBaseUserData派生对象并压入lua中
	* @tparam cls CBaseUserData派生类名
	* @param [in] L lua状态机指针
	* @return 压入状态机中的数据个数
	*/
	template<typename cls>
	static int L_NewUserData(lua_State* L)
	{
		char* pData = reinterpret_cast<char*>(lua_newuserdata(L,sizeof(cls)+8));
		luaL_newmetatable(L,m_strMetatable);
		lua_setmetatable(L,-2);

		lua_pushstring(L,m_pluaObjectEX);
		lua_rawget(L,LUA_REGISTRYINDEX);
		if(!lua_istable(L,-1))
		{
			luaL_error(L,"alloc object failed!,fn=L_NewUserData");
		}

		lua_pushlightuserdata(L,pData);
		lua_pushvalue(L,-3);
		lua_rawset(L,-3);

		lua_pop(L,1);

		cls * pObject = new(pData+8) cls;
		*reinterpret_cast<CBaseUserData**>(pData) = static_cast<CBaseUserData*>(pObject);
		pObject->m_pAllocUserData = pData;
		lua_pushstring(L,m_strMainStateKey);
		lua_rawget(L,LUA_REGISTRYINDEX);
		dynamic_cast<CBaseUserData*>(pObject)->OnCreateObject(lua_tothread(L,-1));
		lua_pop(L,1);
		return 1;
	}

	/**
	* @brief 对象添加一次引用
	* @param [in] L lua 状态机指针
	* @return 返回引用计数
	* @note CBaseBaseData对象在LUA中没任何地方引用，则有可能被gc掉
	* @note 为了避免C++中拿到的是野指针，可以通过调用该函数保存引用避免被gc.
	* @note 在C++中不再需要该对象时，应当调用@ref LuaRelease 释放引用，否则可能内存泄漏
	*/
 	int LuaAddRef(lua_State * L = NULL);
	/**
	* @brief 对象释放一次引用
	* @param [in] L lua 状态机指针
	* @return 返回引用计数
	* @note CBaseBaseData对象在LUA中没任何地方引用，则有可能被gc掉
	* @note 为了避免C++中拿到的是野指针，可以通过调用该函数保存引用避免被gc.
	* @note @ref LuaAddRef 和 该函数调用次数必须相同
	*/ 
 	int LuaRelease(lua_State* L = NULL);

	/**
	* @brief 将自己压入LUA栈中
	* @param [in] L lua状态机指针
	*/
	void PushSelf(lua_State* L);

	/**
	* @brief 获得对象的事件函数
	* @param [in] L lua状态机
	* @param [in] strEvent 事件名
	* @return 成功压入栈则返回true,否则返回false
	*/
	bool GetEvent(lua_State* L,const char* strEvent);

	/**
	* @brief 创建一个事件表
	* @param [in] L lua状态机，为NULL则内部创建一个新状态机操作
	* @param [in] bDropOld 如果已经存在事件表，bDropOld为true将丢弃原来的事件表，为false则什么都不做
	*/
	void CreateEventTable(lua_State* L = NULL,bool bDropOld = false);

	/**
	* @brief 获得主lua状态机指针
	*/
	lua_State* GetScript(void)
	{
		return m_pMainState;
	}

	///获得事件表的引用索引
	int GetEventRef(void)const
	{
		return m_nEventRef;
	}
	///判断是否存在事件表
	bool HasEventTable(void)const
	{
		return m_nEventRef!=LUA_NOREF;
	}

	/**
	* @brief 强制获得表或者userdata的属性
	* @tparam T 属性的类型
	* @param [in] L lua状态机指针
	* @param [in] strKey 属性名
	* @param [in] value 获得的属性值
	* @param [in] nIndex 表或者userdata在状态机中索引
	* @note 如果属性无法获得，则抛出lua错误
	*/
	template<typename T>
	void CheckProp(lua_State* L,const char* strKey,T& value, int nIndex = -1)
	{
		lua_getfield(L,nIndex,strKey);
		Check(L,value);
		lua_pop(L,1);
	}
	/**
	* @brief 设置表或者userdata的属性
	* @tparam T 属性的类型
	* @param [in] L lua状态机指针
	* @param [in] strKey 属性名
	* @param [in] value 属性值
	* @param [in] nIndex 表或者userdata在状态机中索引
	*/
	template<typename T>
	void SetProp(lua_State* L,const char* strKey,T value,int nIndex = -1)
	{
		Push(L,value);
		lua_setfield(L,((nIndex>0)?nIndex:nIndex-1),strKey);
	}

	/**
	* @brief 调用LUA中的事件函数
	* @param [in] L lua状态机指针
	* @param [in] strEventName 事件名
	* @param [in] strFmt 格式化参数/返回值列表
	* @param [in] vl 参数或返回值列表
	* @return 找到事件并调用了事件函数则返回true,否则返回false
	*/
	bool CallEventV(lua_State* L, const char* strEventName, const char* strFmt, VALIST vl);
	/**
	* @brief 调用LUA中的事件函数
	* @param [in] strEventName 事件名
	* @param [in] strFmt 格式化参数/返回值列表
	* @param [in] ... 参数或返回值列表
	* @return 找到事件并调用了事件函数则返回true,否则返回false
	*/
	bool CallEvent(const char* strEventName,const char* strFmt,...)
	{
		va_list vl;
		va_start(vl,strFmt);
		CLuaThread L = GetScript();
		bool bRet = CallEventV(L, strEventName,strFmt,vl);
		va_end(vl);
		return bRet;

	}
	/**
	* @brief 调用LUA中的事件函数
	* @param [in] L lua状态机指针
	* @param [in] strEventName 事件名
	* @param [in] strFmt 格式化参数/返回值列表
	* @param [in] ... 参数或返回值列表
	* @return 找到事件并调用了事件函数则返回true,否则返回false
	*/
	bool CallEvent(lua_State* L,const char* strEventName,const char* strFmt,...)
	{
		va_list vl;
		va_start(vl,strFmt);
		bool bRet = CallEventV(L,strEventName,strFmt,vl);
		va_end(vl);
		return bRet;
	}

	/**
	* @brief 调用对象中的LUA函数，也可以调用注册的C++函数
	* @param [in] strFunName 函数名
	* @param [in] strFmt 格式化参数/返回值列表
	* @param [in] ... 参数或返回值列表
	* @return 找到并调用了函数则返回true,否则返回false
	*/
	bool CallLua(const char* strFunName,const char* strFmt,...)
	{
		CLuaThread L = GetScript();
		PushSelf(L);
		va_list vl;
		va_start(vl,strFmt);
		bool bRet = CLuaObject::CallMethodV(L,-1,strFunName,strFmt,vl);
		va_end(vl);
		lua_pop(L,1);
		return bRet;
	}
	/**
	* @brief 调用对象中的LUA函数，也可以调用注册的C++函数
	* @param [in] L lua状态机指针
	* @param [in] strFunName 函数名
	* @param [in] strFmt 格式化参数/返回值列表
	* @param [in] ... 参数或返回值列表
	* @return 找到并调用了函数则返回true,否则返回false
	*/
	bool CallLua(lua_State* L,const char* strFunName,const char* strFmt,...)
	{
		PushSelf(L);
		va_list vl;
		va_start(vl,strFmt);
		bool bRet = CLuaObject::CallMethodV(L,lua_gettop(L),strFunName,strFmt,vl);
		va_end(vl);
		lua_pop(L,1);
		return bRet;
	}
private:
	///对象被lua gc(元表触发)
	static int L_BaseUserData_GC(lua_State* L)
	{
		GetThis(L,1)->~CBaseUserData();
		return 0;
	}

	///访问对象的成员（元表触发）
	static int L_BaseUserData_index(lua_State* L);

	///设置对象的成员（元表触发）
	static int L_BaseUserData_newindex(lua_State* L);

	///调用对象的成员（元表触发）
	static int L_BaseUserData_Call(lua_State* )
	{
		return 0;
	}

	///LUA中增加对象引用
	static int L_AddRef(lua_State* L)
	{
		Push(L,GetThis(L,1)->LuaAddRef(L));
		return 1;
	}
	///LUA中释放对象引用
	static int L_Release(lua_State* L)
	{
		Push(L,GetThis(L,1)->LuaRelease(L));
		return 1;
	}
protected:
	///注册类映射
	static std::map<StringA,tagRegisterClassInfo>& GetRegisterMap();
	///元表名
	static const char* m_strMetatable;
	///主LUA状态机指针
	static const char* m_strMainStateKey;
	
	///REG索引字符串
	static const char* m_pluaObjectEX;
	///lua_newuserdata 分配的原始地址
	void*			m_pAllocUserData;
	lua_State*		m_pMainState;	//!<LUA主线程
private:
	
 	int				m_nLuaRef;
 	int				m_nLuaRefCount;
	int				m_nEventRef;	//事件引用
	int				m_nUserIndexTable;
};
