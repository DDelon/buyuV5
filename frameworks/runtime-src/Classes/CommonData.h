/**
* @file 	CommonData.h
* @brief	平台全局数据定义
* @date		2014年3月19日 10:06	
* @by 		首席村支书
* @Email 	wangbing323@163.com
* @QQ		32996758
*********************************************************************/


#pragma once

/**
* @brief 非拷贝类
* @note 该宏修饰的类不能拷贝构造或者赋值
*/
#ifndef NO_COPY_CLASS
#define NO_COPY_CLASS(cls) private:cls(cls&);\
	cls& operator = (cls&)
#endif //NO_COPY_CLASS

#ifndef SAFE_RELEASE
///安全释放引用计数
#define SAFE_RELEASE(p) if(p){p->Release();p=NULL;}
#endif//SAFE_RELEASE

#ifndef SAFE_DELETE
///安全删除指针
#define SAFE_DELETE(p) if(p){delete p;p =NULL;}
#endif//SAFE_DELETE

#ifndef SAFE_DELETE_ARR
///安全删除数组
#define SAFE_DELETE_ARR(p) if(p){delete []p;p=NULL;}
#endif //SAFE_DELETE_ARR

#ifndef ARR_SIZE
///数组大小
#define ARR_SIZE(a) (sizeof(a)/sizeof(a[0]))
#endif//ARR_SIZE
#include <memory.h>
#include <string.h>
#ifdef WIN32
#pragma warning(push,3)
#pragma warning(disable:4251)
#elif CC_PLATFORM_IOS == CC_TARGET_PLATFORM
#pragma warning(push,1)
#endif

#ifdef WIN32
#pragma warning(push,3)
#pragma warning(disable:4244)
#include "cocos2d.h"
#include "cocos-ext.h"
#include "scripting/lua-bindings/manual/CCLuaEngine.h"
#pragma warning(push,4)
#pragma warning(default:4244)
#else
#include "cocos2d.h"
#include "cocos-ext.h"
#include "scripting/lua-bindings/manual/CCLuaEngine.h"
#endif

#ifdef WIN32
#pragma warning(default:4251)
#pragma warning(push,4)
#elif CC_PLATFORM_IOS == CC_TARGET_PLATFORM
#pragma warning(pop)
#endif

///使用cocos2d-x命名空间
USING_NS_CC;
///使用cocos2d-x扩展命名空间
USING_NS_CC_EXT;


#ifdef WIN32
///未引用的标识符
#define UNREF(p) (void)p
///排除windows不需要的库
#define WIN32_LEAN_AND_MEAN  
#include <WinSock2.h>
#include <Windows.h>
#include <Guiddef.h>
///废弃的变量、函数、或者类
#define DEPRECATED(msg) __declspec(deprecated(msg))
extern "C"
{
#include "tolua++.h"
#include "zconf.h"
#include "zlib.h"
};

/**
* @brief 日志类型，WriteLog函数的第一个参数
*/
enum emLogType
{
	LOG_NORMAL,		//!<普通日志
	LOG_INFO,		//!<信息日志
	LOG_WARNING,	//!<警告日志
	LOG_ERROR,		//!<错误日志
	LOG_DEBUG,		//!<调试日志
	LOG_COMMAND,	//!<命令日志
};
/**
* @brief 写入日志

* @param[in] dwLogFilter 日志类型，值为 @ref emLogType 中的一种
* @param[in] fmt 要写入日志的字符串格式
*/
void WriteLog(DWORD dwLogFilter,const char* fmt,...);
///DLL导出接口
#define DL_EXPORT __declspec(dllexport)
///DLL无导出
#define DL_NOEXPORT 
#else
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
///DLL导出接口
#define DL_EXPORT __attribute__((visibility("default")))
///DLL无导出
#define DL_NOEXPORT __attribute__((visibility("hidden")))
#else
///DLL导出接口
#define DL_EXPORT
///DLL无导出
#define DL_NOEXPORT
#endif
///标记废弃属性
#define DEPRECATED(msg) __attribute__((deprecated(msg)))
///没有使用的变量，避免编译错误
#define UNREFERENCED_PARAMETER(p) (void)p 
///没有使用的变量，避免编译错误
#define UNREF UNREFERENCED_PARAMETER
///BYTE
typedef unsigned char       BYTE;
///WORD
typedef unsigned short      WORD;
///DWORD
typedef unsigned int       DWORD;
///LONGLONG
typedef long long           LONGLONG;
///ULONGLONG
typedef unsigned long long  ULONGLONG;
///TCHAR
typedef WORD				TCHAR;
///WCHAR
typedef WORD				WCHAR;
extern "C"
{
#include <zconf.h>
#include <zlib.h>
};

/**
* @brief GUID结构
*/
struct GUID
{
	char szData[16]; //!<GUID大小为16字节
};

/**
* @brief 系统时间结构
*/
struct SYSTEMTIME {
	WORD wYear;		//!<年
	WORD wMonth;	//!<月
	WORD wDayOfWeek;//!<星期中第几天
	WORD wDay;		//!<天
	WORD wHour;		//!<小时
	WORD wMinute;	//!<分钟
	WORD wSecond;	//!<秒
	WORD wMilliseconds;//!<毫秒
};

/**
* @brief 重载GUID的等于运算符.
* map和hash_map的key必须支持小于运算符，为了能让GUID能做为key，所以重写该运算符
* @param [in] lhs 参与比较时小于号左边的GUID值
* @param [in] rhs 参与比较时小于号右边的GUID值
* @retval lhs与rhs内存ASCII比较，如果lhs等于rhs，返回true,否则返回false
*/
inline bool operator==(const GUID& lhs,const GUID& rhs)
{
	return memcmp(&lhs,&rhs,sizeof(GUID))==0;
}
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include <android/log.h>
#define LOG_NORMAL ANDROID_LOG_VERBOSE	//!<普通日志
#define LOG_INFO  ANDROID_LOG_INFO		//!<信息日志
#define LOG_WARNING ANDROID_LOG_WARN	//!<警告日志
#define LOG_ERROR   ANDROID_LOG_ERROR	//!<错误日志
#define LOG_DEBUG  ANDROID_LOG_DEBUG	//!<调试日志
#define LOG_COMMAND ANDROID_LOG_UNKNOWN	//!<命令日志
///输出日志(仅安卓)
#define  WriteLog(logtype,...)  __android_log_print(logtype,"GameHall",__VA_ARGS__)
#else //IOS

/**
* @brief 日志类型，WriteLog函数的第一个参数
*/
enum emLogType
{
	LOG_NORMAL,		//!<普通日志
	LOG_INFO,		//!<信息日志
	LOG_WARNING,	//!<警告日志
	LOG_ERROR,		//!<错误日志
	LOG_DEBUG,		//!<调试日志
	LOG_COMMAND,	//!<命令日志
};

extern void WriteLog(DWORD dwLogFilter,const char* fmt,...);
#endif //CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#endif//WIN32
#include <assert.h>
#include <string>


typedef std::basic_string<char>             StringA;     //!<GBk,utf8编码字符串
typedef std::u16string   StringW;    //!<UTF16编码


enum
{
	MSG_PACKET_DATA		=0xFFFE,	//!<消息包
	MSG_KEEPALIVE		=0xFFFF,	//!<心跳机制
	MSG_PACKET_SIZE		= 6000,		//!<消息包大小

	MSG_MAX_SIZE				=8192,	//!<消息最大长度
	MSG_SUIT_SIZE				= 8000,	//!<消息适当大小
	MSG_MAX_PACKET				= 8,	//!<最大消息包数量
};

///消息附加参数
enum MSG_HEADER_FLAG
{
	MSG_HEADER_FLAG_NONE		= 0,	//!<发送数据不做任何处理
	MSG_HEADER_FLAG_PACKET		= 0x1,	//!<当前为包文件（一个或者多个连续包），每个包都有个包标记@brief msg_Packet 当所有包首发完毕才能处理
	MSG_HEADER_FLAG_MASK		= 0x2,	//!<检查掩码,@ref	msg_Header::byMask 将当前包的校验码，当前包也会重组数据
	MSG_HEADER_FLAG_ENCODE		= 0x4,	//!<重新编码,使用映射表重新编码数据
	MSG_HEADER_FLAG_COMPRESS	= 0x8,	//!<压缩数据,数据将启用zlib压缩，该标记仅推荐，如果数据压缩后更大，将忽略该标记
	MSG_HEADER_FLAG_ROUTE		= 0x10,	//!<路由包,服务器每次中转，@ref	msg_Header::byRouteCount 的值累加1，当到达特定值后，仍然没有到达目标服务器，该包将丢弃
	MSG_HEADER_FLAG_DELAYSEND	= 0x20,	//!<延迟消息，等待下次非延迟消息合并发送（客户端忽略该标记）

	MSG_HEADER_FLAG_FILLED		= 0x40,	//!<已经填充头部（已经进行掩码、编码、压缩处理)，不能重复处理
	MSG_HEADER_FLAG_OFFSET		= 0x80	//!<指定消息头偏移，掩码、编码、压缩处理将从消息头开始偏移@ref	msg_Header::byHeaderOffset 个字节开始
};

	/**
	*@ brief 输出消息类型
	*/
	enum printMessageType
	{	
		PRINT_MSG_TYPE_NONE		= 0,	//!<不附带任何内容，纯文本


		PRINT_MSG_TYPE_P2P		= 0x1,	//!<玩家跟玩家
		PRINT_MSG_TYPE_SYS		= 0x2,	//!<系统消息
		PRINT_MSG_TYPE_FACTION2P= 0x4,	//!<门派内部聊天
		PRINT_MSG_TYPE_FACTION	= 0x8,	//!<门派系统消息
		PRINT_MSG_TYPE_GM2P		= 0x10,	//!<GM跟玩家
		
		PRINT_MSG_TYPE_SPEAKER  = 0x20,	//!<小喇叭
		PRINT_MSG_TYPE_ROSE		= 0x40,	//!<玫瑰花
		PRINT_MSG_TYPE_PROP		= 0x80,	//!<道具
		PRINT_MSG_TYPE_HALL		= 0x100,//!<大厅消息
		PRINT_MSG_TYPE_CMD		= 0X200,//!<消息为命令
		PRINT_MSG_TYPE_GIFT		= 0X400,//!<奖励（喜报）
		PRINT_MSG_TYPE_SAVE2DB		= 0X80000000,	//!<是否保存（保存到数据库)
	};

	/**
	* @brief 比赛阶段信息	
	*/
	enum pkGroupState
	{
		PK_GROUP_STATE_WAITING,		//等待状态(时间没到)
		PK_GROUP_STATE_JOIN,		//加入状态
		//PK_GROUP_STATE_GAMING,	//游戏状态
		PK_GROUP_STATE_STEP,		//阶段1
		//PK_GROUP_STATE_STEP2,		//阶段2
		//PK_GROUP_STATE_STEP3,		//阶段3
		//PK_GROUP_STATE_FREE,		//空闲状态
	};

#pragma pack(push)
#pragma pack(1)
/**
* @brief 消息结构体
* @note 平台所有网络消息包前面都包含改结构
*/
typedef struct msg_Header
{
	WORD	wLen;				//!<当前消息长度，最大不能超过8192.单位：字节
	WORD	wMessageID;			//!<消息ID
	BYTE	byMask;				//!<奇偶校验码
	BYTE	byFlag;				//!<附加标记,参看 @ref  MSG_HEADER_FLAG
	BYTE	byRouteCount;		//!<路由次数
	BYTE	byHeaderOffset;		//!<自定义消息头偏移
}MSG_HEADER, * LPMSG_HEADER;//!<@see msg_Header

/**
* @brief 心跳消息包
* @note 客户端应当每隔一段时间给服务器发送一个该包
*/
struct msg_KeepAlive:public MSG_HEADER
{
	DWORD	dwTimeStamp;		//!<时间戳
	DWORD	dwDelayTime;		//!<延迟时间

};

/**
* @brief 当消息为包消息时，消息尾部为该结构
* @note 必须所有的guidPacket相同的包全部凑齐了，才能处理
*/
struct msg_Packet
{
	GUID	guidPacket;		//!<包标识ID
	DWORD	dwPacketSize;	//!<包总长度,单位：字节
	DWORD	dwRealSize;		//!<实际大小,单位：字节
	DWORD	dwIndex;		//!<包索引

};
#pragma pack(pop)
///长度定义
enum
{
	USER_NAME_LEN		= 36,	//!<用户名长度
	MD5_PSW_LEN			= 16,	//!<MD5密码长度

	UNDER_WRITE_LEN		= 50,	//!<个性签名长度

	GAME_NAME_LEN		= 20,	//!<游戏名长度

	GAME_SHORT_NAME_LEN = 5,	//!<游戏缩写名长度

	GAME_SHELL_FILE_LEN = 32,	//!<游戏可执行文件名长度

	ROOM_NAME_LEN		=40,		//!<房间名长度
	COMMAND_LEN			= 260,		//!<服务器附加命令长度
		
	PASS_WORD_LEN		= 20,		//!<普通密码长度
	ROOM_MSG_LEN		= 1024,		//!<房间消息长度
	AVATAR_IMG_PATH_LEN = 128,		//!<头像地址长度
	MAX_USER_NAME_LEN	= 36,		//!<实际用户名最大长度
	MISSION_CONTEXT_LEN	= 128,		//!<任务描述长度

	EFFORT_DATA_LEN		= 4000,		//!<任务数据最大长度
	EFFORT_DATA_COUNT	= EFFORT_DATA_LEN/sizeof(LONGLONG),	//!<任务总数据量

	SHOP_GOODS_NAME		= 20,		//!<商店物品名
	SHOP_GOODS_DESCRIPTION = 128,	//!<商店物品信息描述
	ID_CARD_LEN			= 18,		//!<身份证号
	FLYSHEET_ID_LEN		= 36,		//!<传单ID长度
	MOBILE_PHONE_LEN	= 16,		//!<手机号码长度
	QUESTION_ANSWER_LEN	= 10,		//!<密码提示问题答案长度

	DEVICE_CODE_LEN		= 42,		//!<设备ID长度
	GAME_EXTAND_DATA_LEN = 128,		//!<游戏附加数据
};

///平台其他字符串最大长度定义
enum emStringsLengthDefine 
{
	///ip地址或者域名长度
	IP_ADDRESS_LEN	= 64,			
	///mac地址长度
	MACADDRESS_LEN = 6,				
};

#define MAKEMSGOFFSET(m) ((m)<<12) //!<消息高4字节为标识
#define GETMSGOFFSET(m) ((m)>>12)  //!<获取消息高4字节内容

///消息分段定义，每段消息最多有4096个标识
enum 
{
	MSG_OFFSET_BASE_LOGIC,				//!<基础逻辑消息
	MSG_OFFSET_LOBBY_TO_GAMESERVER,		//!<游戏逻辑服务器到客户端大厅
	MSG_OFFSET_LOBBY_TO_HALLSERVER,		//!<大厅到大厅服务器
	MSG_OFFSET_GAMESERVER_TO_HALLSERVER,//!<游戏服务器到大厅服务器
	MSG_OFFSET_LOGIC_TO_CENTER,			//!<逻辑服务器到中央服务器

	MSG_OFFSET_ROUTE,					//!<中转消息
	MSG_OFFSET_CUSTOM,					//!<或者异步调用自己,管道消息，进程之间通讯
	MSG_OFFSET_COMMON,					//!<通用
};

///用户类型
enum USER_TYPE
{
	USER_TYPE_ROBOT,	//!<机器人
	USER_TYPE_NORMAL,	//!<普通用户
	USER_TYPE_VIP,		//!<会员
	USER_TYPE_GM = 250,	//!<2B的管理员
	USER_TYPE_SUPER_GM,	//!<超级管理员
};

///游戏中禁言设置
enum CHAT_LIMIT
{
	CHAT_LIMIT_NONE			=0,				//!<没有禁言
	CHAT_LIMIT_DESK			=0x1,			//!<禁止游戏中聊天
	CHAT_LIMIT_ROOM			=0x2,			//!<禁止房间内聊天
	CHAT_LIMIT_FRIEND		=0x4,			//!<禁止与好友聊天
	CHAT_LIMIT_FACTION		=0x8,			//!<禁止门派聊天
	CHAT_LIMIT_LOUDSPEADER	=0x10,			//!<禁止使用喇叭
	CHAT_LIMIT_ALL			=0xFFFFffff,	//!<禁止所有频道
};

///游戏桌子状态
enum DESK_STATE
{
	DESK_STATE_FREE	= 0,				//!<桌子空闲状态
	DESK_STATE_GAMING,					//!<桌子游戏状态
	DESK_STATE_LOCKED,					//!<桌子上锁状态
};

///房间类型（底层使用28种，游戏开发者可使用4种）
enum ROOM_TYPE
{
	ROOM_TYPE_MONEY			= 0x0001,		//!<游戏币房间
	ROOM_TYPE_TEMPLATE		= 0X0002,		//!<临时货币房间，进入房间分配金币，退出回收，不影响身上货币
	ROOM_TYPE_TEMPSCORE		= 0x0004,		//!<临时积分，进入房间分配固定积分
	ROOM_TYPE_RATETAX		= 0x0008,		//!<可变税率，房间的tax属性为百分比。如果没有此属性，则房间的tax属性为固定扣税值
	ROOM_TYPE_HIDDEN		= 0x0010,		//!<房间内玩家信息不可见，并且无法在房间和游戏中聊天。

	ROOM_TYPE_BASEMONEY		= 0X0020,		//!<基础底注房间，房间的BaseMoney属性有效。没有该属性，则房间内允许每桌不同底注
	ROOM_TYPE_NO_PROP		= 0x0040,		//!<房间内禁止使用道具
	ROOM_TYPE_VIP			= 0x0080,		//!<房间为VIP房间，非VIP用户无法进入
	ROOM_TYPE_VIDEO			= 0x0100,		//!<房间为视频房间。
	ROOM_TYPE_TEST			= 0X0200,		//!<房间为测试房间，所有输赢属性均不计算
	ROOM_TYPE_GAMING_JOIN	= 0x0400,		//!<房间类型为游戏开始后仍可进入类型
    ROOM_TYPE_UPGRADE		= 0x0800,		//!<闯关场
    ROOM_TYPE_DESKPK		= 0x1000,		//!<单桌比赛
    ROOM_TYPE_GIFTMONEY		= 0x2000,		//!<送豆场
    ROOM_TYPE_UPGRADE2		= 0X4000,		//!<闯关场（分场闯关)

	//房间等级类型，共4种,位数22-24位
	ROOM_TYPE_LEVEL_MASK = 0x00C00000,		//!<房间类型掩码

	ROOM_TYPE_LEVEL_0	= 0<<22,		//!<新手房间
	ROOM_TYPE_LEVEL_1	= 1<<22,		//!<初级房间
	ROOM_TYPE_LEVEL_2	= 2<<22,		//!<中级房间
	ROOM_TYPE_LEVEL_3	= 3<<22,		//!<高级房间

	//落座模式，共15种，位数24-28位
	ROOM_TYPE_SIT_MODE_MASK = 0x0F000000,	//!<落座模式掩码（共16种）

	ROOM_TYPE_FREE_MODE = 0<<24,		//!<自由落座模式
	ROOM_TYPE_ALLOCSIT	= 1<<24,		//!<房间内玩家座位由系统分配，无法自己选择座位
	ROOM_TYPE_ALLOCSIT2 = 2<<24,		//!<系统分桌模式，随机分配位置
	ROOM_TYPE_TEAM		= 3<<24,		//!<组队模式房间

	ROOM_TYPE_PK_MODE1	= 8<<24,		//!<比赛模式1
	ROOM_TYPE_PK_MODE2  = 9<<24,		//!<比赛模式2
	ROOM_TYPE_PK_MODE3	= 10<<24,		//!<比赛模式3

	//开发者自定义，取值范围0~15（或四种组合)，位数28-32位
	ROOM_TYPE_CUSTOM_MASK = 0xF0000000,		//!<自定义类型掩码
	ROOM_TYPE_CUSTOM1	= 0x10000000,		//!<给与游戏开发者4种自定义房间类型1
	ROOM_TYPE_CUSTOM2	= 0x20000000,		//!<给与游戏开发者4种自定义房间类型2
	ROOM_TYPE_CUSTOM3	= 0x40000000,		//!<给与游戏开发者4种自定义房间类型3	
	ROOM_TYPE_CUSTOM4	= 0x80000000,		//!<给与游戏开发者4种自定义房间类型4	
};

///获得房间落座模式
#define GET_ROOM_MODE(roominfo) ((roominfo)&ROOM_TYPE_SIT_MODE_MASK)	
///获得房间自定义值
#define GET_ROOM_TYPE_CUSTOM(roominfo) (((roominfo)&ROOM_TYPE_CUSTOM_MASK)>>28)//右移28位
///获得房间级别
#define GET_ROOM_LEVEL(roominfo) (((roominfo)&ROOM_TYPE_LEVEL_MASK)>>22)//右移22位

/**
* @brief 用户登陆客户端类型 
*/
enum CLIENT_TYPE
{
	CLIENT_TYPE_UNKOWN		= 0x0,			//!<未知类型
	CLIENT_TYPE_PC			= 0x1,			//!<PC登陆(传统登陆
	CLIENT_TYPE_WEB			= 0x2,		//!<网页登陆
	CLIENT_TYPE_ANDROID		= 0x4,	//!<安卓登陆
	CLIENT_TYPE_ANDROID_PAD	= 0x8,//!<安卓平板
	CLIENT_TYPE_ANDROID_TV	= 0x10,	//!<安卓电视
	CLIENT_TYPE_IPHONE		= 0x20,		//!<iphone登陆
	CLIENT_TYPE_IPAD		= 0x40,		//!<IPAD登陆
	CLIENT_TYPE_ITV			= 0x80,		//!<IOS系统电视

	CLIENT_TYPE_SUPPORT_JIXIANG		= 1<<14,	//!支持吉祥平台
	CLIENT_TYPE_SUPPORT_WEILE		= 1<<15,	//!支持微乐平台

	CLIENT_TYPE_PLATFORM_MASK = 0xC000,			//!平台掩码
	CLIENT_TYPE_DEVICE_MASK	= 0xFFF,			//!设备掩码
	CLIENT_TYPE_ALL			= 65535,		//!<全平台支持
};
/**
* @brief  游戏分类
*/
enum GAME_GROUP
{
	GAME_GROUP_UNKOWN		= 0x00000000,	//!<未知类
	GAME_GROUP_POKER		= 0x00000001,	//!<扑克类游戏
	GAME_GROUP_MAHJONG		= 0x00000002,	//!<麻将类游戏
	GAME_GROUP_CHESS		= 0x00000004,	//!<棋类游戏
	GAME_GROUP_LEISURE		= 0x00000008,	//!<休闲类游戏
	GAME_GROUP_FLASH		= 0x00000010,	//!<Flash游戏(WEB游戏)
	GAME_GROUP_ALONE		= 0x00000020,	//!<单机游戏
		
	GAME_PACKET_LEISURE		= 1<<20,	//!<休闲游戏包，共可使用15种包，值不能超过0x01000000
	GAME_PACKET_POKER		= 2<<20,	//!<扑克包
	GAME_PACKET_MAHJONG		= 3<<20,	//!<麻将包
	GAME_PACKET_COMMON		= 4<<20,	//!<通用包

	GAME_GROUP_LIVE			= 0X01000000,	//!<娱乐类
	GAME_GROUP_PKLIST		= 0x02000000,	//!<比赛分组

	GAME_GROUP_MYGAME		= 0x04000000,	//!<我的游戏

	GAME_GROUP_OTHER		= 0x80000000,	//!<其他类型
};

///最大15中包类型
#define MAX_GAME_PACKET_COUNT	 15
/**
* @brief 帐号来源 
*/
enum USER_FROM
{
	USER_FROM_UNKOWN,		//!<未知来源
	USER_FROM_UNLOGIN,		//!<游客身份
	USER_FROM_PLATFORM,		//!<帐号来自当前平台
	USER_FROM_TENCENT,		//!<帐号来自腾讯
	USER_FROM_WEIBO,		//!<帐号来自新浪微博
	USER_FROM_JIXIANG = 7,		//!<帐号来自吉祥
};

/**
	* @brief 通用道具  
	*/
enum propIDType
{
	PROP_ID_SPEAKER		=1,				//!<小喇叭
	PROP_ID_PK_TICKET,					//!<参赛券
	PROP_ID_SPREAD_PRESENT,				//!<推广礼盒
	PROP_ID_LOTTERY_CARD,				//!<抽奖卡
	PROP_ID_ACTIVE_CARD,				//!<激活卡
	PROP_ID_FLEE_CLEAR,					//!<逃跑率清零
	PROP_ID_7,							//!<拖鞋
	PROP_ID_8,							//!<鸡蛋
	PROP_ID_9,							//!<发泄
	PROP_ID_10,							//!<玫瑰
	PROP_ID_11,							//!<吻
	PROP_ID_12,							//!<纸巾

	PROP_ID_13,							//!<表情

	PROP_ID_BANK,							//!<别墅


	PROP_ID_MONEY,						//!<游戏币
	PROP_ID_LOTTERY,					//!<元宝
	PROP_ID_XZMONEY,					//!<吉祥币
	PROP_ID_RMB,						//!<人民币
	PROP_ID_VIP,						//!<VIP

 	PROP_ID_CARD1,						//!<卡片套装1
 	PROP_ID_CARD2,
 	PROP_ID_CARD3,
 	PROP_ID_CARD4,
 	PROP_ID_CARD5,
 	PROP_ID_CARD6,
 	PROP_ID_CARD7,
 	PROP_ID_CARD8,
 	PROP_ID_CARD9,
		
		

	PROP_ID_TYPE_WITH_TIME=256,				//!<后续道具跟时间相关
	PROP_ID_WEEK_PK_CARD = 256,				//!<周赛参赛券
	PROP_ID_MONTH_PK_CARD,				//!<月赛参赛券

	PROP_ID_TEMP			,				//!<临时道具，比如月饼

};


///任务类型
enum missionType
{
	MISSION_TYPE_EFFORT,			//!<成就任务
	MISSION_TYPE_DAILY,				//!<日常任务
	MISSION_TYPE_TEMP,				//!<临时任务
	MISSION_TYPE_RANDOM,			//!<随机任务
};

///任务数据比较函数
enum dataCompareFun
{
	DATA_COMPARE_FUN_NEVER,		//!<永远不成立
	DATA_COMPARE_FUN_LESS,		//!<小于
	DATA_COMPARE_FUN_EQUAL,		//!<等于
	DATA_COMPARE_FUN_LESSEQUAL,	//!<小于等于
	DATA_COMPARE_FUN_GREATER,	//!<大于
	DATA_COMPARE_FUN_NOTEQUAL,	//!<不等于
	DATA_COMPARE_FUN_GREATEEQUAL,//!<大于等于
	DATA_COMPARE_FUN_AWAYS,		//!<永远为成立
};


#pragma pack(push,8)
#pragma pack(8)
///游戏消息扩展结构
typedef struct tagGameMsgExData	
{
	DWORD dwServerID;				//!<目标服务器ID
	DWORD dwFromServer;				//!<发送服务器ID
	DWORD dwPlayer;					//!<用户ID
	DWORD dwRoomID;					//!<房间ID(可选)

}GAMEMSGEXDATA,*LPGAMEMSGEXDATA;//!<@see tagGameMsgExData

///用户基本信息，4字节对齐
typedef struct tagUserInfo
{
	LONGLONG llUserMoney;			//!<用户金钱
	LONGLONG llBankMoney;			//!<用户存款

	DWORD	dwUserID;				//!<用户ID

	//DWORD	dwFactionID;			//社团ID
	//DWORD	dwLoveliness;			//魅力值


	DWORD	dwXZMoney;				//!<鑫泽币（吉祥币）
	DWORD	dwLottery;				//!<奖励券（元宝）
	DWORD	dwPrestige;				//!<声望值

	BYTE	byUserType;				//!<用户类型,为@ref USER_TYPE 中的一个值
	BYTE	byUserFrom;				//!<用户来源，为@ref USER_FROM 中的一个或多个值

	WORD	wClientType;			//!<登陆类型，为@ref CLIENT_TYPE 中的一个值

	DWORD	dwAvatarID;				//!<头像ID,为UINT_MAX(值为0XFFFFffff)则为自定义头像
	DWORD	dwClothID;				//!<服装ID
	DWORD	dwVIPValue;				//!<VIP累积值
	//INT		nUserState;				//!<用户状态，为 0 为正常用户，非0则帐号锁定码
	int		nChatLimit;				//!<聊天限制,为@ref CHAT_LIMIT 的一个或多个值的组合
	DWORD	dwUserTireTime;			//!<疲劳时间，为UINT_MAX则不计沉迷时间
	int	bSex;					//!<性别，1：男；0：女

	//TCHAR	szUserName[USER_NAME_LEN];//!<用户名
	char16_t	szNickName[USER_NAME_LEN];//!<昵称
	char16_t	szUnderWrite[UNDER_WRITE_LEN];//!<个性签名
	char	szAvatarImgPath[AVATAR_IMG_PATH_LEN];//!<头像地址

}USERINFO,*LPUSERINFO; //!<@sa tagUserInfo

///游戏信息
typedef struct tagGameInfo
{
	ULONGLONG		llCurrentVersion;	//!<当前版本
	ULONGLONG		llSupportVersion;	//!<支持的版本

	DWORD			dwGameID;			//!<游戏ID
	DWORD			dwGameType;			//!<游戏类型,为@ref GAME_GROUP 中的一个值或多个值
	WORD			wClientTypeSupport;		//!<游戏客户端支持,为@ref CLIENT_TYPE 中的一个值或多个值
	BYTE			byDisable;			//!<是否禁用游戏（是否客户端显示)
	BYTE			byReserved;			//!<保留

	char16_t		szGameName[GAME_NAME_LEN];			//!<游戏名
	char16_t		szShortName[GAME_SHORT_NAME_LEN];	//!<游戏缩写名，最多4字符
	char16_t		szShellFile[GAME_SHELL_FILE_LEN];	//!<游戏命令行
	char16_t		szCommand[COMMAND_LEN];				//!<执行文件名

}GAMEINFO,*LPGAMEINFO;//!< @sa tagGameInfo

///房间信息
typedef struct tagRoomInfo
{
	DWORD	dwRoomID;			//!<房间ID
	DWORD	dwRoomType;			//!<房间类型
	DWORD	dwServerID;			//!<服务器ID；
	DWORD	dwGameID;			//!<游戏ID
	DWORD	dwBaseMoney;		//!<房间底注,如果房间属性为@ref ROOM_TYPE_BASEMONEY 有效
	float   fTax;				//!<税率.房价属性为@ref ROOM_TYPE_RATETAX 则该参数为扣税系数，否则为扣税值
	float	fTaxVIP;			//!<VIP税率.房价属性为@ref ROOM_TYPE_RATETAX 则该参数为扣税系数，否则为扣税值
		
	int		nMinMoneyLimit;		//!<最小金钱或者积分限制 
	int		nMaxMoneyLimit;		//!<最大金钱或者积分限制
	DWORD	dwJoinRight;		//!<加入权限

	WORD	wCriticalPlayers;	//!<临界玩家数
	WORD	wMaxPlayers;		//!<最大玩家数
	WORD	wDeskCount;			//!<桌子数量
	WORD	wPlayersPerDesk;	//!<每桌玩家数
	WORD	wLeastPlayersWhenStart;//!<至少多少个玩家才能开始
	WORD	wClientTypeSupport;		//!<支持的客户端类型，为@ref CLIENT_TYPE 中的一个值或多个值
	char16_t	szRoomName[ROOM_NAME_LEN];	//!<房间名
	char16_t	szRoomCommand[COMMAND_LEN];	//!<房间命令行

}ROOMINFO,*LPROOMINFO; //!<@sa tagRoomInfo


///房间扩展信息
typedef struct tagRoomInfoAndPlayers:public ROOMINFO
{
	WORD wPlayers;	//!<房间玩家数量
}ROOMINFOEX,*LPROOMINFOEX; //!< @sa tagRoomInfoAndPlayers

///关系节点类型
enum RelationType
{
	RELATION_TYPE_ROOT,		//!<节点为顶级节点
	RELATION_TYPE_GAME,		//!<节点为游戏
	RELATION_TYPE_ROOM,		//!<节点为房间
	RELATION_TYPE_RELATION,	//!<节点为节点
};

///关系节点信息
typedef struct tagRelationInfo
{
	DWORD	dwRelationID;			//!<关系节点ID
	DWORD	dwIcon;					//!<图标索引
	DWORD	dwParentType;			//!<父节点类型
	DWORD	dwParentID;				//!<父节点ID
	DWORD	dwObjectType;			//!<对象类型
	DWORD	dwObjectID;				//!<对象ID
	DWORD	dwSortIndex;			//!<排序序号
	DWORD	dwClientType;			//!<支持的客户端类型
	char16_t	szName[USER_NAME_LEN];	//!<节点名,如果为空，则使用默认对象名
	char16_t	szCommand[COMMAND_LEN];	//!<命令行
}RELATIONINFO,*LPRELATIONINFO; //!<@sa tagRelationInfo

///玩家游戏数据
typedef struct tagUserGameInfo
{
	LONGLONG		llScore;		//!<玩家积分
	DWORD	dwWinCount;				//!<玩家赢局数
	DWORD	dwLostCount;			//!<玩家输的局数
	DWORD	dwDrawCount;			//!<玩家平局数
	DWORD	dwFleeCount;			//!<玩家逃跑局数
	int		nGameData1;				//!<附加游戏数据1
	int		nGameData2;				//!<附加游戏数据2
	DWORD	dwPKKempCount;			//!<冠军次数
	DWORD	dwPKAwardCount;			//!<比赛获奖次数
	DWORD	dwPKCount;				//!<比赛次数
	DWORD	dwPKWinCount;			//!<比赛赢的总局数
	DWORD	dwPKEliminate;			//!<比赛淘汰人数

	WORD	wDeskID;				//!<玩家桌子ID
	WORD	wChairID;				//!<玩家椅子ID

	SYSTEMTIME tmLastSave;			//!<最后存储时间
	BYTE	cbTaskData[GAME_EXTAND_DATA_LEN];		//!<游戏任务数据
	BYTE	cbMacthData[GAME_EXTAND_DATA_LEN];		//!<游戏附加存数数据

	BYTE	byReady;				//!<玩家是否准备
	BYTE	byLookOn;				//!<玩家是否旁观
	WORD	wReserved;				//!<预留数据
	DWORD	dwCustom;				//!<游戏开发者定义

}USERGAMEINFO,*LPUSERGAMEINFO;	//!< @sa tagUserGameInfo

///房间定时消息结构
typedef struct tagRoomMsg
{
	DWORD dwID;					//!<消息ID
	DWORD dwObjectType;			//!<对象类型
	DWORD dwObjectID;			//!<对象ID
	DWORD dwTimeinterval;		//!<时间间隔
	DWORD dwSendCount;			//!<发送次数
	WCHAR szBuff[ROOM_MSG_LEN];	//!<消息内容
}ROOMMSG,*LPROOMMSG;	//!<@sa tagRoomMsg

///用户游戏数据
typedef struct tagUserData
{
	USERINFO		userInfo;		//!<用户基本数据
	USERGAMEINFO	userGameInfo;	//!<用户特定游戏数据
}USERDATA,*LPUSERDATA; //!< @sa tagUserData

/**
* @brief 系统消息 
*/
typedef struct tagSystemMsg
{
	DWORD		dwMsgType;		//!<消息类型
	SYSTEMTIME	tmCreate;	//!<创建时间
	TCHAR		szNickFrom[USER_NAME_LEN];//!<发出用户昵称
	TCHAR		szNickTo[USER_NAME_LEN];//!<接受用户昵称
	TCHAR		szMsg[ROOM_MSG_LEN]; //!<消息内容
}SYSTEMMSG,*LPSYSTEMMSG;	//!< @sa tagSystemMsg

/**
* @brief 任务信息
*/
typedef struct tagMissionInfo
{
	LONGLONG	llAwardValue;				//!<奖励数量
	LONGLONG	llDataValue;				//!<期待数据值,期待的数据值，与dwDataType指定的值进行比较的右值

	DWORD		dwID;						//!<任务ID
	char16_t	szName[USER_NAME_LEN];		//!<任务名 
	char16_t	szContext[MISSION_CONTEXT_LEN];//!<任务描述
	DWORD		dwType;						//!<任务类型,为@ref missionType 中的一种
	DWORD		dwRepeatCount;				//!<任务重复次数,为0XFFFFffff则无限次
	DWORD		dwPreMission;				//!<前缀任务,前缀任务ID，必须完成了该任务才能提交当前任务
	DWORD		dwAwardType;				//!<奖励物品类型，为@ref propIDType 中的一种
	DWORD		dwDataType;					//!<比较数据类型，为为@ref propIDType 中的一种，如果为0XffffFFFF,则dwDataIndex为日常数据或者成就数据索引		
	DWORD		dwDataIndex;				//!<当dwDataType为0xFFFFffff时有效,否则必须为0。数据索引，如果为成就任务，则为@ref effortDataType 的一种,如果为日常任务，则为@ref dailyDataType 的一种，为0XFFFFffff则自定义处理

	DWORD		dwDataCompType;				//!<数据比较函数，为@ref dataCompareFun 值的一个
}MISSIONINFO,*LPMISSIONINFO;	//!< @sa tagMissionInfo

///自由落座房间用户设置
typedef struct tagUserSetting
{
	int		nScoreMinLimit;					//!<分数最小限制
	int		nScoreMaxLimit;					//!<分数最大限制

	DWORD   dwMoneyMinLimit;				//!<金钱最少限制
	DWORD	dwPingLimit;					//!<网络延迟限制，单位：毫秒
	DWORD	dwBaseMoney;					//!<房间底注
	BYTE	byFleeRateLimit;				//!<逃跑率限制 0 到100
	BYTE	byAllowLookOn;					//!<是否允许旁观
	BYTE	byAllowSameIP;					//!<是否允许相同IP
	//BYTE	byNoAbhorPlayer;				//!<不跟厌恶玩家一起
	char16_t szDeskPassword[PASS_WORD_LEN];	//!<桌子密码

}USERSETTING,*LPUSERSETTING; //!<@sa tagUserSetting

///成就数据索引
enum effortDataType
{
	EFFORT_DATA_LOGIN_COUNT	,	//!<登陆次数
	EFFORT_DATA_ONLINE_TIME,	 //!<在线时长
	EFFORT_DATA_GAME_COUNT,	 //!<游戏局数
	EFFORT_DATA_GAME_TIME,	 //!<游戏时长
	EFFORT_DATA_GIFT_COUNT,	 //!<总领奖次数
	EFFORT_DATA_WIN_MONEY,	 //!<总赢的钱数
	EFFORT_DATA_LOST_MONEY,	 //!<总输的钱数

	
	EFFORT_DATA_REPORT,	//!<最后签到日期

	EFFORT_DATA_PAY_MONEY,	 //!<充值(卡面值)总数

	EFFORT_DATA_LOTTERY,	 //!<获得的总元宝数
	EFFORT_DATA_MISSION_LOTTERY,//!<任务获得的总元宝数
	EFFORT_DATA_SET_SECOND_PASSWORD,//!<是否第一次设置二级密码

	EFFORT_DATA_WIN_COUNT,	 //!<赢的总局数
	FORT_DATA_LOST_COUNT,	 //!<输的总局数
	///必须最后一行
	EFFORT_COUNT,
};

/**
@brief 日常数据索引
*/
enum dailyDataType
{
	DAILY_DATA_LOGIN_COUNT,	 //!<登陆次数
	DAILY_DATA_ONLINE_TIME,	 //!<在线时长
	DAILY_DATA_GAME_COUNT,	 //!<游戏局数
	DAILY_DATA_GAME_TIME,	 //!<游戏时长
	DAILY_DATA_GIFT_COUNT,	 //!<领奖次数 
	DAILY_DATA_WIN_MONEY,	 //!<赢的钱数
	DAILY_DATA_LOST_MONEY,	 //!<输的钱数
	
	DAILY_DATA_REPORT,	 //!<签到

	DAILY_DATA_PAY_MONEY,	 //!<充值（卡面值）总数
	DAILY_DATA_LOTTERY,	//!<当天获得的元宝数
	DAILY_DATA_MISSION_LOTTERY,	//!<当天任务获得的元宝数

	DAILY_DATA_EXCHANGE_LIMIT,	//!<兑换限制,第一位，300元宝兑换6W开心豆，2位,30元宝兑换2参赛券
	DAILY_DATA_GAME_COUNT_ROOM_LEVEL0,//新手场玩的局数
	DAILY_DATA_GAME_COUNT_ROOM_LEVLE1,//初级房间玩的局数
	DAILY_DATA_GAME_COUNT_ROOM_LEVEL2,//中级房间玩的局数
	DAILY_DATA_ACTIVE_CARD_USED,	 //是否使用激活卡
	DAILY_DATA_WIN_COUNT,	 //赢的局数
	DAILY_DATA_LOST_COUNT,
	///必须最后一行
	DAILY_COUNT,
};

/**
* @brief 商店物品信息 
*/
typedef struct tagShopGoodsInfo
{
	LONGLONG	llPrice;			//!<物品价格
	LONGLONG	llCount;			//!<物品数量
	DWORD		dwID;				//!<物品ID
	DWORD		dwMoneyType;		//!<支付类型（使用哪种货币？道具？）
	DWORD		dwGoodType;			//!<物品类型
	WCHAR		szGoodsName[SHOP_GOODS_NAME];//!<物品名
	WCHAR		szGoodsDescription[SHOP_GOODS_DESCRIPTION];//!<物品描述
	DWORD		dwClientType;		//!<客户端支持类型
		
}SHOPGOODSINFO,*LPSHOPGOODSINFO;//!<@sa tagShopGoodsInfo

/**
* @brief 游戏关卡数据
* @deprecated 游戏关卡暂时已经停用
*/
struct tagGameMission
{
	DWORD	dwMissionID;	//!<关卡ID(已废弃)
	DWORD	dwDataID;		//!<需要数据ID(由游戏开发者决定)
	DWORD	dwDataValue;	//!<需要数据值
	DWORD	dwAwardID;		//!<奖励道具ID
	DWORD	dwAwardValue;	//!<奖励值
	char16_t	szDescript[MISSION_CONTEXT_LEN];//!<任务描述
};


#pragma pack(pop)
/**
* @brief 重载GUID的小于运算符.
* map和hash_map的key必须支持小于运算符，为了能让GUID能做为key，所以重写该运算符
* @param [in] lhs 参与比较时小于号左边的GUID值
* @param [in] rhs 参与比较时小于号右边的GUID值
* @retval lhs与rhs内存ASCII比较，如果lhs小于rhs，返回true,否则返回false
*/
inline bool operator <(const GUID& lhs,const GUID& rhs)
{
	return memcmp(&lhs,&rhs,sizeof(GUID))<0;
}

///网络消息处理接口
struct IMessageProcessor
{
public:
	/**
	* @brief 消息处理
	* @param [in] lpHeader 数据包指针
	* @return 成功处理消息应当返回true
	*/
	virtual bool OnProcessMessage( msg_Header* lpHeader) = 0;
	
	/**
	* @brief 连接到服务器异步结果
	* @param [in] bConnect 是否成功连接到服务器
	*/
	virtual void OnConnect( bool bConnect) = 0;
	/**
	* @brief 与服务器连接断开
	* @param [in] nErrorCode 错误码
	*/
	virtual void OnSocketClose( int nErrorCode) = 0;
};

///注册网络消息处理
#define REG_MSG_PROCESSOR(msgId,fn) RegisterMsgProcess(static_cast<WORD>(msgId),(FNMSGPROCESSOR)(fn))

///无效的桌子ID
#define INVALID_DESK 0XFFff
///无效的椅子ID
#define INVALID_CHAIR 0xffFF

class CBaseRoomManager;
class CBaseGameClient;
/**
* @brief 创建游戏场景函数原型 
*/
typedef CBaseGameClient* (*FNCreateGameClient)(lua_State*, CBaseRoomManager*);

#include "Common/MD5_.h"
#include "Common/Des.h"
#include "Common/sha1.h"

#include "Common/Helper.h"
#include "Common/SerialStream.h"
#include "Common/MsgPacket.h"

extern "C"
{
#include "lualib.h"
#include "lauxlib.h"
};

#include "Common/luaobj/LuaObject.h"
#include "Common/luaobj/BaseUserData.h"

#include "Common/BaseConnectManager.h"

//#include "DelayDelete.h"

#include "Common/BasePlayer.h"
#include "Common/BasePlayerData.h"
#include "Common/BaseGameClient.h"

//#include "GameHall/GameHallApp.h"




