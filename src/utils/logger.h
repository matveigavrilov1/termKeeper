#pragma once

#include <string>
#include <sstream>
#include <iostream>

// TODO: Optimize logs
// clang-format off
#define LOG_INF(message)                              \
	do                                                \
	{                                                 \
		std::stringstream ss;                         \
		ss << "[INF] | "<< message;                   \
	} while (0)

#define LOG_DBG(message)                              \
	do                                                \
	{                                                 \
		std::stringstream ss;                         \
		ss << "[DBG] | "<< message;                   \
	} while (0)


#define LOG_WRN(message)                              \
	do                                                \
	{                                                 \
		std::stringstream ss;                         \
		ss << "[WRN] | "<< message;                   \
	} while (0)


#define LOG_ERR(message)                              \
	do                                                \
	{                                                 \
		std::stringstream ss;                         \
		ss << "[ERR] | "<< message;                   \
	} while (0)

// clang-format on