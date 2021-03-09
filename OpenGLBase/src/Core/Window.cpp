#include "Window.h"
#include "Platform/Windows/WindowsWindow.h"

Scope<Window> Window::Create(const WindowSpecifications& specs)
{
	return CreateScope<WindowsWindow>(specs);
}
