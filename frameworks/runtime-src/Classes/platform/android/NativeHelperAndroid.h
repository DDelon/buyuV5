#ifndef _DEVICE_API_H_
#define _DEVICE_API_H_
#include <map>
#include <string>

typedef std::map<std::string, std::string> StringMap;

class NativeHelperAndroid
{
	NativeHelperAndroid(void);
	~NativeHelperAndroid(void);
public:
	static void onApplicationDidFinishLaunching();
	static void onApplicationDidEnterBackground();
	static void onApplicationWillEnterForeground();

	static const std::string getExternalDirectoryJNI();
	/**
	* 发送图片到微信朋友圈
	*/
	static void shareToWechatJNI(StringMap *params);

	static bool startRecodeJNI(const char*);

	static  void stopRecodeJNI();
	static  int getBatteryLevel();
	/**
	* 显示Toast
	*/
	static void showToastJNI(const std::string &message);
	static void hideLoadingJNI();
	static void vibrateJNI(int millisecond);
	/**
	* 设置屏幕方向  0 横屏 ，1 竖屏
	*/
	static void setScreenOrientationJNI(int orientation);
	static bool getStringJNI( std::string& refString, const char* className);
	static bool getDeviceNameJNI( std::string& deviceName);
	static bool getDeviceCodeJNI( std::string& deviceCode);
	static StringW gbkToUtf16JNI(const StringA& strGbk);
	static StringA utf16ToGbkJNI(const StringW& strUtf16);
	static void doPayJNI(const std::string &url, int payType, StringMap *params);

};

#endif /* _DEVICE_API_H_ */

