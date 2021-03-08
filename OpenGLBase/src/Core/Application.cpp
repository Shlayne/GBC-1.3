#include "Application.h"
#include "Rendering/Renderer.h"
#include "Events/WindowEvents.h"

#define BIND_FUNC(func) [this](auto&&... args) -> decltype(auto) { return this->func(std::forward<decltype(args)>(args)...); }

Application* Application::instance = nullptr;

Application::Application(const WindowSpecifications& windowSpecs)
{
	instance = this;

	window = new Window(windowSpecs);
	window->SetEventCallback(BIND_FUNC(OnEvent));

	imguiWrapper = new ImGuiWrapper(*window);

	Renderer::Init();

	sandbox = new Sandbox();
}

Application::~Application()
{
	delete sandbox;
	delete imguiWrapper;

	Renderer::Shutdown();
	delete window;
}

void Application::Run()
{
	while (running)
	{
		float deltaTime = window->GetContext()->GetElapsedTime();
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
				Renderer::OnViewportResized(wre.GetWidth(), wre.GetHeight());
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
