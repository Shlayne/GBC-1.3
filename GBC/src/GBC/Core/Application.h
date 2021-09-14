#pragma once

#include "GBC/Core/LayerStack.h"
#include "GBC/Core/Window.h"
#include "GBC/Events/DeviceEvents.h"
#include "GBC/Events/WindowEvents.h"
#include "GBC/ImGui/ImGuiWrapper.h"

int main(int argc, char** argv);

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

		void Close();
	protected:
		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);
		Layer* PopLayer();
		Layer* PopOverlay();
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
	private:
		void Run();
		friend int ::main(int argc, char** argv);
	};

	Application* CreateApplication();
}
