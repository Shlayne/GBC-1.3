#include "Core/Application.h"

int main()
{
	auto application = new gbc::Application();
	application->Run();
	delete application;
	return 0;
}
