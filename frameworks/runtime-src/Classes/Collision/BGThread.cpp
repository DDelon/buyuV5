#include "BGThread.h"
#include "cocos2d.h"
#include <iostream>
USING_NS_CC;
CBGThread::CBGThread()
{

}

CBGThread::~CBGThread()
{
	delete _pThread;
}

void CBGThread::begin()
{
	_state = STATE::RUN;
	_pThread = new std::thread(&CBGThread::workFunc, this);
	_pThread->detach();
	
}

void CBGThread::stop()
{
	
	
	_mutex.lock();
	_taskQueue.clear();
	_mutex.unlock();

	std::lock_guard<std::mutex> lock{ _mutex };
	_state = STATE::STOP;
	_condition.notify_one(); // notify one !

}

void CBGThread::wait()
{
	std::unique_lock<std::mutex> lock{ _mutex };
	_state = STATE::WAIT;
	_condition.wait(lock);
}

void CBGThread::notify()
{
	std::lock_guard<std::mutex> lock{ _mutex };
	_state = STATE::RUN;
	_condition.notify_one(); // notify one !
}

void CBGThread::addTask(std::string name, std::function<void()> func)
{
	//log("add task-----------------------------------------");
	TaskItem item;
	
	item.taskId = _taskQueue.size();
	item.taskName = name;
	item.func = func;
	_taskQueue.push_back(item);
	notify();
}

void CBGThread::workFunc()
{
	while (true)
	{
		if (_state == STATE::RUN)
		{
			if (_taskQueue.size() == 0)
			{
				wait();
			}

			TaskItem item;
			item.taskId = NULL_TASK_ID;
			_mutex.lock();
			if (_taskQueue.size() > 0) {
				int index = _taskQueue.size() - 1;
				item = _taskQueue[_taskQueue.size() - 1];
				_taskQueue.erase(_taskQueue.begin() + index);
			}
			_mutex.unlock();
			if (item.taskId != NULL_TASK_ID)
			{
				//ÓÐÈÎÎñ
				const char *name = item.taskName.c_str();
				_mutex.lock();
				item.func();
				_mutex.unlock();
			}

		}
		else if (_state == STATE::STOP)
		{
			break;
		}
	}
	delete _pThread;
}