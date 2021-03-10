#pragma once

#include "GBC/Core/Window.h"
#include "GBC/Core/ImGuiWrapper.h"

namespace gbc
{
	class Application
	{
	public:
		Application(const WindowSpecifications& windowSpecs = WindowSpecifications());
		~Application();

		inline static Application& Get() { return *instance; }
		inline Window& GetWindow() const { return *window; }

		void Run();
		void Terminate();

		virtual void OnClientUpdate(float deltaTime) = 0;
		virtual void OnClientRender() = 0;
		virtual void OnClientImGuiRender() = 0;
		virtual void OnClientEvent(Event& event) = 0;
	private:
		void OnEvent(Event& event);

		static Application* instance;

		bool running = true;
		bool rendering = true;

		Scope<Window> window;
		Scope<ImGuiWrapper> imguiWrapper;
	};

	Application* CreateApplication();
}
