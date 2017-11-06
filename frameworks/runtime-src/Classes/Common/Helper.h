/**
* @brief 工具类
* @file Helper.h
*/
#pragma once

#include <vector>
#include <map>
#include <string>

///工具类
class DL_NOEXPORT CHelper
{
	NO_COPY_CLASS(CHelper);
	CHelper(void);
	~CHelper(void);
public:

	/**
	* @brief 将16进制字符串转换成字节流。
	* @param [in] pHex 字符串内容
	* @param [out] pBuff 接收的转换缓冲
	* @param [in,out] nBuffSize 传入pBuff大小，当成功后，nBuffSize为实际使用大小。
	* @return 成功返回非0
	*/
	static bool HexStringToData(const char* pHex,void* pBuff, int & nBuffSize);
		
	/**
	* @brief 将数据流转换成16进制字符串。
	* @param [in] pBuff 需要转换的数据流
	* @param [in] nBuffSize 数据字节数
	* @param [out] strHex 转换后输出字符串
	* @param [in] bUpcase 是否转换为大写，非0为大写。
	* @return 成功返回非0
	*/
	static bool DataToHexString(const void* pBuff,int nBuffSize,StringA& strHex, bool bUpcase = false );

	/**
	* @brief 将utf16字符串转换为当前多字节字符串(移动平台为utf8,Win平台为GBK) 
	*/
	static StringA Utf16ToStringA(const StringW& strUtf16);

	/**
	* @brief 多字节字符串(移动平台为utf8,Win平台为GBK) 转换为utf16字符串
	*/
	static StringW StringAToUtf16(const StringA& strUtf8);

	/**
	* @brief utf8转utf16	
	*/
	static StringW Utf8ToUtf16(const StringA& strUtf8)
	{
		StringW strUtf16;
		StringUtils::UTF8ToUTF16(strUtf8, strUtf16);
		return strUtf16;
	}

	/**
	* @brief utf16转utf8
	*/
	static StringA Utf16ToUtf8(const StringW& strUtf16)
	{
		StringA strUtf8;
		StringUtils::UTF16ToUTF8(strUtf16, strUtf8);
		return strUtf8;
	}

	/**
	* @brief 切分字符串，将strSrc按strDelimiter切分成数组，存入pvStrRet
	* @param [in] strSrc 要切分的字符串
	* @param [out] pvStrRet 返回的字符串向量数组
	* @param [in] strDelimiter 切分分隔符
	* @return 返回切分段数
	*/
	static int StringSplit( const StringA& strSrc,std::vector<StringA>* pvStrRet, const StringA& strDelimiter);
	
	/**
	* @brief 切分以等号隔开的字符串对
	* @param [in] strSrc 要切分的字符串
	* @param [out] pMapRet 返回的字符串对映射指针
	* @param [in] strDelimiter 切分分隔符
	* @return 返回切分段数
	*/
	static int StringGetPair( const StringA& strSrc,std::map<StringA,StringA>* pMapRet, const StringA& strDelimiter = ";");

	/**
	* @brief 将字符串中转义字符还原
	* @param [in,out] strText 需要转换的字符串
	* @return 返回转换后的字符串内容
	*/
	static StringA& DecoderString(StringA& strText);

	/**
	* @brief 编码转换，将特殊字符替换成XML/HTML转义字符
	* @param [in,out] strText 需要转换的字符串
	* @return 返回strText
	*/
	static StringA& EncoderString(StringA& strText);

	/**
	* @brief 内存查找
	* @param [in] pSrc 要查找的源内存指针
	* @param [in] nSrcSize 要查找的源内存大小，单位：字节
	* @param [in] pFind 要查找的内容
	* @param [in] nFindSize 要查找的内容长度
	* @param [in] nFrom 从源内存的多少字节偏移开始查找
	* @return 成功找到返回偏移量，没有找到则返回-1
	*/
	static size_t MemoryFind(const char* pSrc,size_t nSrcSize,const char* pFind,size_t nFindSize=0,size_t nFrom = 0);

	/**
	* @brief 字符串编码转化，GBK转UTF16-LE
	* @param [in] strGBK GBK字符串
	* @return 转换后的UTF16-LE字符串
	*/
	static StringW GBKToUtf16(const StringA& strGBK);

	/**
	* @brief 字符串编码转化，UTF16-LE转GBK
	* @param [in] strUtf16 需要转换的UTF16-LE字符串
	* @return 转换后的gbk字符串
	*/
	static StringA Utf16ToGBK(const StringW& strUtf16);
  
	/**
	* @brief 用浏览器打开指定网址
	* @param [in] lpUrl 网址
	* @note Windows、安卓下使用默认浏览器打开，ios下使用Safari
	*/
	static void OpenBrowser( const char* lpUrl );
   
	///截屏代码
    static void ScreenShot();

	///获取电量
	static float GetElectricity();

	///获得设备码
	static bool GetDeviceCode(char szDeviceCode[DEVICE_CODE_LEN]);

	///获得计算机名
	static void GetDeviceUserName( StringA& strUserName );

	/**
	* @brief 转屏控制，不改变框架大小
	* @param [in] nType 转屏方向,0:横屏；1：竖屏
	* @param [in] nWidth 屏幕适配宽度,单位：像素
	* @param [in] nHeight 屏幕适配高度,单位:像素
	* @param [in] nAlgin 适配模式，为ResolutionPolicy值中的一个
	*/
	static void SetScreenMode(int nType,int nWidth,int nHeight,int nAlgin);
	/**
	* @brief 转屏控制，改变框架大小
	* @param [in] nType 转屏方向,0:横屏；1：竖屏
	* @param [in] nWidth 框架宽度,单位：像素
	* @param [in] nHeight 框架高度,单位:像素
	* @param [in] nAlign 适配模式，为 ResolutionPolicy 值中的一个
	*/
	static void SetScreenModeWithFrame( int nType,int nWidth,int nHeight,int nAlign );
	/**
	* @brief 获得广告推广码
	* @note 目前仅IOS有效，其他返回空字符串
	*/
	static StringA GetIDFACode(void);
    /**
     * @brief 获得lua 全局变量
     * @param [in] lpVarName  Lua 全局变量名字
     */
    static StringA GetLuaGlobal(const char* lpVarName);

    /**
     * @brief 获取缓存目录
     */
    static StringA GetCachePath();
    
    static int GetDeviceBatteryLevel();

	static StringA StringReplace(const StringA& strSrc, const StringA& strOld, const StringA& strNew);

};

#ifdef WIN32
///GBK编码转换成UTF8编码
inline StringA ___GBK_2_utf8_(const StringA& strGBK,StringA&)
{
	return CHelper::Utf16ToUtf8(CHelper::StringAToUtf16(strGBK));
}

///GBK编码转换成UTF8编码
inline const char* ___GBK_2_utf8_(const char* strGBK,StringA& strUtf8)
{
	//static StringA strUtf8;
	strUtf8 = CHelper::Utf16ToUtf8(CHelper::StringAToUtf16(strGBK));
	return strUtf8.c_str();
}
///GBK编码转换成UTF8编码
#define _UTF8(str)	\
	__pragma(warning(disable:4239))\
	___GBK_2_utf8_(str,StringA())\
	__pragma(warning(default:4239))

#else
#define _UTF8(str) (str)
#endif

/**
* @def _UTF8
* @brief 将字符串由GBK编码转换为utf8编码
* @note 仅windows下有效
*/
