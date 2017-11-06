#ifndef __WAITNOTIFYTHREAD_H__
#define __WAITNOTIFYTHREAD_H__
#include "cocos2d.h"
#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>
using namespace std;
USING_NS_CC;
typedef std::function<void()> Task;

class WaitNotifyThread{
public:
	WaitNotifyThread();
	~WaitNotifyThread();

	void start(Task &task);
	void stop();

	void wait();
	void notify();
	bool isWorking() { return mIsTasking; }
private:

	std::thread mThread;
	int count;
	std::mutex mMutex;
	std::condition_variable condition;
	Task mTask;

	bool mIsRunning;
	bool mThreadFinish;
	bool mIsTasking;
};


#endif // !__WAITNOTIFYTHREAD_H__
