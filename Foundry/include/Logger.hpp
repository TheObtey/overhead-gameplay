#ifndef LOGGING__HPP_
#define LOGGING__HPP_

#include "Define.h"

#include <chrono>
#include <csignal>
#include <cstdlib>
#include <format>
#include <fstream>
#include <ios>
#include <iostream>
#include <memory>
#include <string>
#include <filesystem>
#include <mutex>


//=== ANSI ESCAPE CODE ========
#define ESC "\x1B"
#define ANSI_RED      ESC"[31m"
#define ANSI_GREEN    ESC"[32m"
#define ANSI_GOLD     ESC"[38;5;178m"

#define ANSI_ITALIC   ESC"[3m"
#define ANSI_BOLD     ESC"[1m"

#define ANSI_RESET    ESC"[0m"
//=============================

#define ENGINE_NAME "FORGE"

namespace ch = std::chrono;

class Logger
{
public:
	class LogLevel;

	static constexpr int32 MaxLogFiles {10};
	inline static const std::filesystem::path LogDir{"logs"};

	template <typename ... Params>
	static void LogWithLevel(LogLevel level, Params... params);
	
	template <typename ... Params>
	static void Log(Params... params);

	~Logger() = default; 

private:
	Logger() = default;

#ifndef NO_LOGGER
	static Logger& Instance()
	{
    	std::lock_guard<std::mutex> lock(s_mutex);
		if (pInstance == nullptr)
		{
			pInstance = std::unique_ptr<Logger>(new Logger());
			pInstance->CreateLogFile();

			//attach signal to try to log crash
			std::signal(SIGSEGV, SignalHandler);
			std::signal(SIGABRT, SignalHandler);
			std::signal(SIGFPE, SignalHandler);
		}

		return *pInstance;
	}

	void CreateLogFile();
	void PurgeOldFiles();
	static void SignalHandler(int32 signal);
#endif // !NO_LOGGER

private:

#ifndef NO_LOGGER
	inline static std::unique_ptr<Logger> pInstance;
    inline static std::mutex s_mutex;

	std::fstream m_file {};
#endif // !NO_LOGGER
};

//===============================================================
class Logger::LogLevel
{
public:
	enum LogLevelEnum
	{
		INFO, WARNING, ERROR
	};

	LogLevel() = default;
	LogLevel(LogLevelEnum value) : m_value(value) {}

	constexpr operator LogLevelEnum() const { return m_value; }
  	explicit operator bool() const = delete; 

    constexpr bool operator==(LogLevel a) const { return m_value == a.m_value; }
    constexpr bool operator!=(LogLevel a) const { return m_value != a.m_value; }

	std::string AsAnsi() const
	{
		switch (m_value)
		{
			case WARNING: return ANSI_GOLD;
			case ERROR: return ANSI_RED;
			default: return "";
		}
	}

private:
	LogLevelEnum m_value;
};

template <typename ... Params>
inline void Logger::Log(Params... params)
{
	LogWithLevel(Logger::LogLevel::INFO, params ...);
}

template <typename ... Params>
inline void Logger::LogWithLevel(LogLevel level, Params... params) 
{
#ifdef NO_LOGGER
	return;
#else
	Logger& instance = Instance();

	auto time = ch::floor<ch::seconds>(ch::system_clock::now());
	std::stringstream ss; 
	ss << std::format("{} [{}] ({:%T}) >> ", level.AsAnsi(), ENGINE_NAME, time);

	(ss << ... << params);
	ss << ANSI_RESET << std::endl;

	std::cout << ss.str();
	instance.m_file << ss.str();
#endif
}

#ifndef NO_LOGGER
inline void Logger::SignalHandler(int32 signal)
{
	LogWithLevel(Logger::LogLevel::ERROR, "OH NOOOO APP CRASHED WITH SIGNAL : ", signal);
	std::exit(-1);
}

inline void Logger::CreateLogFile()
{
	auto time = ch::floor<ch::seconds>(ch::system_clock::now());

	std::filesystem::create_directory(LogDir);
	m_file.open(LogDir/std::format("{:%Y-%m-%d_%H-%M-%S}.txt", time), std::ios::out);

	PurgeOldFiles();
}


inline void Logger::PurgeOldFiles()
{
	auto const time = ch::system_clock::now();

	for (auto& file : std::filesystem::directory_iterator{LogDir})
	{
	    const auto file_time_sys = ch::clock_cast<ch::system_clock>(file.last_write_time());

		if ((time - file_time_sys) >= ch::minutes(10))
		{
			std::filesystem::remove(file.path());
		}
	}
}
#endif // !NO_LOGGER


#endif // !LOGGING__H_
