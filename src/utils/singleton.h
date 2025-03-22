#pragma once

namespace tk
{
template<typename T>
class singleton
{
public:
	static T& instance()
	{
		static T instance_;
		return instance_;
	}

	singleton(const singleton<T>& other) = delete;
	singleton(singleton<T>&& other) = delete;

	singleton<T>& operator= (const singleton<T>& other) = delete;
	singleton<T>& operator= (singleton<T>&& other) = delete;

protected:
	singleton() = default;
	~singleton() = default;
};
} // namespace tk