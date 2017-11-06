#include "DebugLog.h"
#include "cocos2d.h"
#include "AppDelegate.h"
USING_NS_CC;

int DebugLog::_count = 0;

DebugLog::DebugLog()
{
}


DebugLog::~DebugLog()
{
}

DebugLog *DebugLog::_pInstance = nullptr;
DebugLog *DebugLog::getInstance()
{
	if (_pInstance == nullptr)
	{
		_pInstance = new DebugLog();
		_pInstance->init();
	}

	return _pInstance;
}

void DebugLog::init()
{
	_isOpen = false;
	
}

void DebugLog::setWritePath(std::string path)
{
	_sWritePath = path;
	_pFile = fopen(_sWritePath.c_str(), "w");
}

void DebugLog::print(std::string theme, std::string sLog)
{
	if (_isOpen)
	{

		if (!_pFile) {
			return;
		}
		std::string sContent;
		sContent = "[" + Value(_count++).asString() + "]" + "." + theme;

		sContent += " :";
		sContent += sLog;
		sContent += "\n";



#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
		struct timeval now;
		struct tm* time;

		gettimeofday(&now, NULL);


		time = localtime(&now.tv_sec);
		//int year = time->tm_year + 1900;
		//log("year = %d", year);

		char date[128] = { 0 };
		sprintf(date, "%d-%02d-%02d %02d:%02d:%02d", (int)time->tm_year + 1900, (int)time->tm_mon + 1, (int)time->tm_mday, (int)time->tm_hour, (int)time->tm_min, (int)time->tm_sec);
		fprintf(_pFile, "%s %s", date, sContent.c_str());
#elif CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
		SYSTEMTIME wtm;
		GetLocalTime(&wtm);



		//int year = time->tm_year + 1900;
		//log("year = %d", year);

		char date[128] = { 0 };
		sprintf(date, "%d-%02d-%02d %02d:%02d:%02d", wtm.wYear, wtm.wMonth, wtm.wDay, wtm.wHour, wtm.wMinute, wtm.wSecond);
		fprintf(_pFile, "%s %s", date, sContent.c_str());
#endif
		fflush(_pFile);
	}



	


}

void DebugLog::log(std::string slog)
{
	this->print("log", slog);
	//CCLOG("%s", slog.c_str());
	cocos2d::log("lua:%s", slog.c_str());
}

void DebugLog::startCount()
{
	_startTime = clock();
}

void DebugLog::endCount(std::string str, int minValue)
{
	_endTime = clock();
	if ((_endTime - _startTime) / 1000 > minValue)
	{
		log(str+"running time " + Value((double)(_endTime - _startTime) / 1000).asString());
	}
	
}
