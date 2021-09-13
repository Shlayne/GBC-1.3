#pragma once

#include "GBC/Core/Application.h"
#include "GBC/Core/Logger.h"
#include "GBC/Debug/Profiler.h"

#if GBC_PLATFORM_WINDOWS

// TODO: Do this with premake if possible
// filter { "configuration:Dist", "system:windows" } entrypoint "mainCRTStartup"
// But I don't know how to do /SUBSYSTEM:windows
#if !GBC_ENABLE_LOGGING
	#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif

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
