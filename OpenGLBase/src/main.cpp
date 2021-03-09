#include "cbcpch.h"
#include "Core/Application.h"

int main()
{
	auto application = new cbc::Application();
	application->Run();
	delete application;
	return 0;
}
