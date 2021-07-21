#include "gbcpch.h"
#include "Application.h"
#include "GBC/Rendering/Renderer.h"
#include "GBC/Events/WindowEvents.h"
#include "Timestep.h"

namespace gbc
{
	Application* Application::instance = nullptr;

	Application::Application(const WindowSpecifications& windowSpecs)
	{
		GBC_PROFILE_FUNCTION();

		GBC_CORE_ASSERT(instance == nullptr, "Application already exists!");
		instance = this;

		window = Window::CreateScope(windowSpecs);
		window->SetEventCallback(GBC_BIND_FUNC(OnEvent));

		Renderer::Init();

#if GBC_ENABLE_IMGUI
		imguiWrapper = new ImGuiWrapper();
#endif
	}

	Application::~Application()
	{
		GBC_PROFILE_FUNCTION();

		delete imguiWrapper;

		for (Layer* layer : layerStack)
		{
			layer->OnDetach();
			delete layer;
		}

		Renderer::Shutdown();
	}

	void Application::Run()
	{
		Timestep timestep;

		while (running)
		{
			GBC_PROFILE_SCOPE("RunLoop");

			timestep = window->GetContext().GetElapsedTime();

			for (Layer* layer : layerStack)
				layer->OnUpdate(timestep);

			if (rendering)
			{
				for (Layer* layer : layerStack)
					layer->OnRender();

#if GBC_ENABLE_IMGUI
				imguiWrapper->Begin();
				for (Layer* layer : layerStack)
					layer->OnImGuiRender();
				imguiWrapper->End();
#endif

				window->SwapBuffers();
			}

			window->PollEvents();
		}
	}

	void Application::Close()
	{
		running = false;
	}

	void Application::PushLayer(Layer* layer)
	{
		layerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* overlay)
	{
		layerStack.PushOverlay(overlay);
		overlay->OnAttach();
	}

	Layer* Application::PopLayer()
	{
		Layer* layer = layerStack.PopLayer();
		if (layer)
			layer->OnDetach();
		return layer;
	}

	Layer* Application::PopOverlay()
	{
		Layer* overlay = layerStack.PopOverlay();
		if (overlay)
			overlay->OnDetach();
		return overlay;
	}

	void Application::OnEvent(Event& event)
	{
		GBC_PROFILE_FUNCTION();

		EventDispatcher dispatcher(event);
		dispatcher.Dispatch(this, &Application::OnWindowResizeEvent);
		dispatcher.Dispatch(this, &Application::OnWindowMinimizeEvent);
		dispatcher.Dispatch(this, &Application::OnJoystickConnectEvent);
		dispatcher.Dispatch(this, &Application::OnMonitorConnectEvent);

		imguiWrapper->OnEvent(event);

		for (auto it = layerStack.rbegin(); !event.handled && it != layerStack.rend(); ++it)
			(*it)->OnEvent(event);

		// Let the client handle window close events if they want to.
		if (!event.handled)
			dispatcher.Dispatch(this, &Application::OnWindowCloseEvent);
	}

	bool Application::OnWindowResizeEvent(WindowResizeEvent& event)
	{
		GBC_PROFILE_FUNCTION();

		if (event.GetWidth() == 0 || event.GetHeight() == 0)
		{
			rendering = false;
			return true;
		}
		else
		{
			rendering = true;
			Renderer::SetViewport(0, 0, event.GetWidth(), event.GetHeight());
			return false;
		}
	}

	bool Application::OnWindowMinimizeEvent(WindowMinimizeEvent& event)
	{
		rendering = !event.IsMinimized();
		return false;
	}

	bool Application::OnJoystickConnectEvent(JoystickConnectEvent& event)
	{
		GBC_CORE_INFO("Joystick Connect Event: jid={0}, connected={1}", event.GetJID(), event.IsConnected());
		return false;
	}

	bool Application::OnMonitorConnectEvent(MonitorConnectEvent& event)
	{
		GBC_CORE_INFO("Monitor Connect Event: connected={0}", event.IsConnected());
		return false;
	}

	bool Application::OnWindowCloseEvent(WindowCloseEvent& event)
	{
		Close();
		return true;
	}
}
