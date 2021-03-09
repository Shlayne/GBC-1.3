#include "Application.h"
#include "Rendering/Renderer.h"
#include "Events/WindowEvents.h"

#define BIND_FUNC(func) [this](auto&&... args) -> decltype(auto) { return this->func(std::forward<decltype(args)>(args)...); }

Application* Application::instance = nullptr;

Application::Application(const WindowSpecifications& windowSpecs)
{
	instance = this;

	window = Window::Create(windowSpecs);
	window->SetEventCallback(BIND_FUNC(OnEvent));

	imguiWrapper = CreateScope<ImGuiWrapper>(window->GetNativeWindow());
	Renderer::Init();
	sandbox = CreateScope<Sandbox>();
}

Application::~Application()
{
	Renderer::Shutdown();
}

void Application::Run()
{
	while (running)
	{
		float deltaTime = window->GetContext().GetElapsedTime();
		sandbox->OnUpdate(deltaTime);

		if (rendering)
		{
			sandbox->OnRender();
			imguiWrapper->Begin();
			sandbox->OnImGuiRender();
			imguiWrapper->End();
		}

		window->PollEvents();

		if (rendering)
			window->SwapBuffers();
	}
}

void Application::Terminate()
{
	running = false;
}

void Application::OnEvent(Event& event)
{
	switch (event.GetType())
	{
		case EventType::WindowClose:
		{
			Terminate();
			event.Handle();
			break;
		}
		case EventType::WindowResize:
		{
			const WindowResizeEvent& wre = (const WindowResizeEvent&)event;
			if (wre.GetWidth() == 0 || wre.GetHeight() == 0)
			{
				rendering = false;
				event.Handle();
			}
			else
			{
				rendering = true;
				Renderer::SetViewport(0, 0, wre.GetWidth(), wre.GetHeight());
			}
			break;
		}
		case EventType::WindowMinimize:
		{
			const WindowMinimizeEvent& wme = (const WindowMinimizeEvent&)event;
			rendering = !wme.IsMinimized();
			break;
		}
	}

	if (!event.IsHandled())
		sandbox->OnEvent(event);
}
