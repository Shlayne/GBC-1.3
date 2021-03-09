#include "Core/Application.h"

int main()
{
	cbc::Application* application = new cbc::Application();
	application->Run();
	delete application;
	return 0;
}
