#ifndef TIDE_THREADPOOL_H
#define TIDE_THREADPOOL_H

//a singleton threadpool with priority
//C++11 ONLY!!!
//if use in lower C++ version, ensure constuctor just run once to avoid unexpected exception

#include <thread>
#include <functional>
#include <future>
#include <vector>
#include <atomic>
#include <queue>
#include <mutex>
#include <condition_variable>

using namespace std;

#define THREAD_NUM 8
#define MAX_THREAD_NUM 512

typedef function<void()> Task;

struct cmp
{
	bool operator()(const pair<int, Task>& left, const pair<int, Task>& right) {
		return left.first < right.first;
	}
};

class ThreadPool
{
	vector<thread> workers; //threads
	//priority_queue<pair<int, Task>, vector<pair<int, Task>>, cmp> tasks; //functions with priority
	queue<Task> tasks;

	mutex m_lock;
	condition_variable cv_task;
	bool stopped;
	atomic<int> idleNum;

private:

	inline ThreadPool(int num)
		:stopped(false)
	{
		idleNum = num;
		for (int i = 0; i < idleNum; i++)
		{
			workers.emplace_back(
				//use lambda
				[this]
				{
					while (!this->stopped)
					{
						Task task;
						{
							unique_lock<mutex> lock(this->m_lock);
							this->cv_task.wait(lock,
								//predicate function
								//true: threadpool stop or new task coming
								[this] () {return this->stopped || !this->tasks.empty();}
							);
							if (this->stopped && this->tasks.empty()) return;

							task = move(this->tasks.front());
							this->tasks.pop();
						}
						this->idleNum--;
						task();
						this->idleNum++;
					}
				}
			);
		}
	}

	inline ~ThreadPool()
	{
		{
			unique_lock<mutex> lock(this->m_lock);
			stopped = true;
		}
		cv_task.notify_all();
		for (thread& thread : workers)
		{
			if(thread.joinable()) thread.join();
		}
	}

	void expandThreadSize()
	{
		int curNum = workers.size();
		if (curNum >= MAX_THREAD_NUM) return;
		for (int i = 0; i < curNum; i++)
		{
			workers.emplace_back(
				//use lambda
				[this]
				{
					while (!this->stopped)
					{
						Task task;
						{
							unique_lock<mutex> lock(this->m_lock);
							this->cv_task.wait(lock,
								//predicate function
								//true: threadpool stop or new task coming
								[this] () {return this->stopped || !this->tasks.empty();}
							);
							if (this->stopped && this->tasks.empty()) return;

							task = move(this->tasks.front());
							this->tasks.pop();
						}
						this->idleNum--;
						task();
						this->idleNum++;
					}
				}
			);
			idleNum++;
		}
		return;
	}

public:

	ThreadPool(const ThreadPool&)=delete;
	ThreadPool& operator=(const ThreadPool&)=delete;

	static ThreadPool* getResourcePool()
	{
		static ThreadPool instance(THREAD_NUM);
		return &instance;
	}

	/*template<class F, class... Args>
	auto commit(int priority, F&& f, Args&&... args) ->future<decltype(f(args...))>
	{
		using retType = decltype(f(args...));
	
			original function:
				retType f(args);
			after binding:
				retType task();
			after packing:
				void task();
			make_shared: 
				transfer the ownership of dequeued task
		 */
		/*auto task = make_shared<packaged_task<retType()> >(bind(forward<F>(f), forward<Args>(args)...));
		//expect function output, block in result.get()
		future<retType> result = task->get_future();
		{
			lock_guard<mutex> lock(m_lock);
			//a Task equals to function<void()> as mentioned before
			tasks.emplace(make_pair(priority, [task](){ (*task)();}));
		}
		//expand thread num
		if (idleNum <= 0)
		{
			expandThreadSize();
		}
		cv_task.notify_one();
		return result;
	}*/

	//priority 0 
	template<class F, class... Args>
	auto commit(F&& f, Args&&... args) ->future<decltype(f(args...))>
	{
		using retType = decltype(f(args...));
		/*
			original function:
			    retType f(args);
			after binding:
			    retType task();
			after packing:
			    void task();
			make_shared: 
			    transfer the ownership of dequeued task
		 */
		auto task = make_shared<packaged_task<retType()> >(bind(forward<F>(f), forward<Args>(args)...));

		//expect function output, block in result.get()
		future<retType> result = task->get_future();

		{
			lock_guard<mutex> lock(m_lock);
			//a Task equals to function<void()> as mentioned before
			//tasks.emplace(make_pair(0, [task](){ (*task)();}));
			tasks.emplace([task](){ (*task)();});
		}
		//expand thread num
		if (idleNum <= 0)
		{
			expandThreadSize();
		}
		cv_task.notify_one();
		return result;
	}

	int checkIdleNum()
	{
		return idleNum;
	}

	int getSize()
	{
		return workers.size();
	}

	int getTaskNum()
	{
		return tasks.size();
	}
};

#endif