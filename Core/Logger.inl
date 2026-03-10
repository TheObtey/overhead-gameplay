inline Logger& Logger::Instance()
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

inline void Logger::SignalHandler(int32 signal)
{
	LogWithLevel(LogLevel::ERROR, "OH NOOOO APP CRASHED WITH SIGNAL : ", signal);
	std::exit(-1);
}

inline void Logger::CreateLogFile()
{
	auto time = ch::floor<ch::seconds>(ch::system_clock::now());

	std::filesystem::create_directory(LogDir);
	m_file.open(LogDir/std::format("{:%Y-%m-%d_%H-%M-%S}.log", time), std::ios::out);

	PurgeOldFiles();
}

inline void Logger::PurgeOldFiles()
{
	auto const time = ch::system_clock::now();

	for (auto& file : std::filesystem::directory_iterator{LogDir})
	{
	    //const auto file_time_sys = ch::clock_cast<ch::system_clock>(file.last_write_time());

		//if ((time - file_time_sys) >= ch::minutes(10))
		//{
		//	std::filesystem::remove(file.path());
		//}
	}
}


inline std::string Logger::ToString(LogLevel level)
{
	switch (level)
	{
		case LogLevel::DEBUG : return "DEBUG";
		case LogLevel::WARNING : return "WARNING";
		case LogLevel::ERROR : return "ERROR";
		default: return "";
	}
}

inline std::string Logger::ToColorCode(LogLevel level)
{
	switch (level)
	{
		case LogLevel::WARNING : return ANSI_GOLD;
		case LogLevel::ERROR : return ANSI_RED;
		default: return "";
	}
}

template <typename ... Params>
inline void Logger::Log(Params... params)
{
	LogWithLevel(LogLevel::DEBUG, params ...);
}

template <typename ... Params>
inline void Logger::LogWithLevel(LogLevel level, Params... params) 
{
	Logger& instance = Instance();

	auto time = ch::floor<ch::seconds>(ch::system_clock::now());

	std::stringstream ss; 
	ss << std::format("[{}] ({:%T}) >> ", ToString(level), time);
	(ss << ... << params);

	std::cout << ToColorCode(level) << ss.str() << ANSI_RESET << std::endl;
	instance.m_file << ss.str() << std::endl;
}
