#pragma once

#include <string>
#include <sstream>
#include <iostream>

#include <fstream>

// TODO: Optimize logs
// clang-format off
#ifdef DEBUG
#define LOG_INF(message)                              \
	do                                                \
	{                                                 \
		std::fstream ss("log.txt", std::ios::app);	                         \
		ss << "[INF] | "<< message << std::endl;                   \
	} while (0)
#else
#define LOG_INF(message)
#endif

#ifdef DEBUG
#define LOG_DBG(message)                              \
	do                                                \
	{                                                 \
		std::fstream ss("log.txt", std::ios::app);	                        \
		ss << "[DBG] | "<< message << std::endl;                   \
	} while (0)
#else
#define LOG_DBG(message)
#endif

#ifdef DEBUG
#define LOG_WRN(message)                              \
	do                                                \
	{                                                 \
		std::fstream ss("log.txt", std::ios::app);	                         \
		ss << "[WRN] | "<< message << std::endl;                 \
	} while (0)
#else
#define LOG_WRN(message)
#endif


#ifdef DEBUG
#define LOG_ERR(message)                            \
	do                                                \
	{                                                 \
		std::fstream ss("log.txt", std::ios::app);	;                         \
		ss << "[ERR] | "<< message << std::endl;                  \
	} while (0)
#else
#define LOG_ERR(message)
#endif

// clang-format on;