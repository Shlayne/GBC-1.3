#include "gbcpch.h"
#if CBC_PLATFORM_WINDOWS
#include "WindowsWindow.h"
#include "glfw/glfw3.h"
#include "Events/WindowEvents.h"
#include "Events/KeyEvents.h"
#include "Events/MouseEvents.h"
//#include "Events/DeviceEvents.h"

namespace gbc
{
	Ref<Window> Window::CreateRef(const WindowSpecifications& specs)
	{ return gbc::CreateRef<WindowsWindow>(specs); }

	Scope<Window> Window::CreateScope(const WindowSpecifications& specs)
	{ return gbc::CreateScope<WindowsWindow>(specs); }

	WindowsWindow::WindowsWindow(const WindowSpecifications& specs)
	{
		int initState = glfwInit();
		CBC_ASSERT(initState == GLFW_TRUE, "Failed to initialize GLFW!");

		state.current.width = specs.width;
		state.current.height = specs.height;
		state.title = specs.title;
		state.resizable = specs.resizable;
		state.fullscreen = specs.fullscreen;

		// TODO: get these from specifications
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		glfwWindowHint(GLFW_RESIZABLE, state.resizable);

		GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
		const GLFWvidmode* videoMode = glfwGetVideoMode(primaryMonitor);

		// Calculate the offset needed for the window to be centered in the primary monitor
		state.current.x = (videoMode->width - state.current.width) / 2;
		state.current.y = (videoMode->height - state.current.height) / 2;

		if (state.fullscreen)
		{
			SaveDimensions();
			state.current.x = 0;
			state.current.y = 0;
			state.current.width = videoMode->width;
			state.current.height = videoMode->height;
		}

		window = glfwCreateWindow(state.current.width, state.current.height, state.title, state.fullscreen ? primaryMonitor : nullptr, nullptr);
		CBC_ASSERT(window != nullptr, "Failed to create window!");

		glfwSetWindowPos(window, state.current.x, state.current.y);
		glfwSetInputMode(window, GLFW_LOCK_KEY_MODS, GLFW_TRUE);

		context = gbc::CreateScope<Context>(window);

		SetVSync(specs.vsync);
		SetCaptureMouse(specs.captureMouse);

		glfwSetWindowUserPointer(window, &state);

		// Window Events
		glfwSetWindowCloseCallback(window, [](GLFWwindow* window)
		{
			auto& state = *static_cast<WindowState*>(glfwGetWindowUserPointer(window));
			WindowCloseEvent event;
			state.eventCallback(event);
		});
		glfwSetWindowSizeCallback(window, [](GLFWwindow* window, int width, int height)
		{
			auto& state = *static_cast<WindowState*>(glfwGetWindowUserPointer(window));
			state.current.width = width;
			state.current.height = height;
			WindowResizeEvent event(width, height);
			state.eventCallback(event);
		});
		glfwSetWindowPosCallback(window, [](GLFWwindow* window, int x, int y)
		{
			auto& state = *static_cast<WindowState*>(glfwGetWindowUserPointer(window));
			state.current.x = x;
			state.current.y = y;
			WindowMoveEvent event(x, y);
			state.eventCallback(event);
		});
		glfwSetWindowFocusCallback(window, [](GLFWwindow* window, int focused)
		{
			auto& state = *static_cast<WindowState*>(glfwGetWindowUserPointer(window));
			WindowFocusEvent event(focused == GLFW_TRUE);
			state.eventCallback(event);
		});
		glfwSetWindowIconifyCallback(window, [](GLFWwindow* window, int iconified)
		{
			auto& state = *static_cast<WindowState*>(glfwGetWindowUserPointer(window));
			WindowMinimizeEvent event(iconified == GLFW_TRUE);
			state.eventCallback(event);
		});
		glfwSetWindowMaximizeCallback(window, [](GLFWwindow* window, int maximized)
		{
			auto& state = *static_cast<WindowState*>(glfwGetWindowUserPointer(window));
			WindowMaximizeEvent event(maximized == GLFW_TRUE);
			state.eventCallback(event);
		});
		glfwSetDropCallback(window, [](GLFWwindow* window, int pathCount, const char** paths)
		{
			auto& state = *static_cast<WindowState*>(glfwGetWindowUserPointer(window));
			WindowDropEvent event(pathCount, paths);
			state.eventCallback(event);
		});
		glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, int width, int height)
		{
			auto& state = *static_cast<WindowState*>(glfwGetWindowUserPointer(window));
			WindowFramebufferResizeEvent event(width, height);
			state.eventCallback(event);
		});
		glfwSetWindowContentScaleCallback(window, [](GLFWwindow* window, float scaleX, float scaleY)
		{
			auto& state = *static_cast<WindowState*>(glfwGetWindowUserPointer(window));
			WindowContentScaleEvent event(scaleX, scaleY);
			state.eventCallback(event);
		});
		glfwSetWindowRefreshCallback(window, [](GLFWwindow* window)
		{
			auto& state = *static_cast<WindowState*>(glfwGetWindowUserPointer(window));
			WindowRefreshEvent event;
			state.eventCallback(event);
		});

		// Key Events
		glfwSetKeyCallback(window, [](GLFWwindow* window, int keycode, int scancode, int action, int mods)
		{
			auto& state = *static_cast<WindowState*>(glfwGetWindowUserPointer(window));
			if (keycode != GLFW_KEY_UNKNOWN)
			{
				switch (action)
				{
				case GLFW_PRESS:
				{
					KeyPressEvent event(static_cast<Keycode>(keycode), mods);
					state.eventCallback(event);
					break;
				}
				case GLFW_REPEAT:
				{
					KeyRepeatEvent event(static_cast<Keycode>(keycode), mods);
					state.eventCallback(event);
					break;
				}
				case GLFW_RELEASE:
				{
					KeyReleaseEvent event(static_cast<Keycode>(keycode), mods);
					state.eventCallback(event);
					break;
				}
				}
			}
		});
		// GLFW has a bug where this doesn't get called when ctrl is pressed
		glfwSetCharModsCallback(window, [](GLFWwindow* window, unsigned int codepoint, int mods)
		{
			auto& state = *static_cast<WindowState*>(glfwGetWindowUserPointer(window));
			KeyCharEvent event(codepoint, mods);
			state.eventCallback(event);
		});

		// Mouse Events
		glfwSetMouseButtonCallback(window, [](GLFWwindow* window, int button, int action, int mods)
		{
			auto& state = *static_cast<WindowState*>(glfwGetWindowUserPointer(window));
			switch (action)
			{
			case GLFW_PRESS:
			{
				MouseButtonPressEvent event(static_cast<Mousecode>(button), mods);
				state.eventCallback(event);
				break;
			}
			case GLFW_RELEASE:
			{
				MouseButtonReleaseEvent event(static_cast<Mousecode>(button), mods);
				state.eventCallback(event);
				break;
			}
			}
		});
		glfwSetCursorPosCallback(window, [](GLFWwindow* window, double x, double y)
		{
			auto& state = *static_cast<WindowState*>(glfwGetWindowUserPointer(window));
			MouseMoveEvent event((float)x, (float)y);
			state.eventCallback(event);
		});
		glfwSetScrollCallback(window, [](GLFWwindow* window, double offsetX, double offsetY)
		{
			auto& state = *static_cast<WindowState*>(glfwGetWindowUserPointer(window));
			MouseScrollEvent event((float)offsetX, (float)offsetY);
			state.eventCallback(event);
		});
		glfwSetCursorEnterCallback(window, [](GLFWwindow* window, int entered)
		{
			auto& state = *static_cast<WindowState*>(glfwGetWindowUserPointer(window));
			MouseEnterEvent event(entered == GLFW_TRUE);
			state.eventCallback(event);
		});

		//// Device Events
		//glfwSetJoystickCallback([](int jid, int event)
		//{
		//	// Big brain hacks
		//	auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		//	auto& state = *static_cast<WindowState*>(glfwGetWindowUserPointer(window));

		//	JoystickConnectEvent event(jid, event == GLFW_CONNECTED);
		//	state.eventCallback(event);
		//});

		glfwShowWindow(window);
	}

	WindowsWindow::~WindowsWindow()
	{
		glfwDestroyWindow(window);
		glfwTerminate();
	}

	void WindowsWindow::PollEvents()
	{
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

	void WindowsWindow::SetVSync(bool vsync)
	{
		glfwSwapInterval((int)vsync);
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

				int overlapX = std::max(0, std::min(state.current.x + state.current.width, monitorX + videoMode->width) - std::max(state.current.x, monitorX));
				int overlapY = std::max(0, std::min(state.current.y + state.current.height, monitorY + videoMode->height) - std::max(state.current.y, monitorY));
				int overlap = overlapX * overlapY;

				if (overlap > largestOverlap)
				{
					largestOverlap = overlap;
					monitor = monitors[i];
				}
			}

			const GLFWvidmode* videoMode = glfwGetVideoMode(monitor);
			glfwSetWindowMonitor(window, monitor, 0, 0, videoMode->width, videoMode->height, GLFW_DONT_CARE);

			SetVSync(IsVSync());
		}
		else
			glfwSetWindowMonitor(window, nullptr, state.preFullscreen.x, state.preFullscreen.y, state.preFullscreen.width, state.preFullscreen.height, GLFW_DONT_CARE);

		state.fullscreen = fullscreen;
	}

	void WindowsWindow::SetCaptureMouse(bool captureMouse)
	{
		glfwSetInputMode(window, GLFW_CURSOR, captureMouse ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
		state.captureMouse = captureMouse;
	}

	void WindowsWindow::SaveDimensions()
	{
		state.preFullscreen.x = state.current.x;
		state.preFullscreen.y = state.current.y;
		state.preFullscreen.width = state.current.width;
		state.preFullscreen.height = state.current.height;
	}
}
#endif
