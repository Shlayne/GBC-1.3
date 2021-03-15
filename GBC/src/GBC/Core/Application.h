#pragma once

#include "Window.h"
#include "GBC/ImGui/ImGuiLayer.h"
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
		inline Window& GetWindow() { return *window; }
#if GBC_ENABLE_IMGUI
		inline ImGuiLayer& GetImGuiLayer() { return *imguiLayer; }
#endif

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
#if GBC_ENABLE_IMGUI
		ImGuiLayer* imguiLayer;
#endif
		LayerStack layerStack;
	};

	Application* CreateApplication();
}
