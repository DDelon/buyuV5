//
//  AudioManager.h
//  WeiLeGame
//
//  Created by 王 兵 on 16/8/31.
//
//

typedef void(*CBAUDIOPLAYCOMPLETE)(unsigned, void*);
class CAudioManager
{
	CAudioManager();
	~CAudioManager();
public:
	bool StartCapture(const char* filename);

	void StopCapture();


	static CAudioManager& GetInstance()
	{
		static CAudioManager ins;
		return ins;
	}


private:
	void*	m_pRecorder;

};
