/**
* @file 	luaHelper.h
* @brief	LUA扩展类
* @date		2015年1月9日 10:05	
* @by 		首席村支书
* @Email 	wangbing323@163.com
* @QQ		32996758
*********************************************************************/
#pragma once
///LUA工具扩展类
class DL_NOEXPORT CLuaHelper
{
public:

	///注册属性和函数
	static bool Register(lua_State* L);

private:
	/**
	* @brief 32位运算 与 
	* @note 返回LUA p1 & p2;
	*/
	static int L_Bit_And(lua_State* L);

	/**
	* @brief 32位运算 或
	* @note 返回LUA p1 | p2;
	*/
	static int L_Bit_Or(lua_State * L);

	/**
	* @brief 32位运算 异或
	* @note 返回LUA p1 ^ p2;
	*/
	static int L_Bit_Xor(lua_State* L);

	/**
	* @brief 32位运算 反
	* @note 返回LUA !p1;
	*/
	static int L_Bit_Not(lua_State* L);

	/**
	* @brief 32位运算 左移
	* @note 返回LUA p1<<p2;
	*/
	static int L_Bit_LShift(lua_State* L);

	/**
	* @brief 32位运算 右移
	* @note 返回LUA p1>>p2
	*/
	static int L_Bit_RShift(lua_State* L);

	/**
	* @brief 32位运算　获得指定位的值
	* @note 返回p1的从p2开始到p3结束的位的值，描述为p1[p2,p3]，下标从１开始,如果p3没有指定，则默认为p1[p2,32]
	*/
	static int L_Bit_Get(lua_State* L);
	/**
	* @brief 32位运算　设置指定位的值
	* @note 设置p1的从p2开始到p3结束的位的值为p4,描述为p1[p2,p3]=p4,下标从１开始
	*/
	static int L_Bit_Put(lua_State* L);

	///元表函数，保护Helper对象内容不会被lua中修改
	static int L_NewIndex_Limit(lua_State* L);
	
	/**
	* @brief 字符串切分函数
	* @note 将字符串p1按p2(默认是';')切分开，返回一个表
	* @sa CHelper::StringSplit
	*/
	static int L_Spliter(lua_State* L);

	/**
	* @brief 字符串对切分
	* @note 将字符串"a=b;e=2"类似的字符串切分，返回一个表{a="b",e=2},其中p1为字符串内容，p2为切分字符串(默认为';')
	* @sa CHelper::StringGetPair
	*/
	static int L_StringGetPair(lua_State* L);

	///将lua中的lightuserdata转换为字符串，如果不是userdata，则返回nil
	static int L_DataToString(lua_State* L);

	///获得设备码 @sa CHelper::GetDeviceCode
	static int L_GetDeviceCode(lua_State* L);

	///网站用字符串编码函数 @sa CryptStr
	static int L_CryptStr(lua_State* L);

	///URL编码
	static int L_UrlEncode(lua_State* L);
	///URL解码
	static int L_UrlDecode(lua_State* L);

	///Base64编码
	static int L_Base64Encode(lua_State* L);
	///Base64解码
	static int L_Base64Decode(lua_State* L);

	///字符串替换,将p1中所有出现了的p2替换为p3
	static int L_StringReplace(lua_State* L);
	///获得设备名
	static int L_GetDeviceUserName(lua_State * L);
	///sha1编码
	static int L_Sha1(lua_State* L);

	///文件的sha1编码
	static int L_FileSha1(lua_State* L);

	///文件的MD5编码
	static int L_FileMd5(lua_State* L);

	///打开浏览器，p1为url
	static int L_OpenBrowser(lua_State* L);

	///MD5加密
	static int L_Md5(lua_State* L);
public:
    
    //Base64加密
    static StringA CryptStr(const StringA& strData,const StringA& strKeyValue,bool bEncode=true,int dwExpiry=0);
    
    static const StringA& Base64Encode( const char* pData,int nDataLen,StringA& strValue );
    
    static bool Base64Decode(const char* pData,size_t nDataLen,void* pDecodeData);

    ///LUA中默认的断点函数（仅输出断点信息)
	static int L_BreakPoint(lua_State* L);
	///文件拷贝,将p1拷贝到writeablepath+p2
	static int L_CopyFile(lua_State* L);
	///判断文件是否存在(cocos2d-x在lua中没有找到类似的函数)
	static int L_IsFileExist(lua_State* L);
	///从内存数据中加载一张图片纹理
	static int L_ImageFromData(lua_State* L);

	///创建目录
	static int L_CreateDir(lua_State* L);
	///创建目录实现
	static bool CreateDir(const char* strDir);

	///设置屏幕模式 @sa CHelper::SetScreenMode
	static int L_SetScreenMode(lua_State* L )
	{
		int nType = luaL_optint(L,1,0);
		int nWidth = luaL_optint(L,2,1024);
		int nHeight = luaL_optint(L,3,768);
		int nAlign = luaL_optint(L,4,3) ;//kResolutionFixedHeight
		CHelper::SetScreenMode(nType,nWidth,nHeight,nAlign);
		return 0;
	}
	///设置屏幕模式　@sa CHelper::SetScreenModeWithFrame
	static int L_SetScreenModWithFrame(lua_State* L)
	{
		int nType = luaL_optint(L,1,0);
		int nWidth = luaL_optint(L,2,1024);
		int nHeight = luaL_optint(L,3,768);
		int nAlign = luaL_optint(L,4,3) ;//kResolutionFixedHeight
		CHelper::SetScreenModeWithFrame(nType,nWidth,nHeight,nAlign);
		return 0;
	}

	///删除文件 
	static int L_DeleteFile(lua_State* L);

	///重命名文件
	static int L_RenameFile(lua_State* L);
private:
	static bool CreateDirImp(const StringA& strDir);

private:
	//转换URL编码
	static StringA& UrlEncode(StringA& strText , bool bUppercase);

	//URL解码
	static StringA& UrlDecode(StringA& strText);

	static char GetHexValue( char chr );

	
	static StringA MakeMd5(const StringA& strText);
	

	/**
	* @brief 用户数据加密
	* 
	*/
	static int L_RoleDataEncode(lua_State* L);

	/**
	* @brief 用户数据解密
	*/
	static int L_RoleDataDecode(lua_State* L);

	/**
	* @brief 获得广告推广码
	* @note 目前仅IOS有效，其他返回空字符串
	*/
	static int L_GetIDFACode(lua_State* L);

	static int L_StartCaptureAudio(lua_State* L);

	static int L_StopCaptureAudio(lua_State* L);

private:

};
