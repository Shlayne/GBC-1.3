#pragma once

#include "Core.h"
#include "spdlog/spdlog.h"

namespace gbc
{
	class Logger
	{
	public:
		static void Init();

		inline static Ref<spdlog::logger>& GetCoreLogger() { return coreLogger; }
		inline static Ref<spdlog::logger>& GetClientLogger() { return clientLogger; }
	private:
		static Ref<spdlog::logger> coreLogger;
		static Ref<spdlog::logger> clientLogger;
	};
}

#if CBC_ENABLE_LOGGING
#define CBC_CORE_TRACE(...) ::gbc::Logger::GetCoreLogger()->trace(__VA_ARGS__)
#define CBC_CORE_DEBUG(...) ::gbc::Logger::GetCoreLogger()->debug(__VA_ARGS__)
#define CBC_CORE_INFO(...)  ::gbc::Logger::GetCoreLogger()->info(__VA_ARGS__)
#define CBC_CORE_WARN(...)  ::gbc::Logger::GetCoreLogger()->warn(__VA_ARGS__)
#define CBC_CORE_ERROR(...) ::gbc::Logger::GetCoreLogger()->error(__VA_ARGS__)
#define CBC_CORE_FATAL(...) ::gbc::Logger::GetCoreLogger()->critical(__VA_ARGS__)

#define CBC_CLIENT_TRACE(...) ::gbc::Logger::GetClientLogger()->trace(__VA_ARGS__)
#define CBC_CLIENT_DEBUG(...) ::gbc::Logger::GetClientLogger()->debug(__VA_ARGS__)
#define CBC_CLIENT_INFO(...)  ::gbc::Logger::GetClientLogger()->info(__VA_ARGS__)
#define CBC_CLIENT_WARN(...)  ::gbc::Logger::GetClientLogger()->warn(__VA_ARGS__)
#define CBC_CLIENT_ERROR(...) ::gbc::Logger::GetClientLogger()->error(__VA_ARGS__)
#define CBC_CLIENT_FATAL(...) ::gbc::Logger::GetClientLogger()->critical(__VA_ARGS__)
#else
#define CBC_CORE_TRACE(...)
#define CBC_CORE_DEBUG(...)
#define CBC_CORE_INFO(...)
#define CBC_CORE_WARN(...)
#define CBC_CORE_ERROR(...)
#define CBC_CORE_FATAL(...)

#define CBC_CLIENT_TRACE(...)
#define CBC_CLIENT_DEBUG(...)
#define CBC_CLIENT_INFO(...)
#define CBC_CLIENT_WARN(...)
#define CBC_CLIENT_ERROR(...)
#define CBC_CLIENT_FATAL(...)
#endif
