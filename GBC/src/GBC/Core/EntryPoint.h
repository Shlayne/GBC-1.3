#pragma once

#include "Application.h"
#include "Logger.h"
#include "GBC/Debug/Profiler.h"
#include "GBC/Rendering/RendererAPI.h"

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
	gbc::RendererAPI::SetAPI(gbc::RendererAPI::API::OpenGL);
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
