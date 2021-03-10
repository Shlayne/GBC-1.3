#include "gbcpch.h"
#include "Application.h"
#include "Rendering/Renderer.h"
#include "Events/WindowEvents.h"

namespace gbc
{
	Application* Application::instance = nullptr;

	Application::Application(const WindowSpecifications& windowSpecs)
	{
		instance = this;
		Logger::Init();

		window = Window::CreateScope(windowSpecs);
		window->SetEventCallback(CBC_BIND_FUNC(OnEvent));

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
				WindowResizeEvent& wre = (WindowResizeEvent&)event;
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
				WindowMinimizeEvent& wme = (WindowMinimizeEvent&)event;
				rendering = !wme.IsMinimized();
				break;
			}
		}

		if (!event.IsHandled())
			sandbox->OnEvent(event);
	}
}
