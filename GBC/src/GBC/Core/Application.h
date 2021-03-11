#pragma once

#include "Window.h"
#include "ImGuiWrapper.h"
#include "Timestep.h"
#include "GBC/Events/WindowEvents.h"
#include "LayerStack.h"

namespace gbc
{
	class Application
	{
	public:
		Application(const WindowSpecifications& windowSpecs = WindowSpecifications());
		virtual ~Application();

		inline static Application& Get() { return *instance; }
		inline Window& GetWindow() const { return *window; }

		void Run();
		void Terminate();
	protected:
		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);
		void PopLayer(Layer* layer);
		void PopOverlay(Layer* overlay);
	private:
		bool OnWindowCloseEvent(WindowCloseEvent& event);
		bool OnWindowResizeEvent(WindowResizeEvent& event);
		bool OnWindowMinimizeEvent(WindowMinimizeEvent& event);
		void OnEvent(Event& event);

		static Application* instance;

		bool running = true;
		bool rendering = true;

		Scope<Window> window;
		Scope<ImGuiWrapper> imguiWrapper;
		LayerStack layerStack;
	};

	Application* CreateApplication();
}
