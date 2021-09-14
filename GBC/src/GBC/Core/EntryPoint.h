#pragma once

#include "GBC/Core/Application.h"
#include "GBC/Core/Logger.h"
#include "GBC/Debug/Profiler.h"

#if GBC_PLATFORM_WINDOWS

extern gbc::Application* gbc::CreateApplication();

int main(int argc, char** argv)
{
	gbc::Logger::Init();

	GBC_PROFILE_BEGIN("Init", "GBCProfileInit.json");
	auto application = gbc::CreateApplication();
	GBC_PROFILE_END();

	GBC_PROFILE_BEGIN("Run", "GBCProfileRun.json");
	application->Run();
	GBC_PROFILE_END();

	GBC_PROFILE_BEGIN("Shutdown", "GBCProfileShutdown.json");
	delete application;
	GBC_PROFILE_END();

	return 0;
}
#endif
