#include "gbcpch.h"
#include "WindowsWindow.h"
#include <glfw/glfw3.h>
#include "GBC/Core/Application.h"
#include "GBC/Core/Input.h"
#include "GBC/Events/DeviceEvents.h"
#include "GBC/Events/KeyEvents.h"
#include "GBC/Events/MouseEvents.h"
#include "GBC/Events/WindowEvents.h"

namespace gbc
{
	static int glfwWindowCount = 0;
	static bool glfwInitialized = false;

	Ref<Window> Window::CreateRef(const WindowSpecifications& specs)
	{
		auto window = ::gbc::CreateRef<WindowsWindow>(specs);

		if (specs.focusOnShow)
		{
			WindowFocusEvent event(window->GetNativeWindow(), true);
			Application::EventCallback(event);
		}

		return window;
	}

	Scope<Window> Window::CreateScope(const WindowSpecifications& specs)
	{
		auto window = ::gbc::CreateScope<WindowsWindow>(specs);

		if (specs.focusOnShow)
		{
			WindowFocusEvent event(window->GetNativeWindow(), true);
			Application::EventCallback(event);
		}

		return window;
	}

	WindowsWindow::WindowsWindow(const WindowSpecifications& specs)
	{
		GBC_PROFILE_FUNCTION();

		if (!glfwInitialized)
		{
			Input::PreInit();

			int initState = glfwInit();
			GBC_CORE_ASSERT(initState == GLFW_TRUE, "Failed to initialize GLFW!");
			glfwInitialized = true;

			// Error Callback
			glfwSetErrorCallback([](int error, const char* description)
			{
				GBC_CORE_ERROR("GLFW Error (id={0}): {1}", error, description);
			});

			Input::Init();
		}

		state.current.size = { specs.width, specs.height };
		state.title = specs.title;
		state.resizable = specs.resizable;
		state.fullscreen = specs.fullscreen;
		state.focused = specs.focusOnShow;

		context = Context::CreateScope();
		context->PreInit();

		glfwWindowHint(GLFW_RESIZABLE, state.resizable);
		glfwWindowHint(GLFW_FOCUS_ON_SHOW, state.focused);

		GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();

		if (state.fullscreen)
		{
			SaveDimensions();
			state.current.position = { 0, 0 };

			const GLFWvidmode* videoMode = glfwGetVideoMode(primaryMonitor);
			state.current.size = { videoMode->width, videoMode->height };
		}

		window = glfwCreateWindow(state.current.size.x, state.current.size.y, state.title, state.fullscreen ? primaryMonitor : nullptr, nullptr);
		GBC_CORE_ASSERT(window != nullptr, "Failed to create window!");
		glfwWindowCount++;

		if (!state.fullscreen)
		{
			int left, top, right, bottom;
			glfwGetMonitorWorkarea(primaryMonitor, &left, &top, &right, &bottom);
			glm::ivec2 monitorWorkArea(right - left, bottom - top);
			glfwGetWindowFrameSize(window, &left, &top, &right, &bottom);
			glm::ivec2 windowFrameSize(right + left, bottom + top);

			// Calculate the offset needed for the window to be centered in the primary monitor
			state.current.position = glm::ivec2(left, top) + (monitorWorkArea - (state.current.size + windowFrameSize)) / 2;
		}

		glfwGetFramebufferSize(window, &state.framebufferSize.x, &state.framebufferSize.y);
		glfwSetWindowPos(window, state.current.position.x, state.current.position.y);
		glfwSetInputMode(window, GLFW_LOCK_KEY_MODS, GLFW_TRUE);
		glfwSetWindowUserPointer(window, &state);

		context->Init(window);
		SetVSync(specs.vsync);
		SetCaptureMouse(specs.captureMouse);
		SetCallbacks(window);

		glfwShowWindow(window);
	}

	WindowsWindow::~WindowsWindow()
	{
		GBC_PROFILE_FUNCTION();

		glfwDestroyWindow(window);

		if (--glfwWindowCount == 0)
		{
			glfwTerminate();
			glfwInitialized = false;
		}
	}

	void WindowsWindow::PollEvents()
	{
		GBC_PROFILE_FUNCTION();

		// Only call on thread that called glfwInit and once per all windows
		glfwPollEvents();
	}

	void WindowsWindow::SwapBuffers()
	{
		context->SwapBuffers();
	}

	void WindowsWindow::SetTitle(const char* title)
	{
		glfwSetWindowTitle(window, title);
		state.title = title;
	}

	void WindowsWindow::SetIcon(const Ref<LocalTexture2D>& texture)
	{
		GLFWimage image{ texture->GetWidth(), texture->GetHeight(), texture->GetData() };
		glfwSetWindowIcon(window, 1, &image);
	}

	void WindowsWindow::SetVSync(bool vsync)
	{
		glfwSwapInterval(vsync);
		state.vsync = vsync;
	}

	void WindowsWindow::SetResizable(bool resizable)
	{
		glfwSetWindowAttrib(window, GLFW_RESIZABLE, resizable);
		state.resizable = resizable;
	}

	void WindowsWindow::SetFullscreen(bool fullscreen)
	{
		if (fullscreen)
		{
			SaveDimensions();

			// From: https://stackoverflow.com/questions/21421074/how-to-create-a-full-screen-window-on-the-current-monitor-with-glfw
			// Get the monitor that most of the window is on
			int largestOverlap = INT_MIN;
			GLFWmonitor* monitor = nullptr;

			int monitorCount;
			GLFWmonitor** monitors = glfwGetMonitors(&monitorCount);

			for (int i = 0; i < monitorCount; i++)
			{
				const GLFWvidmode* videoMode = glfwGetVideoMode(monitors[i]);

				int monitorX, monitorY;
				glfwGetMonitorPos(monitors[i], &monitorX, &monitorY);

				int overlapX = std::max(0, std::min(state.current.position.x + state.current.size.x, monitorX + videoMode->width) - std::max(state.current.position.x, monitorX));
				int overlapY = std::max(0, std::min(state.current.position.y + state.current.size.y, monitorY + videoMode->height) - std::max(state.current.position.y, monitorY));
				int overlap = overlapX * overlapY;

				if (overlap > largestOverlap)
				{
					largestOverlap = overlap;
					monitor = monitors[i];
				}
			}

			const GLFWvidmode* videoMode = glfwGetVideoMode(monitor);
			glfwSetWindowMonitor(window, monitor, 0, 0, videoMode->width, videoMode->height, videoMode->refreshRate);
		}
		else
			glfwSetWindowMonitor(window, nullptr, state.preFullscreen.position.x, state.preFullscreen.position.y, state.preFullscreen.size.x, state.preFullscreen.size.y, GLFW_DONT_CARE);

		SetVSync(IsVSync());

		state.fullscreen = fullscreen;
	}

	void WindowsWindow::SetCaptureMouse(bool captureMouse)
	{
		glfwSetInputMode(window, GLFW_CURSOR, captureMouse ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
		state.captureMouse = captureMouse;
	}

	void WindowsWindow::SaveDimensions()
	{
		state.preFullscreen.position.x = state.current.position.x;
		state.preFullscreen.position.y = state.current.position.y;
		state.preFullscreen.size.x = state.current.size.x;
		state.preFullscreen.size.y = state.current.size.y;
	}

	// Callbacks

	void WindowsWindow::SetCallbacks(GLFWwindow* window)
	{
		// Window Events
		glfwSetWindowCloseCallback(window, WindowCloseCallback);
		glfwSetWindowSizeCallback(window, WindowSizeCallback);
		glfwSetWindowPosCallback(window, WindowPosCallback);
		glfwSetWindowFocusCallback(window, WindowFocusCallback);
		glfwSetWindowIconifyCallback(window, WindowIconifyCallback);
		glfwSetWindowMaximizeCallback(window, WindowMaximizeCallback);
		glfwSetDropCallback(window, DropCallback);
		glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);
		glfwSetWindowContentScaleCallback(window, WindowContentScaleCallback);
		glfwSetWindowRefreshCallback(window, WindowRefreshCallback);

		// Key Events
		glfwSetKeyCallback(window, KeyCallback);
		glfwSetCharCallback(window, CharCallback);
		glfwSetCharModsCallback(window, CharModsCallback);

		// Mouse Events
		glfwSetMouseButtonCallback(window, MouseButtonCallback);
		glfwSetCursorPosCallback(window, CursorPosCallback);
		glfwSetScrollCallback(window, ScrollCallback);
		glfwSetCursorEnterCallback(window, CursorEnterCallback);
	}

#define BEGIN_EVENT_CALLBACK GLFWwindow* context = glfwGetCurrentContext(); glfwMakeContextCurrent(window)
#define END_EVENT_CALLBACK glfwMakeContextCurrent(context)

	void WindowsWindow::WindowCloseCallback(GLFWwindow* window)
	{
		BEGIN_EVENT_CALLBACK;

		// Since ImGui's glfw implementation doesn't set a window user pointer, this is fine
		if (WindowState* state = static_cast<WindowState*>(glfwGetWindowUserPointer(window)); state != nullptr)
		{
			// Only send a close event if the window is not ImGui's
			WindowCloseEvent event(window);
			Application::EventCallback(event);
		}

		END_EVENT_CALLBACK;
	}

	void WindowsWindow::WindowSizeCallback(GLFWwindow* window, int width, int height)
	{
		BEGIN_EVENT_CALLBACK;

		if (WindowState* state = static_cast<WindowState*>(glfwGetWindowUserPointer(window)); state != nullptr)
			state->current.size = { width, height };

		WindowResizeEvent event(window, width, height);
		Application::EventCallback(event);

		END_EVENT_CALLBACK;
	}

	void WindowsWindow::WindowPosCallback(GLFWwindow* window, int x, int y)
	{
		BEGIN_EVENT_CALLBACK;

		if (WindowState* state = static_cast<WindowState*>(glfwGetWindowUserPointer(window)); state != nullptr)
			state->current.position = { x, y };

		WindowMoveEvent event(window, x, y);
		Application::EventCallback(event);

		END_EVENT_CALLBACK;
	}

	void WindowsWindow::WindowFocusCallback(GLFWwindow* window, int focused)
	{
		BEGIN_EVENT_CALLBACK;

		WindowFocusEvent event(window, focused == GLFW_TRUE);
		if (WindowState* state = static_cast<WindowState*>(glfwGetWindowUserPointer(window)); state != nullptr)
			state->focused = event.IsFocused();
		Application::EventCallback(event);

		END_EVENT_CALLBACK;
	}

	void WindowsWindow::WindowIconifyCallback(GLFWwindow* window, int iconified)
	{
		BEGIN_EVENT_CALLBACK;

		WindowMinimizeEvent event(window, iconified == GLFW_TRUE);
		if (WindowState* state = static_cast<WindowState*>(glfwGetWindowUserPointer(window)); state != nullptr)
			state->minimized = event.IsMinimized();
		Application::EventCallback(event);

		END_EVENT_CALLBACK;
	}

	void WindowsWindow::WindowMaximizeCallback(GLFWwindow* window, int maximized)
	{
		BEGIN_EVENT_CALLBACK;

		WindowMaximizeEvent event(window, maximized == GLFW_TRUE);
		if (WindowState* state = static_cast<WindowState*>(glfwGetWindowUserPointer(window)); state != nullptr)
			state->maximized = event.IsMaximized();
		Application::EventCallback(event);

		END_EVENT_CALLBACK;
	}

	void WindowsWindow::DropCallback(GLFWwindow* window, int pathCount, const char** paths)
	{
		BEGIN_EVENT_CALLBACK;

		WindowDropEvent event(window, pathCount, paths);
		Application::EventCallback(event);

		END_EVENT_CALLBACK;
	}

	void WindowsWindow::FramebufferSizeCallback(GLFWwindow* window, int width, int height)
	{
		BEGIN_EVENT_CALLBACK;

		if (WindowState* state = static_cast<WindowState*>(glfwGetWindowUserPointer(window)); state != nullptr)
			state->framebufferSize = { width, height };
		WindowFramebufferResizeEvent event(window, width, height);
		Application::EventCallback(event);

		END_EVENT_CALLBACK;
	}

	void WindowsWindow::WindowContentScaleCallback(GLFWwindow* window, float scaleX, float scaleY)
	{
		BEGIN_EVENT_CALLBACK;

		WindowContentScaleEvent event(window, scaleX, scaleY);
		Application::EventCallback(event);

		END_EVENT_CALLBACK;
	}

	void WindowsWindow::WindowRefreshCallback(GLFWwindow* window)
	{
		BEGIN_EVENT_CALLBACK;

		WindowRefreshEvent event(window);
		Application::EventCallback(event);

		END_EVENT_CALLBACK;
	}


	void WindowsWindow::KeyCallback(GLFWwindow* window, int keycode, int scancode, int action, int mods)
	{
		BEGIN_EVENT_CALLBACK;

		if (keycode != GLFW_KEY_UNKNOWN)
		{
			switch (action)
			{
				case GLFW_PRESS:
				{
					KeyPressEvent event(static_cast<Keycode>(keycode), mods);
					Application::EventCallback(event);
					break;
				}
				case GLFW_REPEAT:
				{
					KeyRepeatEvent event(static_cast<Keycode>(keycode), mods);
					Application::EventCallback(event);
					break;
				}
				case GLFW_RELEASE:
				{
					KeyReleaseEvent event(static_cast<Keycode>(keycode), mods);
					Application::EventCallback(event);
					break;
				}
			}
		}

		END_EVENT_CALLBACK;
	}

	void WindowsWindow::CharCallback(GLFWwindow* window, uint32_t codepoint)
	{
		BEGIN_EVENT_CALLBACK;

		KeyCharEvent event(codepoint);
		Application::EventCallback(event);

		END_EVENT_CALLBACK;
	}

	void WindowsWindow::CharModsCallback(GLFWwindow* window, uint32_t codepoint, int mods)
	{
		BEGIN_EVENT_CALLBACK;

		KeyCharModsEvent event(codepoint, mods);
		Application::EventCallback(event);

		END_EVENT_CALLBACK;
	}


	void WindowsWindow::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
	{
		BEGIN_EVENT_CALLBACK;

		switch (action)
		{
			case GLFW_PRESS:
			{
				MouseButtonPressEvent event(static_cast<MouseButton>(button), mods);
				Application::EventCallback(event);
				break;
			}
			case GLFW_RELEASE:
			{
				MouseButtonReleaseEvent event(static_cast<MouseButton>(button), mods);
				Application::EventCallback(event);
				break;
			}
		}

		END_EVENT_CALLBACK;
	}

	void WindowsWindow::CursorPosCallback(GLFWwindow* window, double x, double y)
	{
		BEGIN_EVENT_CALLBACK;

		MouseMoveEvent event(static_cast<float>(x), static_cast<float>(y));
		Application::EventCallback(event);

		END_EVENT_CALLBACK;
	}

	void WindowsWindow::ScrollCallback(GLFWwindow* window, double offsetX, double offsetY)
	{
		BEGIN_EVENT_CALLBACK;

		MouseScrollEvent event(static_cast<float>(offsetX), static_cast<float>(offsetY));
		Application::EventCallback(event);

		END_EVENT_CALLBACK;
	}

	void WindowsWindow::CursorEnterCallback(GLFWwindow* window, int entered)
	{
		BEGIN_EVENT_CALLBACK;

		MouseEnterEvent event(entered == GLFW_TRUE);
		if (WindowState* state = static_cast<WindowState*>(glfwGetWindowUserPointer(window)); state != nullptr)
			state->containsMouse = event.HasEntered();
		Application::EventCallback(event);

		END_EVENT_CALLBACK;
	}
}
