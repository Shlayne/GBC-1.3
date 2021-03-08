#pragma once

#include <memory>
#include "Core/Window.h"
#include "Core/ImGuiWrapper.h"
#include "Sandbox/Sandbox.h"

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

	// Window needs to be deleted after everything else
	Window* window;
	ImGuiWrapper* imguiWrapper;
	Sandbox* sandbox;
};
