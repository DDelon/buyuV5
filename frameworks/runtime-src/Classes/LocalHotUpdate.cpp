
#include <iostream>

#include "LocalHotUpdate.h"
#include "cocos2d.h"
#include "runtime/ConfigParser.h"
#include "scripting/lua-bindings/manual/CCLuaEngine.h"

using namespace cocos2d;
namespace local_hot_update
{
	static void s_reloadGame()
	{
		// 3. 可以加个时间判断，防止刷新频繁
		auto engine = LuaEngine::getInstance();
		LuaStack* luaStack = engine->getLuaStack();
		// 4. 修改路径为本地路径"require \"C:\\xxxx\\main.lua\\" "
		luaStack->executeString("require \"main.lua\" ");
	}

	DWORD WINAPI WatchDirectory(LPVOID lp)
	{
		LPTSTR lpDir = (LPTSTR)lp;
		HANDLE dwChangeHandles[1];  
		dwChangeHandles[0] = FindFirstChangeNotification(
			lpDir,
			TRUE,
			FILE_NOTIFY_CHANGE_FILE_NAME |
			FILE_NOTIFY_CHANGE_SIZE |
			FILE_NOTIFY_CHANGE_ATTRIBUTES);
		if (dwChangeHandles[0] == INVALID_HANDLE_VALUE)
			ExitProcess(GetLastError());

		//改变通知已经设置完成，现在只需等待这些通知被触发，然后做相应处理  
		while (TRUE)
		{
			DWORD dwWaitStatus = WaitForMultipleObjects(1, dwChangeHandles, FALSE, INFINITE);
			switch (dwWaitStatus)
			{
			case WAIT_OBJECT_0:
				//执行某对应操作  
				s_reloadGame();
				if (FindNextChangeNotification(dwChangeHandles[0]) == FALSE)
					ExitProcess(GetLastError());
				break;
			default:
				ExitProcess(GetLastError());
			}
		}
	}

	// docNames：相对路径 
	void monitorDocumentNames(const std::vector<std::string>& docNames)
	{
		// 1. 监听本地路径
		// 2. 添加本地路径为搜索路径优先级最高FileUtils::getInstance()->addSearchPath("与上面路径一样", true);
		LPTSTR lpDir = L"C:\\Users\\46791\\Desktop\\MyLuaGame\\src";
		CreateThread(NULL, 0, WatchDirectory, lpDir, 0, NULL);
	}
}
