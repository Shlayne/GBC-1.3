#include "gbcpch.h"
#if GBC_PLATFORM_WINDOWS
#include "WindowsWindow.h"
#include "glfw/glfw3.h"
#include "GBC/Events/WindowEvents.h"
#include "GBC/Events/KeyEvents.h"
#include "GBC/Events/MouseEvents.h"
#include "GBC/Events/DeviceEvents.h"
#include "GBC/Core/Application.h"

namespace gbc
{
	static int glfwWindowCount = 0;
	static bool glfwInitialized = false;

	Ref<Window> Window::CreateRef(const WindowSpecifications& specs)
	{ return ::gbc::CreateRef<WindowsWindow>(specs); }

	Scope<Window> Window::CreateScope(const WindowSpecifications& specs)
	{ return ::gbc::CreateScope<WindowsWindow>(specs); }

	WindowsWindow::WindowsWindow(const WindowSpecifications& specs)
	{
		GBC_PROFILE_FUNCTION();

		if (!glfwInitialized)
		{
			int initState = glfwInit();
			GBC_CORE_ASSERT(initState == GLFW_TRUE, "Failed to initialize GLFW!");
			glfwInitialized = true;

			// Error Callback
			glfwSetErrorCallback([](int error, const char* description)
			{
				GBC_CORE_ERROR("GLFW Error (id={0}): {1}", error, description);
			});

			// Device Events
			glfwSetJoystickCallback([](int jid, int deviceEvent)
			{
				auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
				WindowState& state = *static_cast<WindowState*>(glfwGetWindowUserPointer(window));

				// TODO: joystick (dis)connections should be handled by the engine
				// JoystickConnectEvent should not exist
				JoystickConnectEvent event(jid, deviceEvent == GLFW_CONNECTED);
				state.eventCallback(event);
			});
			glfwSetMonitorCallback([](GLFWmonitor* monitor, int deviceEvent)
			{
				auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
				WindowState& state = *static_cast<WindowState*>(glfwGetWindowUserPointer(window));

				// TODO: monitor (dis)connections should be handled by the engine
				// MonitorConnectEvent should not exist
				MonitorConnectEvent event(deviceEvent == GLFW_CONNECTED);
				state.eventCallback(event);
			});
		}

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
		GBC_CORE_ASSERT(window != nullptr, "Failed to create window!");
		glfwWindowCount++;

		glfwSetWindowPos(window, state.current.x, state.current.y);
		glfwSetInputMode(window, GLFW_LOCK_KEY_MODS, GLFW_TRUE);

		context = Context::CreateScope(window);
		context->Init();

		SetVSync(specs.vsync);
		SetCaptureMouse(specs.captureMouse);

		glfwSetWindowUserPointer(window, &state);

		// Window Events
		glfwSetWindowCloseCallback(window, [](GLFWwindow* window)
		{
			WindowState& state = *static_cast<WindowState*>(glfwGetWindowUserPointer(window));
			WindowCloseEvent event;
			state.eventCallback(event);
		});
		glfwSetWindowSizeCallback(window, [](GLFWwindow* window, int width, int height)
		{
			WindowState& state = *static_cast<WindowState*>(glfwGetWindowUserPointer(window));
			state.current.width = width;
			state.current.height = height;
			WindowResizeEvent event(width, height);
			state.eventCallback(event);
		});
		glfwSetWindowPosCallback(window, [](GLFWwindow* window, int x, int y)
		{
			WindowState& state = *static_cast<WindowState*>(glfwGetWindowUserPointer(window));
			state.current.x = x;
			state.current.y = y;
			WindowMoveEvent event(x, y);
			state.eventCallback(event);
		});
		glfwSetWindowFocusCallback(window, [](GLFWwindow* window, int focused)
		{
			WindowState& state = *static_cast<WindowState*>(glfwGetWindowUserPointer(window));
			WindowFocusEvent event(focused == GLFW_TRUE);
			state.eventCallback(event);
		});
		glfwSetWindowIconifyCallback(window, [](GLFWwindow* window, int iconified)
		{
			WindowState& state = *static_cast<WindowState*>(glfwGetWindowUserPointer(window));
			WindowMinimizeEvent event(iconified == GLFW_TRUE);
			state.eventCallback(event);
		});
		glfwSetWindowMaximizeCallback(window, [](GLFWwindow* window, int maximized)
		{
			WindowState& state = *static_cast<WindowState*>(glfwGetWindowUserPointer(window));
			WindowMaximizeEvent event(maximized == GLFW_TRUE);
			state.eventCallback(event);
		});
		glfwSetDropCallback(window, [](GLFWwindow* window, int pathCount, const char** paths)
		{
			WindowState& state = *static_cast<WindowState*>(glfwGetWindowUserPointer(window));
			WindowDropEvent event(pathCount, paths);
			state.eventCallback(event);
		});
		glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, int width, int height)
		{
			WindowState& state = *static_cast<WindowState*>(glfwGetWindowUserPointer(window));
			WindowFramebufferResizeEvent event(width, height);
			state.eventCallback(event);
		});
		glfwSetWindowContentScaleCallback(window, [](GLFWwindow* window, float scaleX, float scaleY)
		{
			WindowState& state = *static_cast<WindowState*>(glfwGetWindowUserPointer(window));
			WindowContentScaleEvent event(scaleX, scaleY);
			state.eventCallback(event);
		});
		glfwSetWindowRefreshCallback(window, [](GLFWwindow* window)
		{
			WindowState& state = *static_cast<WindowState*>(glfwGetWindowUserPointer(window));
			WindowRefreshEvent event;
			state.eventCallback(event);
		});

		// Key Events
		glfwSetKeyCallback(window, [](GLFWwindow* window, int keycode, int scancode, int action, int mods)
		{
			WindowState& state = *static_cast<WindowState*>(glfwGetWindowUserPointer(window));
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
			WindowState& state = *static_cast<WindowState*>(glfwGetWindowUserPointer(window));
			KeyCharEvent event(codepoint, mods);
			state.eventCallback(event);
		});

		// Mouse Events
		glfwSetMouseButtonCallback(window, [](GLFWwindow* window, int button, int action, int mods)
		{
			WindowState& state = *static_cast<WindowState*>(glfwGetWindowUserPointer(window));
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
			WindowState& state = *static_cast<WindowState*>(glfwGetWindowUserPointer(window));
			MouseMoveEvent event((float)x, (float)y);
			state.eventCallback(event);
		});
		glfwSetScrollCallback(window, [](GLFWwindow* window, double offsetX, double offsetY)
		{
			WindowState& state = *static_cast<WindowState*>(glfwGetWindowUserPointer(window));
			MouseScrollEvent event((float)offsetX, (float)offsetY);
			state.eventCallback(event);
		});
		glfwSetCursorEnterCallback(window, [](GLFWwindow* window, int entered)
		{
			WindowState& state = *static_cast<WindowState*>(glfwGetWindowUserPointer(window));
			MouseEnterEvent event(entered == GLFW_TRUE);
			state.eventCallback(event);
		});

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

	void WindowsWindow::SetVSync(bool vsync)
	{
		glfwSwapInterval(vsync ? 1 : 0);
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
			glfwSetWindowMonitor(window, monitor, 0, 0, videoMode->width, videoMode->height, videoMode->refreshRate);
		}
		else
			glfwSetWindowMonitor(window, nullptr, state.preFullscreen.x, state.preFullscreen.y, state.preFullscreen.width, state.preFullscreen.height, GLFW_DONT_CARE);

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
		state.preFullscreen.x = state.current.x;
		state.preFullscreen.y = state.current.y;
		state.preFullscreen.width = state.current.width;
		state.preFullscreen.height = state.current.height;
	}
}
#endif
