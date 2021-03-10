#pragma once

#include "GBC/Core/Application.h"

#if GBC_PLATFORM_WINDOWS
extern gbc::Application* gbc::CreateApplication();

int main()
{
	auto application = gbc::CreateApplication();
	application->Run();
	delete application;
	return 0;
}
#endif
