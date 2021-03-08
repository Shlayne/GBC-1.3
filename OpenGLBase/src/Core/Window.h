#pragma once

#include <functional>
#include "Context.h"
#include "Events/Event.h"

struct WindowSpecifications
{
	int width = 1280;
	int height = 720;
	const char* title = "OpenGL Base";
	bool vsync = true;
	bool resizable = true;
	bool fullscreen = false;
	bool captureMouse = false;
};

struct GLFWwindow;
class Window
{
public:
	Window(const WindowSpecifications& specs = WindowSpecifications());
	~Window();

	void PollEvents();
	void SwapBuffers();
	
	inline int GetWidth() const { return state.current.width; }
	inline int GetHeight() const { return state.current.height; }

	const char* GetTitle() const { return state.title; }
	void SetTitle(const char* title);

	inline bool GetVSync() const { return state.vsync; }
	void SetVSync(bool vsync);
	inline void ToggleVSync() { SetVSync(!GetVSync()); }

	inline bool GetResizable() const { return state.resizable; }
	void SetResizable(bool resizable);
	inline void ToggleResizable() { SetResizable(!GetResizable()); }

	inline bool GetFullscreen() const { return state.fullscreen; }
	void SetFullscreen(bool fullscreen);
	inline void ToggleFullscreen() { SetFullscreen(!GetFullscreen()); }

	inline bool GetCaptureMouse() const { return state.captureMouse; }
	void SetCaptureMouse(bool captureMouse);
	inline void ToggleCaptureMouse() { SetCaptureMouse(!GetCaptureMouse()); }

	inline GLFWwindow* GetNativeWindow() const { return window; }
	inline Context* GetContext() const { return context; }

	using EventCallbackFunc = std::function<void(Event&)>;
	inline void SetEventCallback(const EventCallbackFunc& callback) { state.eventCallback = callback; }
private:
	void SaveDimensions();

	GLFWwindow* window = nullptr;
	Context* context = nullptr;

	struct WindowState
	{
		struct
		{
			int x = 0;
			int y = 0;
			int width = 0;
			int height = 0;
		} current, preFullscreen;

		const char* title = nullptr;
		bool vsync = false;
		bool resizable = false;
		bool fullscreen = false;
		bool captureMouse = false;

		EventCallbackFunc eventCallback;
	} state;
};
