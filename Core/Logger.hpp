#ifndef LOGGING__HPP_
#define LOGGING__HPP_

#include "Define.h"

#include <csignal>
#include <fstream>
#include <memory>
#include <string>
#include <filesystem>
#include <mutex>
#include <format>
#include <chrono>
#include <iostream>

namespace ch = std::chrono;

enum class LogLevel : int8
{
	DEBUG = 0, 
	WARNING = 38, 
	ERROR = 31
};

class Logger
{
public:
	inline static const std::filesystem::path LogDir{"logs"};

	template <typename ... Params>
	static void LogWithLevel(LogLevel level, Params... params);
	
	template <typename ... Params>
	static void Log(Params... params);

	~Logger() = default; 

private:
	Logger() = default;

	void CreateLogFile();
	void PurgeOldFiles();

	static Logger& Instance();
	static void SignalHandler(int32 signal);

	static std::string ToColorCode(LogLevel level);
	static std::string ToString(LogLevel level);

private:

	inline static std::unique_ptr<Logger> pInstance;
    inline static std::mutex s_mutex;

	std::fstream m_file {};
};

#ifdef NO_LOGGER
template <typename ... Params>
void Logger::Log(Params... params) {}

template <typename ... Params>
void Logger::LogWithLevel(LogLevel level, Params... params) {}
#else
#include "Logger.inl"
#endif


#endif // !LOGGING__H_
