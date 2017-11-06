#ifndef __StringUtil__
#define __StringUtil__

#include <string>
#include "cocos2d.h"


using namespace std;
using namespace cocos2d;

class StringUtil
{
public:

    ~StringUtil() {};

	static std::string getStringXml(const char* fileName, const char* key);
    static std::string format(const char* formatStr, ...);
	static std::string getStringXmlEx(const char* fileName,const char* key, ...);
    static std::string getcurrTime();

	static bool _STR_EQUAL(const std::string& s1, const std::string& s2);
	static void _STR_SPLIT(const string& src, const string& separator, vector<string>& dest, bool isNull = false);
	static std::string getServerIP();
	static std::string convertToScoreStr(long long score);
	static std::wstring stringToWstring(std::string str);
private:
    StringUtil() {};
};

#define IntToString(__var)  StringUtil::format("%d",__var)
#endif /* StringUtil_h */
