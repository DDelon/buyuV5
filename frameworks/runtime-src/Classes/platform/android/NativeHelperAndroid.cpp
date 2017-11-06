#include "../Classes/CommonData.h"
#include "NativeHelperAndroid.h"
#include "platform/android/jni/JniHelper.h"

#include "ide-support/SimpleConfigParser.h"
#include "ide-support/CodeIDESupport.h"
#include "cocos2d.h"

#define JAVA_CLASS_NAME "com.weile.api.NativeHelper"

#define  LOG_TAG    "GAME_JNI"
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

jobject createJavaMapObject(JNIEnv* env, StringMap* paramMap) {
	jclass class_HashMap = env->FindClass("java/util/HashMap");
	jmethodID construct_method = env->GetMethodID(class_HashMap, "<init>",
			"()V");
	jobject obj_Map = env->NewObject(class_HashMap, construct_method, "");
	if (paramMap != NULL) {
		jmethodID add_method = env->GetMethodID(class_HashMap, "put",
				"(Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;");
		for (std::map<std::string, std::string>::const_iterator it =
				paramMap->begin(); it != paramMap->end(); ++it) {
			env->CallObjectMethod(obj_Map, add_method,
					env->NewStringUTF(it->first.c_str()),
					env->NewStringUTF(it->second.c_str()));
		}
	}
	env->DeleteLocalRef(class_HashMap);
	return obj_Map;
}

void createNativeMap(JNIEnv *env, jobject javaMap, StringMap *nativeMap) {
	JniMethodInfo m;
	if (!JniHelper::getMethodInfo(m, "java/util/HashMap", "entrySet",
			"()Ljava/util/Set;")) {
		LOGE("Get 'entrySet' method of 'HashMap' failed!");
		return;
	}
	jobject entrySet = env->CallObjectMethod(javaMap, m.methodID);

	if (!JniHelper::getMethodInfo(m, "java/util/Set", "iterator",
			"()Ljava/util/Iterator;")) {
		LOGE("Get 'iterator' method of 'Set' failed!");
		return;
	}
	jobject itr = env->CallObjectMethod(entrySet, m.methodID);

	JniMethodInfo m_hasNext;
	if (!JniHelper::getMethodInfo(m_hasNext, "java/util/Iterator", "hasNext",
			"()Z")) {
		LOGE("Get 'hasNext' method of 'Iterator' failed!");
		return;
	}

	JniMethodInfo m_next;
	if (!JniHelper::getMethodInfo(m_next, "java/util/Iterator", "next",
			"()Ljava/lang/Object;")) {
		LOGE("Get 'next' method of 'Iterator' failed!");
		return;
	}

	JniMethodInfo m_getKey;
	if (!JniHelper::getMethodInfo(m_getKey, "java/util/Map$Entry", "getKey",
			"()Ljava/lang/Object;")) {
		LOGE("Get 'getKey' method of 'Map$Entry' failed!");
		return;
	}

	JniMethodInfo m_getValue;
	if (!JniHelper::getMethodInfo(m_getValue, "java/util/Map$Entry", "getValue",
			"()Ljava/lang/Object;")) {
		LOGE("Get 'getValue' method of 'Map$Entry' failed!");
		return;
	}

	jobject entry;
	jstring key, value;
	while ((bool) env->CallBooleanMethod(itr, m_hasNext.methodID)) {
		entry = env->CallObjectMethod(itr, m_next.methodID);
		key = (jstring) env->CallObjectMethod(entry, m_getKey.methodID);
		value = (jstring) env->CallObjectMethod(entry, m_getValue.methodID);
		nativeMap->insert(
				std::pair<std::string, std::string>(
						JniHelper::jstring2string(key),
						JniHelper::jstring2string(value)));
	}
}

NativeHelperAndroid::NativeHelperAndroid(void) {
}
NativeHelperAndroid::~NativeHelperAndroid(void) {
}

void NativeHelperAndroid::showToastJNI(const std::string &message) {
	JniMethodInfo t;
	if (JniHelper::getStaticMethodInfo(t, JAVA_CLASS_NAME, "showToast",
			"(Ljava/lang/String;)V")) {
		t.env->CallStaticVoidMethod(t.classID, t.methodID,
				t.env->NewStringUTF(message.c_str()));
	}
}

void NativeHelperAndroid::hideLoadingJNI() {
	JniMethodInfo t;
	if (JniHelper::getStaticMethodInfo(t, JAVA_CLASS_NAME, "hideLoading",
			"()V")) {
		t.env->CallStaticVoidMethod(t.classID, t.methodID);
	}
}

//void NativeApi::sendImageToWXJNI(const std::string& imagePath) {
//	JniMethodInfo t;
//	if (JniHelper::getStaticMethodInfo(t, JAVA_CLASS_NAME, "sendImageToWX",
//			"(Ljava/lang/String;)V")) {
//		t.env->CallStaticVoidMethod(t.classID, t.methodID,
//				t.env->NewStringUTF(imagePath.c_str()));
//	}
//}

const std::string NativeHelperAndroid::getExternalDirectoryJNI() {

	JniMethodInfo t;
	if (JniHelper::getStaticMethodInfo(t, JAVA_CLASS_NAME,
			"getExternalDirectory", "()Ljava/lang/Object;")) {
		jstring jstr = (jstring) t.env->CallStaticObjectMethod(t.classID,
				t.methodID);
		return JniHelper::jstring2string(jstr);
	} else {
		return NULL;
	}
}
bool NativeHelperAndroid::getStringJNI(std::string& refString, const char* className) {
	JniMethodInfo t;
	bool bRet = JniHelper::getStaticMethodInfo(t, JAVA_CLASS_NAME, className,
			"()Ljava/lang/String;");
	if (bRet) {
		jobject objstring = t.env->CallStaticObjectMethod(t.classID,
				t.methodID);
		refString = JniHelper::jstring2string((jstring)(objstring));
		t.env->DeleteLocalRef(t.classID);
	}
	return bRet;
}

bool NativeHelperAndroid::getDeviceNameJNI(std::string& deviceName) {
	return NativeHelperAndroid::getStringJNI(deviceName, "getDeviceName");
}

bool NativeHelperAndroid::getDeviceCodeJNI(std::string& deviceCode) {
	return NativeHelperAndroid::getStringJNI(deviceCode, "getDeviceCode");

}

void NativeHelperAndroid::vibrateJNI(int millisecond) {
	JniMethodInfo t;
	if (JniHelper::getStaticMethodInfo(t, JAVA_CLASS_NAME, "vibrate", "(I)V")) {
		t.env->CallStaticVoidMethod(t.classID, t.methodID, millisecond);
		t.env->DeleteLocalRef(t.classID);
	}
}
// Cocos 活动周期函数
void NativeHelperAndroid::onApplicationDidFinishLaunching() {
	JniMethodInfo t;
		if (JniHelper::getStaticMethodInfo(t, JAVA_CLASS_NAME, "onApplicationDidFinishLaunching", "()V")) {
			t.env->CallStaticVoidMethod(t.classID, t.methodID);
			t.env->DeleteLocalRef(t.classID);
		}
}

void NativeHelperAndroid::onApplicationDidEnterBackground() {
	JniMethodInfo t;
			if (JniHelper::getStaticMethodInfo(t, JAVA_CLASS_NAME, "onApplicationDidEnterBackground", "()V")) {
				t.env->CallStaticVoidMethod(t.classID, t.methodID);
				t.env->DeleteLocalRef(t.classID);
			}
}

void NativeHelperAndroid::onApplicationWillEnterForeground() {
	JniMethodInfo t;
	if (JniHelper::getStaticMethodInfo(t, JAVA_CLASS_NAME, "onApplicationWillEnterForeground", "()V")) {
		t.env->CallStaticVoidMethod(t.classID, t.methodID);
		t.env->DeleteLocalRef(t.classID);
	}
}

void NativeHelperAndroid::setScreenOrientationJNI(int orientation) {
	JniMethodInfo t;
	if (JniHelper::getStaticMethodInfo(t, JAVA_CLASS_NAME, "setScreenOrientation", "(I)V")) {
		t.env->CallStaticVoidMethod(t.classID, t.methodID, orientation);
		t.env->DeleteLocalRef(t.classID);
	}
}

void NativeHelperAndroid::doPayJNI(const std::string& url, int payType,
		StringMap* params) {

//	JniMethodInfo t;
//	if (JniHelper::getStaticMethodInfo(t, JAVA_CLASS_NAME, "doPay",
//			"(Ljava/lang/String;ILjava/util/HashMap;)V")) {
//		jobject obj = createJavaMapObject(t.env, params);
//		t.env->CallStaticVoidMethod(t.classID, t.methodID,
//				t.env->NewStringUTF(url.c_str()), (int) payType, obj);
//	} else {
//		LOGE("Get static method: doPay failed!");
//	}
}

StringW NativeHelperAndroid::gbkToUtf16JNI(const StringA& strGbk) {

	StringW strUtf16;
	JniMethodInfo t;
	jclass classid = t.env->FindClass("java/lang/String");
	jmethodID fn = t.env->GetMethodID(classid, "<init>",
			"([BLjava/lang/String;)V");
	jbyteArray arr = t.env->NewByteArray(strGbk.length());
	t.env->SetByteArrayRegion(arr, 0, strGbk.length(),
			reinterpret_cast<const jbyte*>(strGbk.c_str()));
	jstring strCharset = t.env->NewStringUTF("gb2312");
	jstring strObject = reinterpret_cast<jstring>(t.env->NewObject(classid, fn,
			arr, strCharset));
	if (strObject) {
		const jchar* pData = t.env->GetStringChars(strObject, NULL);
		if (pData)
			strUtf16 = reinterpret_cast<const char16_t*>(pData);
		t.env->ReleaseStringChars(strObject, pData);

	}
	t.env->DeleteLocalRef(classid);
	return strUtf16.c_str();
}

StringA NativeHelperAndroid::utf16ToGbkJNI(const StringW& strUtf16) {
	StringA strGBK;
	JniMethodInfo t;
	jclass classid = t.env->FindClass("java/lang/String");
	if (classid == 0) {
		return strGBK;
	}
	jmethodID fn = t.env->GetMethodID(classid, "getBytes",
			"(Ljava/lang/String;)[B");
	if (fn == NULL) {
		//WriteLog(LOG_ERROR,"无法获得构造函数");
		return strGBK;
	}
	jstring strJavaUtf16 = t.env->NewString(
			reinterpret_cast<const jchar*>(strUtf16.c_str()),
			strUtf16.length());
	jstring strCharset = t.env->NewStringUTF("gb2312");
	jbyteArray pObject = reinterpret_cast<jbyteArray>(t.env->CallObjectMethod(
			strJavaUtf16, fn, strCharset));
	if (pObject) {
		char* pData = reinterpret_cast<char*>(t.env->GetByteArrayElements(
				pObject, NULL));
		int nLen = t.env->GetArrayLength(pObject);
		strGBK.assign(pData, pData + nLen);
	}
	t.env->DeleteLocalRef(classid);
	return strGBK.c_str();

}

void NativeHelperAndroid::shareToWechatJNI(StringMap* params) {
}

 bool NativeHelperAndroid::startRecodeJNI(const char* filepath)
{
	JniMethodInfo t;
		if (JniHelper::getStaticMethodInfo(t, JAVA_CLASS_NAME,"startRecord", "(Ljava/lang/String;)V")) {
				t.env->CallStaticVoidMethod(t.classID,t.methodID,t.env->NewStringUTF(filepath));
				t.env->DeleteLocalRef(t.classID);
				return true;
		} else {
				LOGE("Get static method: startRecord failed!");
				return false;
		}
}

 void NativeHelperAndroid::stopRecodeJNI()
{
	JniMethodInfo t;
		if (JniHelper::getStaticMethodInfo(t, JAVA_CLASS_NAME,"stopRecord", "()V")) {
				t.env->CallStaticVoidMethod(t.classID,t.methodID);
				t.env->DeleteLocalRef(t.classID);
		} else {
				LOGE("Get static method: stopRecord failed!");
		}
}
 int NativeHelperAndroid::getBatteryLevel()
 {
	 jint levelinfo=0;
		JniMethodInfo t;
			if (JniHelper::getStaticMethodInfo(t, JAVA_CLASS_NAME,"getBatteryLevel", "()I")) {
				levelinfo=t.env->CallStaticIntMethod(t.classID,t.methodID);
					t.env->DeleteLocalRef(t.classID);
			} else {
					LOGE("Get static method: getBatteryLevel failed!");
			}
	return (int)levelinfo;
 }

//****************-*************************************************
extern "C" {
	bool _IsAppInit(void);
	bool Java_com_weile_api_NativeHelper_nativeIsLandScape(JNIEnv *env,
			jobject thisz) {
		return SimpleConfigParser::getInstance()->isLanscape();
	}

	JNIEXPORT jstring JNICALL Java_com_weile_api_NativeHelper_nativeOnBatteryChangeNotify(
			JNIEnv *env, jclass, jint state) {
		auto eventDispatcher = cocos2d::Director::getInstance()->getEventDispatcher();
		eventDispatcher->dispatchCustomEvent("BATTERY_STATE_CHANGED_EVENT",reinterpret_cast<void*>(state));
	}

	JNIEXPORT jstring JNICALL Java_com_weile_api_NativeHelper_nativeOnNetworkChangeNotify(
			JNIEnv *env, jclass, jint state) {
		auto eventDispatcher = cocos2d::Director::getInstance()->getEventDispatcher();
				eventDispatcher->dispatchCustomEvent("NETWORK_STATE_CHANGED_EVENT",reinterpret_cast<void*>(state));
	}

	JNIEXPORT jstring JNICALL Java_com_weile_api_NativeHelper_nativeLuaGetGlobal(
			JNIEnv *env, jclass, jstring key) {
		if (_IsAppInit()) {
			const char* c_key = env->GetStringUTFChars(key, NULL);
			CLuaThread L;
			lua_getglobal(L, c_key);
			const char* c_val = lua_tostring(L, -1);
			lua_pop(L, 1);
			LOGE("----------------%s", c_val);
			env->ReleaseStringUTFChars(key, c_key);
			return env->NewStringUTF(c_val);
		}else
		{
			LOGE("----------------_IsAppInit =false");
			return NULL;
		}
	}
}

//******************************************

