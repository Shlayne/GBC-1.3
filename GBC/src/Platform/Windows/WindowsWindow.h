#pragma once

#if GBC_PLATFORM_WINDOWS
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

			// TODO: Should all be updated upon window creation
			bool focused = true;
			bool minimized = false;
			bool maximized = false;
			bool containsMouse = false;

			EventCallbackFunc eventCallback;
		} state;
	};
}
#endif
