#pragma once

#include <atomic>

namespace tk
{
class inputm
{
public:
	void run();
	void stop();
	void worker();

private:
	std::atomic<bool> running_ { false };
};
} // namespace tk