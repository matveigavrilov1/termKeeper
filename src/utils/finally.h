#pragma once

#include <functional>

namespace utils
{
class finally
{
public:
	finally(std::function<void()> action)
	: action_(action)
	{ }

	~finally()
	{
		if (action_)
		{
			action_();
		}
	}

	finally(const finally&) = delete;
	finally& operator= (const finally&) = delete;

private:
	std::function<void()> action_;
};

} // namespace utils