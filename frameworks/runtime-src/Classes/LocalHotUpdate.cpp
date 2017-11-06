
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
		// 3. ���ԼӸ�ʱ���жϣ���ֹˢ��Ƶ��
		auto engine = LuaEngine::getInstance();
		LuaStack* luaStack = engine->getLuaStack();
		// 4. �޸�·��Ϊ����·��"require \"C:\\xxxx\\main.lua\\" "
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

		//�ı�֪ͨ�Ѿ�������ɣ�����ֻ��ȴ���Щ֪ͨ��������Ȼ������Ӧ����  
		while (TRUE)
		{
			DWORD dwWaitStatus = WaitForMultipleObjects(1, dwChangeHandles, FALSE, INFINITE);
			switch (dwWaitStatus)
			{
			case WAIT_OBJECT_0:
				//ִ��ĳ��Ӧ����  
				s_reloadGame();
				if (FindNextChangeNotification(dwChangeHandles[0]) == FALSE)
					ExitProcess(GetLastError());
				break;
			default:
				ExitProcess(GetLastError());
			}
		}
	}

	// docNames�����·�� 
	void monitorDocumentNames(const std::vector<std::string>& docNames)
	{
		// 1. ��������·��
		// 2. ��ӱ���·��Ϊ����·�����ȼ����FileUtils::getInstance()->addSearchPath("������·��һ��", true);
		LPTSTR lpDir = L"C:\\Users\\46791\\Desktop\\MyLuaGame\\src";
		CreateThread(NULL, 0, WatchDirectory, lpDir, 0, NULL);
	}
}
