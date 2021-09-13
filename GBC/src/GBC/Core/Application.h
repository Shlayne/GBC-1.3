#pragma once

#include "GBC/Core/LayerStack.h"
#include "GBC/Core/Window.h"
#include "GBC/Events/DeviceEvents.h"
#include "GBC/Events/WindowEvents.h"
#include "GBC/ImGui/ImGuiWrapper.h"

namespace gbc
{
	class Application
	{
	public:
		Application(const WindowSpecifications& windowSpecs = {});
		virtual ~Application();

		inline static Application& Get() { return *instance; }
		inline Window& GetWindow() { return *window; }
		inline void* GetFocusedNativeWindow() { return focusedNativeWindow; }
#if GBC_ENABLE_IMGUI
		inline ImGuiWrapper& GetImGuiWrapper() { return *imguiWrapper; }
#endif

		void Run();
		void Close();
	protected:
		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);
		Layer* PopLayer();
		Layer* PopOverlay();
	public:
		using EventCallbackFunc = std::function<void(Event&)>;
		static void EventCallback(Event& event);
	private:
		void OnEvent(Event& event);
		bool OnWindowCloseEvent(WindowCloseEvent& event);
		bool OnWindowFramebufferResizeEvent(WindowFramebufferResizeEvent& event);
		bool OnWindowFocusEvent(WindowFocusEvent& event);
		bool OnWindowMinimizeEvent(WindowMinimizeEvent& event);

		static Application* instance;

		bool running = true;
		bool rendering = true;
		bool windowZeroSize = false;

		Scope<Window> window;
		void* focusedNativeWindow = nullptr;
#if GBC_ENABLE_IMGUI
		ImGuiWrapper* imguiWrapper;
#endif
		LayerStack layerStack;
	};

	Application* CreateApplication();
}
