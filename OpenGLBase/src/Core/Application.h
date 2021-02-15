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

	inline static Application& get() { return *instance; }
	inline Window& getWindow() const { return *window; }
	inline Sandbox& getSandbox() const { return *sandbox; }

	void run();
	void terminate();
private:
	void onEvent(const Event& event);

	static Application* instance;

	bool running = true;
	bool rendering = true;

	// Window needs to be deleted after everything else
	Window* window;
	ImGuiWrapper* imguiWrapper;
	Sandbox* sandbox;
};
