#pragma once

#include <functional>
#include "Core/Core.h"
#include "Events/Event.h"
#include "Context.h"

namespace cbc
{
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

	class Window
	{
	public:
		virtual ~Window() = default;

		virtual void PollEvents() = 0;
		virtual void SwapBuffers() = 0;

		virtual int GetWidth() const = 0;
		virtual int GetHeight() const = 0;

		virtual const char* GetTitle() const = 0;
		virtual void SetTitle(const char* title) = 0;

		virtual bool IsVSync() const = 0;
		virtual void SetVSync(bool vsync) = 0;

		virtual bool IsResizable() const = 0;
		virtual void SetResizable(bool resizable) = 0;

		virtual bool IsFullscreen() const = 0;
		virtual void SetFullscreen(bool fullscreen) = 0;

		virtual bool IsCaptureMouse() const = 0;
		virtual void SetCaptureMouse(bool captureMouse) = 0;

		virtual void* GetNativeWindow() const = 0;
		virtual Context& GetContext() const = 0;

		using EventCallbackFunc = std::function<void(Event&)>;
		virtual void SetEventCallback(const EventCallbackFunc& callback) = 0;

		static Ref<Window> CreateRef(const WindowSpecifications& specs = WindowSpecifications());
		static Scope<Window> CreateScope(const WindowSpecifications& specs = WindowSpecifications());
	};
}
