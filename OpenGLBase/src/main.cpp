#include "Core/Application.h"

int main()
{
	Application* application = new Application();
	application->Run();
	delete application;
	return 0;
}
