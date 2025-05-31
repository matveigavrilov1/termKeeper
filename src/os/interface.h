#pragma once

#include <atomic>
#include <string>

namespace os 
{
	int runApp(const char * appName, std::atomic<bool>& running);
	void writeToClipboard(const std::string& content);
}