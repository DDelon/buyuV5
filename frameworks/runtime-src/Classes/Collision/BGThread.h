#ifndef __BG_THREAD_H__
#define __BG_THREAD_H__
#include "cocos2d.h"
#include <thread>
#include <condition_variable>
#include <mutex>
USING_NS_CC;

#define NULL_TASK_ID -1



struct TaskItem
{
	int taskId;
	std::string taskName;
	std::function<void()> func;
};

class CBGThread
{
public:
	enum STATE
	{
		RUN,
		WAIT,
		STOP,
	};

public:
	CBGThread();
	~CBGThread();

	void begin();

	void stop();

	void wait();

	void notify();

	void addTask(std::string name, std::function<void ()> func);
private:
	void workFunc();

private:
	std::vector<TaskItem> _taskQueue;

	std::thread *_pThread;

	STATE _state;

	std::mutex _mutex;

	std::condition_variable _condition;
};

#endif