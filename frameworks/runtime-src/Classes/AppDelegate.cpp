#include "../Classes/CommonData.h"
#include "AppDelegate.h"
#include "scripting/lua-bindings/manual/CCLuaEngine.h"
#include "audio/include/SimpleAudioEngine.h"
#include "scripting/lua-bindings/manual/lua_module_register.h"
#include "auto/lua_LuaCppAdapter_auto.hpp"
#include "LuaCppAdapter.h"
#include "ParserConfig/ParseGameData.h"
#include "ljmsg.h"
#include "StringUtil.h"
#include "FishFrameManager.h"
#include <time.h>
#include "Common/ClientSocket.h"
#include "DebugLog.h"


#if (CC_TARGET_PLATFORM != CC_PLATFORM_LINUX)
#include "ide-support/CodeIDESupport.h"
#endif
#if (CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
//#include "platform/android/NativeApiAndroid.h"
#elif(CC_TARGET_PLATFORM==CC_PLATFORM_IOS)

#endif

#if (COCOS2D_DEBUG > 0) && (CC_CODE_IDE_DEBUG_SUPPORT > 0)
#include "runtime/Runtime.h"
#include "ide-support/RuntimeLuaImpl.h"
#endif

#define CC_SHOW_FPS
//#define LUA_DEBUG

/*const int LUA_DEBUG = 1;*/

#if LUA_DEBUG > 0
#ifndef WIN32
#include <dlfcn.h>
#include <errno.h>
#include <sys/stat.h>
#define HMODULE void*
#define GetProcAddress dlsym
#elif CC_TARGET_PLATFORM==CC_PLATFORM_IOS || CC_TARGET_PLATFORM==CC_PLATFORM_MAC
#else
#include <io.h>
#define access _access
#endif
HMODULE g_hLuaDebug = NULL;
#endif

using namespace CocosDenshion;

USING_NS_CC;
using namespace std;

#if LUA_DEBUG > 0
extern "C" bool GetFileData(const char* pfilename, void** ppData, int* nLen)
{
	std::string filename(pfilename);
	Data data;
	if (cocos2d::FileUtils::Status::OK != FileUtils::getInstance()->getContents(pfilename, &data))
	{
		//std::string filename(pfilename);
		size_t pos = filename.rfind(".wld");
		if (pos != std::string::npos)
			filename = filename.substr(0, pos);
		else
		{
			pos = filename.rfind(".lua");
			if (pos != std::string::npos)
				filename = filename.substr(0, pos);
		}

		pos = filename.find_first_of(".");
		while (pos != std::string::npos)
		{
			filename.replace(pos, 1, "/");
			pos = filename.find_first_of(".");
		}

		//search file in package.path
		std::string chunkName = "";
		auto utils = FileUtils::getInstance();

		CLuaThread L;
		lua_getglobal(L, "package");
		lua_getfield(L, -1, "path");
		std::string searchpath(lua_tostring(L, -1));
		lua_pop(L, 2);
		size_t begin = 0;
		size_t next = searchpath.find_first_of(";", 0);
		do
		{
			if (next == std::string::npos)
				next = searchpath.length();

			std::string prefix = searchpath.substr(begin, next);
			if (prefix[0] == '.' && prefix[1] == '/')
			{
				prefix = prefix.substr(2);
			}

			pos = prefix.find("?.lua");
			if (pos != std::string::npos)
			{
				StringA strPathName = prefix.substr(0, pos);
				chunkName = strPathName + filename + ".wld";
				if (FileUtils::Status::OK == utils->getContents(chunkName, &data))
				{
					filename = chunkName;
					break;
				}
				else
				{
					chunkName = strPathName + filename + ".lua";
					if (FileUtils::Status::OK == utils->getContents(chunkName, &data))
					{
						filename = chunkName;
						break;
					}
				}
			}

			begin = next + 1;
			next = searchpath.find_first_of(";", begin);
		} while (begin < (size_t)searchpath.length());
	}

	if (data.getBytes())
	{
		*nLen = static_cast<int>(data.getSize());
		*ppData = malloc(*nLen+filename.length()+1);
		if (!*ppData)
			return false;

		memcpy(*ppData, data.getBytes(), *nLen);
		memcpy(reinterpret_cast<char*>(*ppData) + *nLen, filename.c_str(), filename.length() + 1);
		return true;
	}
	else
		return false;
}

extern "C" void FreeFileData(void* pData)
{
	if (pData)
		free(pData);
}
typedef bool(*FNGETFILEDATA)(const char*, void**, int*);
typedef void(*FNFREEFILEDATA)(void*);
typedef void(*FNSETFILESYSTEMCALLBACK)(FNGETFILEDATA, FNFREEFILEDATA);
#if CC_TARGET_PLATFORM==CC_PLATFORM_IOS || CC_TARGET_PLATFORM==CC_PLATFORM_MAC
extern "C" {
    void LuaDebugUninit();
    bool LuaDebugInit(WORD);
    bool RegisterScript(const char* ,lua_State* );
    void SetFileSystemCallBack(FNGETFILEDATA, FNFREEFILEDATA);
}
#endif
#endif

///////////////////////////////////load game data//////////////////////////////
void loadGameData()
{
	
}


AppDelegate::AppDelegate()
{
}

AppDelegate::~AppDelegate()
{
    SimpleAudioEngine::end();
#if LUA_DEBUG > 0
#if (CC_TARGET_PLATFORM != CC_PLATFORM_IOS && CC_TARGET_PLATFORM!=CC_PLATFORM_MAC)

	if (g_hLuaDebug)
	{
		typedef void(*FnUnit)();
		FnUnit fn = reinterpret_cast<FnUnit>(GetProcAddress(g_hLuaDebug, "LuaDebugUninit"));
		if (fn)
			fn();

#ifdef WIN32
		FreeLibrary(g_hLuaDebug);
#else
		dlclose(g_hLuaDebug);
#endif
		g_hLuaDebug = NULL;
	}
#else
	    LuaDebugUninit();
#endif
#endif
#if (COCOS2D_DEBUG > 0) && (CC_CODE_IDE_DEBUG_SUPPORT > 0)
    // NOTE:Please don't remove this call if you want to debug with Cocos Code IDE
    RuntimeEngine::getInstance()->end();
#endif

}

static bool g_bInit = false;

extern "C"
{
	bool _IsAppInit(void)
	{
		return g_bInit;
	}
}
// if you want a different context, modify the value of glContextAttrs
// it will affect all platforms
void AppDelegate::initGLContextAttrs()
{
    // set OpenGL context attributes: red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

// if you want to use the package manager to install more packages, 
// don't modify or remove this function
static int register_all_packages()
{
    return 0; //flag for packages manager
}



bool AppDelegate::applicationDidFinishLaunching()
{


	
    // set default FPS
    Director::getInstance()->setAnimationInterval(1.0f / 60.0f);
    // register lua module
    auto engine = LuaEngine::getInstance();
    ScriptEngineManager::getInstance()->setScriptEngine(engine);
    lua_State* L = engine->getLuaStack()->getLuaState();
	engine->getLuaStack()->addLuaLoader(&CLuaObject::L_Loader);

	register_all_LuaCppAdapter(L);

    lua_module_register(L);
	JMsgRegisterLua(L);
    register_all_packages();


	
	LuaStack* stack = engine->getLuaStack();
	stack->setXXTEAKeyAndSign("weilebuyu577588599", strlen("weilebuyu577588599"), "weilebuyu577588599", strlen("weilebuyu577588599"));
	


	//添加文件搜索路径 热更新
	std::string searchPath = FileUtils::getInstance()->getWritablePath();
	std::string srcPath = searchPath + "src/";
	std::string resPath = searchPath + "res/";
	std::string platSearchPath = resPath + "platform/";
	FileUtils::getInstance()->addSearchPath(srcPath);
	FileUtils::getInstance()->addSearchPath(resPath);
	FileUtils::getInstance()->addSearchPath(platSearchPath);
	std::string globalConfigPath = "";
	std::string binConfigPath = "";
	bool isExistGlobal = FileUtils::getInstance()->isFileExist(searchPath + "src/globalconfig.lua");
	if (isExistGlobal)
	{
		globalConfigPath = searchPath + "src/globalconfig.lua";
	}
	else
	{
		globalConfigPath = "src/globalconfig.lua";
	}
	log("-----------------------------8888");
    //download::lua_open_download(L);
// #if (CC_TARGET_PLATFORM!=CC_PLATFORM_IOS && CC_TARGET_PLATFORM!=CC_PLATFORM_MAC)
// 	
// #endif
	CLuaObject::Register(L);
	CBaseUserData::Register(L);
	CLuaObject::DoFile(L, "src/globalconfig.lua");
#if LUA_DEBUG > 0
#if (CC_TARGET_PLATFORM!=CC_PLATFORM_IOS && CC_TARGET_PLATFORM!=CC_PLATFORM_MAC)
#ifdef WIN32
	g_hLuaDebug = LoadLibraryA("LuaDebug.dll");

#else
	std::string strLibPath = "/data/data/com.weile.gamehall/lib/libluadebug.so";//CCFileUtils::sharedFileUtils()->getWritablePath()+("lib/libweilegamehall.so");
	g_hLuaDebug = dlopen(strLibPath.c_str(), RTLD_LAZY);
#endif
	if (g_hLuaDebug)
	{
		WriteLog(LOG_INFO, _UTF8("加载lua调试库成功!"));
		typedef bool(*FnInit)(unsigned short);

		FnInit fnInit = reinterpret_cast<FnInit>(::GetProcAddress(g_hLuaDebug, "LuaDebugInit"));
		if (fnInit&& fnInit(7342))
		{
			typedef bool(*FnReg)(const char*, lua_State*);
			FnReg fn = reinterpret_cast<FnReg>(::GetProcAddress(g_hLuaDebug, "RegisterScript"));
			if (fn)
			{
				fn("main", stack->getLuaState());
			}
			 

			FNSETFILESYSTEMCALLBACK fnSet = reinterpret_cast<FNSETFILESYSTEMCALLBACK>(::GetProcAddress(g_hLuaDebug, "SetFileSystemCallBack"));
			if (fnSet)
				fnSet(&GetFileData, &FreeFileData);
		}
	}
	else
	{
		WriteLog(LOG_DEBUG, _UTF8("加载LUA调试库失败，errno=%d,err=%s"), errno, strerror(errno));
	}
	
	
#else

	    if(LuaDebugInit(7342))
        {
            RegisterScript("main", stack->getLuaState());
            SetFileSystemCallBack(&GetFileData, &FreeFileData);
        }
#endif
#endif

#if (COCOS2D_DEBUG > 0) && (CC_CODE_IDE_DEBUG_SUPPORT > 0)
    // NOTE:Please don't remove this call if you want to debug with Cocos Code IDE
    auto runtimeEngine = RuntimeEngine::getInstance();
    runtimeEngine->addRuntime(RuntimeLuaImpl::create(), kRuntimeEngineLua);
    runtimeEngine->start();
#else



// 	if (engine->executeScriptFile("src/hallmain.lua"))
// 	{
// 		return false;
// 	}

	g_bInit = true;
	if (!CLuaObject::DoFile(L,"src/main.lua"))
    {
        return false;
    }

	/*lua_State*  L1 = LuaEngine::defaultEngine()->getLuaStack()->getLuaState();
	lua_getfield(L1, LUA_GLOBALSINDEX, ("FishGF"));
	lua_pushstring(L1, "setLodingEnd");
	lua_gettable(L1, -2);
	lua_pcall(L1, 0, 0, 0);*/

	

#endif
#if (CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
//NativeApiAndroid::onApplicationDidFinishLaunching();
#elif(CC_TARGET_PLATFORM==CC_PLATFORM_IOS)

#endif

    return true;
}

void AppDelegate::loadFishConfig()
{
	//LuaCallCpp::getInstance()->loadFishConfig();
}

void AppDelegate::setSocketPause(bool isPause)
{
    CClientSocket::setPauseState(isPause);
}

// This function will be called when the app is inactive. Note, when receiving a phone call it is invoked.
void AppDelegate::applicationDidEnterBackground()
{
	CClientSocket::setPauseState(true);
    Director::getInstance()->stopAnimation();
 
    SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
#if (CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
	//NativeApiAndroid::onApplicationDidEnterBackground();
#elif(CC_TARGET_PLATFORM==CC_PLATFORM_IOS)

#endif

	auto eventDispatcher = cocos2d::Director::getInstance()->getEventDispatcher();
	eventDispatcher->dispatchCustomEvent("applicationDidEnterBackground", nullptr);
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground()
{
	CClientSocket::setPauseState(false);
    Director::getInstance()->startAnimation();

    //SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
#if (CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
	//NativeApiAndroid::onApplicationWillEnterForeground();
#elif(CC_TARGET_PLATFORM==CC_PLATFORM_IOS)

#endif
	auto eventDispatcher = cocos2d::Director::getInstance()->getEventDispatcher();
	eventDispatcher->dispatchCustomEvent("applicationWillEnterForeground", nullptr);
}

void AppDelegate::createFile()
{



#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	struct timeval now;
	struct tm* time;

	gettimeofday(&now, NULL);


	time = localtime(&now.tv_sec);
	//int year = time->tm_year + 1900;
	//log("year = %d", year);

	char date[64] = { 0 };
	sprintf(date, "%d%02d%02d%02d%02d%02d", (int)time->tm_year + 1900, (int)time->tm_mon + 1, (int)time->tm_mday, (int)time->tm_hour, (int)time->tm_min, (int)time->tm_sec);
	//log("%s", date);
	std::string path = "/sdcard/clientLog";
	path.append(date);
	path += ".txt";
	JMsgSetLogPath(path);
#endif

#if  CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	auto getExePath = [=]()->std::string{
		char path[4096] = {};
		GetModuleFileNameA(NULL, path, sizeof(path));
		char* pos = strrchr(path, '/');

		if (!pos) {
			pos = strrchr(path, '\\');
		}

		if (pos) {
			*pos = '\0';
		}

		while (true) {
			pos = strrchr(path, '\\');

			if (pos) {
				*pos = '/';
			}
			else {
				break;
			}
		}
		return path;
	};
	time_t now;
	struct tm* time;

	SYSTEMTIME wtm;
	GetLocalTime(&wtm);

	 

	//int year = time->tm_year + 1900;
	//log("year = %d", year);

	char date[128] = { 0 };
	sprintf(date, "%u%02d%02d%02d%02d%02d", wtm.wYear, wtm.wMonth, wtm.wDay, wtm.wHour, wtm.wMinute, wtm.wSecond);
	std::string path = getExePath() + "clientLog";
	path.append(date);
	path += ".txt";
	JMsgSetLogPath(path);
#endif

	DebugLog::getInstance()->setOpen(true);
}

 
