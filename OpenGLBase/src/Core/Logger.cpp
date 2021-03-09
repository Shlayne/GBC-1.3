#include "cbcpch.h"
#include "Logger.h"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace cbc
{
	Ref<spdlog::logger> Logger::coreLogger;
	Ref<spdlog::logger> Logger::clientLogger;

	void Logger::Init()
	{
		spdlog::set_pattern("%^[%T] %n: %v%$");

		coreLogger = spdlog::stdout_color_mt("CBC");
		coreLogger->set_level(spdlog::level::trace);

		clientLogger = spdlog::stdout_color_mt("Client");
		clientLogger->set_level(spdlog::level::trace);
	}
}
