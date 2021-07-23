#include "gbcpch.h"
#include "Logger.h"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace gbc
{
	Ref<spdlog::logger> Logger::coreLogger;
	Ref<spdlog::logger> Logger::clientLogger;

	void Logger::Init()
	{
		spdlog::set_pattern("%^[%T] %n: %v%$");

		coreLogger = spdlog::stdout_color_mt("GBC");
		coreLogger->set_level(GBC_LOGGING_LEVEL);

		clientLogger = spdlog::stdout_color_mt("Client");
		clientLogger->set_level(GBC_LOGGING_LEVEL);
	}
}
