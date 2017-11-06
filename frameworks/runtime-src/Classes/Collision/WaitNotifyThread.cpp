#include "WaitNotifyThread.h"

WaitNotifyThread::WaitNotifyThread()
{
	count = 0;
	mTask = nullptr;
	mIsRunning = false;
	mThreadFinish = true;
	mIsTasking = false;
}

WaitNotifyThread::~WaitNotifyThread()
{
	mTask = nullptr;
	mIsRunning = false;
}

void WaitNotifyThread::start(Task &task)
{
	if (!mThreadFinish && mIsRunning)
	{
		mTask = task;
	}
	else
	{
		mTask = task;
		mIsRunning = true;
		mThreadFinish = false;
		mThread = std::thread([=](){
			while (mIsRunning)
			{
				////CCLOG("=======wait! %d========", GetCurrentThreadId());
				wait();
				////CCLOG("=======notify!========");
				if (!mIsRunning)
				{
					break;
				}

				mIsTasking = true;
				mMutex.lock();
				if (mTask)
				{
					
					mTask();
				}
				mMutex.unlock();
				mIsTasking = false;
			}
			mThreadFinish = true;
			////CCLOG("============Thread finish============!");
		});
		mThread.detach();
	}


}

void WaitNotifyThread::wait()
{
	std::unique_lock<std::mutex> lock{ mMutex };
	if (--count < 0) // count is not enough ?
		condition.wait(lock); // suspend and wait...
}

void WaitNotifyThread::notify()
{
	if (mIsTasking)
	{
		return;
	}
	std::lock_guard<std::mutex> lock{ mMutex };
	if (++count <= 0) // have some thread suspended ?
		condition.notify_one(); // notify one !
}

void WaitNotifyThread::stop()
{
	mIsRunning = false;
	notify();
}

