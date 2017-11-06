#pragma once

#include "cocos2d.h"
#include "audio/include/SimpleAudioEngine.h"

USING_NS_CC;
using namespace CocosDenshion;

class AudioControl
{
public:
	static AudioControl* getInstance();
	virtual void init();

	static void end();
	virtual void preloadBackgroundMusic(const char* pszFilePath);
	virtual void playBackgroundMusic(const char* filePath, bool loop = false);
	virtual void stopBackgroundMusic(bool releaseData = false);
	virtual void pauseBackgroundMusic();
	virtual void resumeBackgroundMusic();
	virtual void rewindBackgroundMusic();
	virtual bool willPlayBackgroundMusic();
	virtual bool isBackgroundMusicPlaying();
	virtual float getBackgroundMusicVolume();
	virtual void setBackgroundMusicVolume(float volume);

	virtual float getEffectsVolume();
	virtual void setEffectsVolume(float volume);
	virtual unsigned int playEffect(const char* filePath, bool loop = false,
		float pitch = 1.0f, float pan = 0.0f, float gain = 1.0f);
	virtual void pauseEffect(unsigned int soundId);
	virtual void pauseAllEffects();
	virtual void resumeEffect(unsigned int soundId);
	virtual void resumeAllEffects();
	virtual void stopAllEffects();
	virtual void preloadEffect(const char* filePath);
	virtual void unloadEffect(const char* filePath);
	virtual void stopEffect(unsigned int soundId);

	void setBackgroundMusicStatus(bool status);
	bool getBackgroundMusicStatus();
	void setEffectStatus(bool status);
	bool getEffectStatus();
protected:
	AudioControl();
	virtual ~AudioControl();
private:
	bool _BackgroundMusicStatus;
	float _BackgroundMusicVolume;
	bool _EffectStatus;
	float _EffectVolume;
};