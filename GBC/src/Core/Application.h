#pragma once

#include "Core/Window.h"
#include "Core/ImGuiWrapper.h"
#include "Sandbox/Sandbox.h"

namespace gbc
{
	class Application
	{
	public:
		Application(const WindowSpecifications& windowSpecs = WindowSpecifications());
		~Application();

		inline static Application& Get() { return *instance; }
		inline Window& GetWindow() const { return *window; }
		inline Sandbox& GetSandbox() const { return *sandbox; }

		void Run();
		void Terminate();
	private:
		void OnEvent(Event& event);

		static Application* instance;

		bool running = true;
		bool rendering = true;

		Scope<Window> window;
		Scope<ImGuiWrapper> imguiWrapper;
		Scope<Sandbox> sandbox;
	};
}
