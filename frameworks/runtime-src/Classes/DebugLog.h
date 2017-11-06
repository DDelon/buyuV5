#ifndef __DEBUG_LOG_H__
#define __DEBUG_LOG_H__
#include "cocos2d.h"
USING_NS_CC;

#define WRITE_LOG

class DebugLog
{
public:
	DebugLog();
	~DebugLog();

	static DebugLog *getInstance();

	void init();

	void setWritePath(std::string path);

	void print(std::string theme, std::string slog);

	void log(std::string slog);

	void startCount();

	void setOpen(bool isOpen) { _isOpen = isOpen; }

	void endCount(std::string str = "", int minValue = 0);

private:
	static DebugLog *_pInstance;
	static int _count;

	clock_t _startTime;
	clock_t _endTime;
	std::string _sWritePath;
	FILE *_pFile;

	bool _isOpen;

	

};

#endif