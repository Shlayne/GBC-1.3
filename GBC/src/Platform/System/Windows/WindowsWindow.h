#pragma once

#include "GBC/Core/Window.h"

struct GLFWwindow;

namespace gbc
{
	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowSpecifications& specs);
		virtual ~WindowsWindow();

		virtual void PollEvents() override;
		virtual void SwapBuffers() override;

		virtual int GetWidth() const override { return state.current.width; }
		virtual int GetHeight() const override { return state.current.height; }

		virtual bool IsFocused() const override { return state.focused; };
		virtual bool ContainsMouse() const override { return state.containsMouse; }
		virtual bool IsMinimized() const override { return state.minimized; }
		virtual bool IsMaximized() const override { return state.maximized; }

		virtual const char* GetTitle() const override { return state.title; }
		virtual void SetTitle(const char* title) override;

		virtual bool IsVSync() const override { return state.vsync; }
		virtual void SetVSync(bool vsync) override;

		virtual bool IsResizable() const override { return state.resizable; }
		virtual void SetResizable(bool resizable) override;

		virtual bool IsFullscreen() const override { return state.fullscreen; }
		virtual void SetFullscreen(bool fullscreen) override;

		virtual bool IsCaptureMouse() const override { return state.captureMouse; }
		virtual void SetCaptureMouse(bool captureMouse) override;

		virtual void* GetNativeWindow() override { return window; }
		virtual Context& GetContext() override { return *context; }

		virtual void SetEventCallback(const EventCallbackFunc& callback) override { state.eventCallback = callback; }
	private:
		void SaveDimensions();

		GLFWwindow* window = nullptr;
		Scope<Context> context = nullptr;

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

			bool focused = true;
			bool minimized = false;
			bool maximized = false;
			bool containsMouse = false;

			EventCallbackFunc eventCallback;
		} state;

		// Callbacks
		static void SetCallbacks(GLFWwindow* window);

		static void WindowCloseCallback(GLFWwindow* window);
		static void WindowSizeCallback(GLFWwindow* window, int width, int height);
		static void WindowPosCallback(GLFWwindow* window, int x, int y);
		static void WindowFocusCallback(GLFWwindow* window, int focused);
		static void WindowIconifyCallback(GLFWwindow* window, int iconified);
		static void WindowMaximizeCallback(GLFWwindow* window, int maximized);
		static void DropCallback(GLFWwindow* window, int pathCount, const char** paths);
		static void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
		static void WindowContentScaleCallback(GLFWwindow* window, float scaleX, float scaleY);
		static void WindowRefreshCallback(GLFWwindow* window);

		static void KeyCallback(GLFWwindow* window, int keycode, int scancode, int action, int mods);
		static void CharCallback(GLFWwindow* window, uint32_t codepoint);
		static void CharModsCallback(GLFWwindow* window, uint32_t codepoint, int mods);

		static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
		static void CursorPosCallback(GLFWwindow* window, double x, double y);
		static void ScrollCallback(GLFWwindow* window, double offsetX, double offsetY);
		static void CursorEnterCallback(GLFWwindow* window, int entered);
	};
}
