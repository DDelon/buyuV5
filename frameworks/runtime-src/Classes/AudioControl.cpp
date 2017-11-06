#include "AudioControl.h"
//#include "base/CCUserDefault.h"

AudioControl::AudioControl()
{
}

AudioControl::~AudioControl()
{
}

AudioControl* AudioControl::getInstance()
{
	static AudioControl s_SharedAudioControl;
	static bool s_FirstLoad = true;
	if (s_FirstLoad)
	{
		s_FirstLoad = false;
		s_SharedAudioControl.init();
	}
	return &s_SharedAudioControl;
}

void AudioControl::init()
{
	if (CCUserDefault::getInstance()->getBoolForKey("isXMLFileExist"))
	{
		_BackgroundMusicStatus = CCUserDefault::getInstance()->getBoolForKey("musicStatus");
		_BackgroundMusicVolume = CCUserDefault::getInstance()->getFloatForKey("musicVolume");
		_EffectStatus = CCUserDefault::getInstance()->getBoolForKey("effectStatus");
		_EffectVolume = CCUserDefault::getInstance()->getFloatForKey("effectVolume");
	}
	else
	{
		_BackgroundMusicStatus = true;
		_BackgroundMusicVolume = 1.0;
		_EffectStatus = true;
		_EffectVolume = 1.0;
		CCUserDefault::getInstance()->setBoolForKey("isXMLFileExist", true);
	}
	//AudioControl::getInstance()->setBackgroundMusicStatus(_BackgroundMusicStatus);
	//AudioControl::getInstance()->setBackgroundMusicVolume(_BackgroundMusicVolume);
	//AudioControl::getInstance()->setEffectStatus(_EffectStatus);
	//AudioControl::getInstance()->setEffectsVolume(_EffectVolume);
}

void AudioControl::end()
{
	SimpleAudioEngine::getInstance()->end();
}

//////////////////////////////////////////////////////////////////////////
// BackgroundMusic
//////////////////////////////////////////////////////////////////////////

void AudioControl::playBackgroundMusic(const char* pszFilePath, bool bLoop)
{
	if (_BackgroundMusicStatus)
	{
		SimpleAudioEngine::getInstance()->playBackgroundMusic(pszFilePath, bLoop);
	}
}

void AudioControl::stopBackgroundMusic(bool releaseData)
{
	SimpleAudioEngine::getInstance()->stopBackgroundMusic(releaseData);
}

void AudioControl::pauseBackgroundMusic()
{
	SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

void AudioControl::resumeBackgroundMusic()
{
	if (_BackgroundMusicStatus)
	{
		SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
	}
}

void AudioControl::rewindBackgroundMusic()
{
	SimpleAudioEngine::getInstance()->rewindBackgroundMusic();
}

bool AudioControl::willPlayBackgroundMusic()
{
	if (_BackgroundMusicStatus)
	{
		return SimpleAudioEngine::getInstance()->willPlayBackgroundMusic();
	}
	else
	{
		return false;
	}
}

bool AudioControl::isBackgroundMusicPlaying()
{
	if (_BackgroundMusicStatus)
	{
		return SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying();
	}
	else
	{
		return false;
	}
}

//////////////////////////////////////////////////////////////////////////
// effect function
//////////////////////////////////////////////////////////////////////////

unsigned int AudioControl::playEffect(const char* pszFilePath, bool bLoop,
	float pitch, float pan, float gain)
{
	if (_EffectStatus)
	{
		return SimpleAudioEngine::getInstance()->playEffect(pszFilePath, bLoop, pitch, pan, gain);
	}
	return 0;
}

void AudioControl::stopAllEffects()
{
	SimpleAudioEngine::getInstance()->stopAllEffects();
}

void AudioControl::preloadEffect(const char* pszFilePath)
{
	SimpleAudioEngine::getInstance()->preloadEffect(pszFilePath);
}

void AudioControl::pauseEffect(unsigned int soundId)
{
	SimpleAudioEngine::getInstance()->pauseEffect(soundId);
}

void AudioControl::pauseAllEffects()
{
	SimpleAudioEngine::getInstance()->pauseAllEffects();
}

void AudioControl::resumeEffect(unsigned int nSoundId)
{
	if (_EffectStatus)
	{
		SimpleAudioEngine::getInstance()->resumeEffect(nSoundId);
	}
}

void AudioControl::resumeAllEffects()
{
	if (_EffectStatus)
	{
		SimpleAudioEngine::getInstance()->resumeAllEffects();
	}
}

void AudioControl::preloadBackgroundMusic(const char* pszFilePath)
{
	if (_BackgroundMusicStatus)
	{
		SimpleAudioEngine::getInstance()->preloadBackgroundMusic(pszFilePath);
	}
}

void AudioControl::unloadEffect(const char* filePath)
{
	SimpleAudioEngine::getInstance()->unloadEffect(filePath);
}

void AudioControl::stopEffect(unsigned int soundId)
{
	SimpleAudioEngine::getInstance()->stopEffect(soundId);
}

//////////////////////////////////////////////////////////////////////////
// volume interface
//////////////////////////////////////////////////////////////////////////

float AudioControl::getBackgroundMusicVolume()
{
	return _BackgroundMusicVolume;
	//return SimpleAudioEngine::getInstance()->getBackgroundMusicVolume();
}

void AudioControl::setBackgroundMusicVolume(float volume)
{
	_BackgroundMusicVolume = volume;
// 	CCUserDefault::getInstance()->setFloatForKey("backgroundMusicVolume", _BackgroundMusicVolume);
// 	CCUserDefault::getInstance()->flush();
 	SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(volume);
}

float AudioControl::getEffectsVolume()
{
	return _EffectVolume;
	//return SimpleAudioEngine::getInstance()->getEffectsVolume();
}

void AudioControl::setEffectsVolume(float volume)
{
	_EffectVolume = volume;
// 	CCUserDefault::getInstance()->setFloatForKey("effectVolume", _EffectVolume);
// 	CCUserDefault::getInstance()->flush();
	SimpleAudioEngine::getInstance()->setEffectsVolume(volume);
}

void AudioControl::setBackgroundMusicStatus(bool status)
{
	_BackgroundMusicStatus = status;
// 	CCUserDefault::getInstance()->setBoolForKey("backgroundMusicStatus", status);
// 	CCUserDefault::getInstance()->flush();
}

bool AudioControl::getBackgroundMusicStatus()
{
	return _BackgroundMusicStatus;
}

void AudioControl::setEffectStatus(bool status)
{
	_EffectStatus = status;
// 	CCUserDefault::getInstance()->setBoolForKey("effectStatus", status);
// 	CCUserDefault::getInstance()->flush();
}

bool AudioControl::getEffectStatus()
{
	return _EffectStatus;
}