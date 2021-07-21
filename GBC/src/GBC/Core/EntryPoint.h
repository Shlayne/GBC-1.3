#pragma once

#include "Application.h"
#include "Logger.h"
#include "GBC/Debug/Profiler.h"
#include "GBC/Rendering/RendererAPI.h"

#if GBC_PLATFORM_WINDOWS
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
