#include "Window.h"
#include "Rendering/RendererAPI.h"
#include "Platform/Windows/WindowsWindow.h"

namespace cbc
{
	Ref<Window> Window::CreateRef(const WindowSpecifications& specs)
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::None:   return nullptr;
			case RendererAPI::API::OpenGL: return cbc::CreateRef<WindowsWindow>(specs);
		}
		
		CBC_ASSERT(false, "Unknown Renderer API!");
		return nullptr;
	}

	Scope<Window> Window::CreateScope(const WindowSpecifications& specs)
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::None:   return nullptr;
			case RendererAPI::API::OpenGL: return cbc::CreateScope<WindowsWindow>(specs);
		}

		CBC_ASSERT(false, "Unknown Renderer API!");
		return nullptr;
	}
}