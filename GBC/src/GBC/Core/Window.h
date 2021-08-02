#pragma once

#include <functional>
#include "Core.h"
#include "GBC/Events/Event.h"
#include "GBC/Rendering/Context.h"
#include "GBC/Rendering/LocalTexture2D.h"

namespace gbc
{
	struct WindowSpecifications
	{
		int width = 1280;
		int height = 720;
		const char* title = "GBC v1.3";
		bool vsync = true;
		bool resizable = true;
		bool fullscreen = false;
		bool captureMouse = false;
		bool focusOnShow = true;
	};

	class Window
	{
	public:
		virtual ~Window() = default;

		virtual void PollEvents() = 0;
		virtual void SwapBuffers() = 0;

		virtual int GetWidth() const = 0;
		virtual int GetHeight() const = 0;

		virtual bool IsFocused() const = 0;
		virtual bool ContainsMouse() const = 0;
		virtual bool IsMinimized() const = 0;
		virtual bool IsMaximized() const = 0;

		virtual const char* GetTitle() const = 0;
		virtual void SetTitle(const char* title) = 0;

		virtual void SetIcon(const Ref<LocalTexture2D>& texture) = 0;

		virtual bool IsVSync() const = 0;
		virtual void SetVSync(bool vsync) = 0;
		inline void ToggleVSync() { SetVSync(!IsVSync()); }

		virtual bool IsResizable() const = 0;
		virtual void SetResizable(bool resizable) = 0;
		inline void ToggleResizable() { SetResizable(!IsResizable()); }

		virtual bool IsFullscreen() const = 0;
		virtual void SetFullscreen(bool fullscreen) = 0;
		inline void ToggleFullscreen() { SetFullscreen(!IsFullscreen()); }

		virtual bool IsCaptureMouse() const = 0;
		virtual void SetCaptureMouse(bool captureMouse) = 0;
		inline void ToggleCaptureMouse() { SetCaptureMouse(!IsCaptureMouse()); }

		virtual void* GetNativeWindow() = 0;
		virtual Context& GetContext() = 0;

		static Ref<Window> CreateRef(const WindowSpecifications& specs);
		static Scope<Window> CreateScope(const WindowSpecifications& specs);
	};
}
