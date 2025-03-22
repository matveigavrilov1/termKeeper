#pragma once

#include <vector>
#include <algorithm>

#include "utils/observer.h"

namespace tk
{
class subject
{
public:
	void attach(observer::shared_ptr_type observer) { observers_.push_back(observer); }

	void detach(observer::shared_ptr_type observer) { observers_.erase(std::remove(observers_.begin(), observers_.end(), observer), observers_.end()); }

	void notify(const std::string& message)
	{
		for (const auto& observer : observers_)
		{
			observer->update(message);
		}
	}

private:
	std::vector<observer::shared_ptr_type> observers_;
};
} // namespace tk