#pragma once

#include <string>
#include <iostream>

std::string getFileName(const std::string& filePath);

// clang-format off
#define LOG_INF(message)                                                                                             \
	do                                                                                                                \
	{                                                                                                                 \
		std::cout << "[INF] " << getFileName(__FILE__) << ":" << __LINE__ << " | " << message << std::endl;           \
	} while (0)

#ifdef DEBUG
#define LOG_DBG(message)                                                                                            \
	do                                                                                                                \
	{                                                                                                                 \
		std::cout << "[DBG] " << getFileName(__FILE__) << ":" << __LINE__ << " | " << message << std::endl;           \
	} while (0)
#else
#define LOG_DBG(message)
#endif

#define LOG_ERR(message)                                                                                            \
	do                                                                                                                \
	{                                                                                                                 \
		std::cerr << "[ERR] " << getFileName(__FILE__) << ":" << __LINE__ << " | " << message << std::endl;           \
	} while (0)

#define LOG_WRN(message)                                                                                              \
	do                                                                                                                \
	{                                                                                                                 \
		std::cout << "[WRN] " << getFileName(__FILE__) << ":" << __LINE__ << " | " << message << std::endl;           \
	} while (0)
// clang-format on