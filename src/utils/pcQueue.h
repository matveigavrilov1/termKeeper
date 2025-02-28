#pragma once
#include <queue>
#include <mutex>
#include <condition_variable>

namespace tk

{

template<typename T>
class pcQueue
{
public:
	void push(const T& item)
	{
		std::unique_lock<std::mutex> lock(mutex_);
		queue_.push(item);
		lock.unlock();
		cond_.notify_one();
	}

	T wait_and_pop()
	{
		std::unique_lock<std::mutex> lock(mutex_);
		cond_.wait(lock, [this]() { return !queue_.empty(); });
		T item = queue_.front();
		queue_.pop();
		return item;
	}

	bool empty() const
	{
		std::unique_lock<std::mutex> lock(mutex_);
		return queue_.empty();
	}

	bool size() const
	{
		std::unique_lock<std::mutex> lock(mutex_);
		return queue_.size();
	}

private:
	std::queue<T> queue_;
	mutable std::mutex mutex_;
	std::condition_variable cond_;
};
} // namespace tk