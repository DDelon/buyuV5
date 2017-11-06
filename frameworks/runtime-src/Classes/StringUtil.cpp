#include "StringUtil.h"

std::string StringUtil::format(const char *formatStr, ...)
{
#define CC_MAX_STRING_LENGTH (1024*100)

    std::string ret;

    va_list ap;
    va_start(ap, formatStr);

    char* buf = (char*)malloc(CC_MAX_STRING_LENGTH);

    if(buf != nullptr)
    {
        vsnprintf(buf, CC_MAX_STRING_LENGTH, formatStr, ap);
        ret = buf;
        free(buf);
    }

    va_end(ap);

    return ret;
}

std::string StringUtil::getStringXmlEx(const char* fileName,const char* key, ...)
{

	string formatString = StringUtil::getStringXml(fileName, key);

    const char* formatStr = formatString.c_str();
#define CC_MAX_STRING_LENGTH (1024*100)

    std::string ret;

    va_list ap;
    va_start(ap, key);

    char* buf = (char*)malloc(CC_MAX_STRING_LENGTH);

    if(buf != nullptr)
    {
        vsnprintf(buf, CC_MAX_STRING_LENGTH, formatStr, ap);
        ret = buf;
        free(buf);
    }

    va_end(ap);

    return ret;


}

std::string StringUtil::getStringXml(const char* fileName,const char* key)
{
	static ValueMap valueMap = FileUtils::getInstance()->getValueMapFromFile(fileName);
    if(valueMap[key].isNull())
    {
        return "not find the key:" + string(key);
    }

    return valueMap[key].asString();
}

std::string StringUtil::getcurrTime()
{
    struct tm *tm;
    time_t timep;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    time(&timep);
#else
    struct timeval tv;
    gettimeofday(&tv, NULL);
    timep = tv.tv_sec;
#endif

    tm = localtime(&timep);

    return StringUtils::format("share_%d_%d_%d__%d_%d_%d.png", tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec);
}



void StringUtil::_STR_SPLIT(const string& src, const string& separator, vector<string>& dest, bool isNull)
{
	string::size_type start = 0, index;
	do
	{
		index = src.find_first_of(separator, start);
		if (index != string::npos)
		{
			std::string substring(src.substr(start, index - start));
			start = index + 1;
			if (isNull&&substring.empty() && substring.length() == 0)
				continue;
			dest.push_back(substring);
		}
	} while (index != string::npos);

	//the last token
	std::string substring(src.substr(start));
	if (isNull&&substring.empty() && substring.length() == 0)
		return;
	dest.push_back(substring);
}
//×Ö·û´®±È½Ï
bool StringUtil::_STR_EQUAL(const std::string& s1, const std::string& s2)
{
	return strcmp(s1.c_str(), s2.c_str()) == 0;
}

std::string StringUtil::getServerIP()
{
	return getStringXml("ipConfig.plist","ServerIP");
}

std::string StringUtil::convertToScoreStr(long long score)
{
	std::string str;
	if (score < 10000)
	{
		str = StringUtil::format("%d",score);
	}
	else
	{
		str = StringUtil::format("%.2fw", score / 10000.0f);
	}
	return str;
}

std::wstring StringUtil::stringToWstring(std::string str)
{
	/*int size = MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);
	wchar_t *ch = new wchar_t[size + 1];
	MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, ch, size);*/

	return wstring();
}
