#pragma once

#include "GBC/Events/Event.h"

namespace gbc
{
	class WindowEvent : public Event
	{
	public:
		WindowEvent(void* nativeWindow);

		inline void* GetNativeWindow() const noexcept { return nativeWindow; }
	private:
		void* nativeWindow;
	};

	class WindowCloseEvent : public WindowEvent
	{
	public:
		WindowCloseEvent(void* nativeWindow);
		EVENT_CLASS_TYPE(EventType::WindowClose);
		EVENT_CLASS_CATEGORY(EventCategory_Window | EventCategory_Application);

#if GBC_ENABLE_LOGGING
		virtual std::string ToString() const override;
#endif
	};

	class WindowResizeEvent : public WindowEvent
	{
	public:
		WindowResizeEvent(void* nativeWindow, int32_t width, int32_t height);
		EVENT_CLASS_TYPE(EventType::WindowResize);
		EVENT_CLASS_CATEGORY(EventCategory_Window | EventCategory_Application);

		inline int32_t GetWidth() const noexcept { return width; }
		inline int32_t GetHeight() const noexcept { return height; }

#if GBC_ENABLE_LOGGING
		virtual std::string ToString() const override;
#endif
	private:
		int32_t width;
		int32_t height;
	};

	class WindowMoveEvent : public WindowEvent
	{
	public:
		WindowMoveEvent(void* nativeWindow, int32_t x, int32_t y);
		EVENT_CLASS_TYPE(EventType::WindowMove);
		EVENT_CLASS_CATEGORY(EventCategory_Window);

		inline int32_t GetX() const noexcept { return x; }
		inline int32_t GetY() const noexcept { return y; }

#if GBC_ENABLE_LOGGING
		virtual std::string ToString() const override;
#endif
	private:
		int32_t x;
		int32_t y;
	};

	class WindowFocusEvent : public WindowEvent
	{
	public:
		WindowFocusEvent(void* nativeWindow, bool focus);
		EVENT_CLASS_TYPE(EventType::WindowFocus);
		EVENT_CLASS_CATEGORY(EventCategory_Window | EventCategory_Application);

		inline bool IsFocused() const noexcept { return focus; }

#if GBC_ENABLE_LOGGING
		virtual std::string ToString() const override;
#endif
	private:
		bool focus;
	};

	class WindowMinimizeEvent : public WindowEvent
	{
	public:
		WindowMinimizeEvent(void* nativeWindow, bool minimize);
		EVENT_CLASS_TYPE(EventType::WindowMinimize);
		EVENT_CLASS_CATEGORY(EventCategory_Window | EventCategory_Application);

		inline bool IsMinimized() const noexcept { return minimize; }

#if GBC_ENABLE_LOGGING
		virtual std::string ToString() const override;
#endif
	private:
		bool minimize;
	};

	class WindowMaximizeEvent : public WindowEvent
	{
	public:
		WindowMaximizeEvent(void* nativeWindow, bool maximize);
		EVENT_CLASS_TYPE(EventType::WindowMaximize);
		EVENT_CLASS_CATEGORY(EventCategory_Window);

		inline bool IsMaximized() const noexcept { return maximize; }

#if GBC_ENABLE_LOGGING
		virtual std::string ToString() const override;
#endif
	private:
		bool maximize;
	};

	class WindowDropEvent : public WindowEvent
	{
	public:
		WindowDropEvent(void* nativeWindow, int32_t pathCount, const char** paths);
		EVENT_CLASS_TYPE(EventType::WindowDrop);
		EVENT_CLASS_CATEGORY(EventCategory_Window);

		inline int32_t GetPathCount() const noexcept { return pathCount; }
		inline const char** GetPaths() const noexcept { return paths; }

#if GBC_ENABLE_LOGGING
		virtual std::string ToString() const override;
#endif
	private:
		int32_t pathCount;
		const char** paths;
	};

	class WindowFramebufferResizeEvent : public WindowEvent
	{
	public:
		WindowFramebufferResizeEvent(void* nativeWindow, int32_t width, int32_t height);
		EVENT_CLASS_TYPE(EventType::WindowFramebufferResize);
		EVENT_CLASS_CATEGORY(EventCategory_Window);

		inline int32_t GetWidth() const noexcept { return width; }
		inline int32_t GetHeight() const noexcept { return height; }

#if GBC_ENABLE_LOGGING
		virtual std::string ToString() const override;
#endif
	private:
		int32_t width;
		int32_t height;
	};

	class WindowContentScaleEvent : public WindowEvent
	{
	public:
		WindowContentScaleEvent(void* nativeWindow, float scaleX, float scaleY);
		EVENT_CLASS_TYPE(EventType::WindowContentScale);
		EVENT_CLASS_CATEGORY(EventCategory_Window);

		inline float GetScaleX() const noexcept { return scaleX; }
		inline float GetScaleY() const noexcept { return scaleY; }

#if GBC_ENABLE_LOGGING
		virtual std::string ToString() const override;
#endif
	private:
		float scaleX;
		float scaleY;
	};

	class WindowRefreshEvent : public WindowEvent
	{
	public:
		WindowRefreshEvent(void* nativeWindow);
		EVENT_CLASS_TYPE(EventType::WindowRefresh);
		EVENT_CLASS_CATEGORY(EventCategory_Window);

#if GBC_ENABLE_LOGGING
		virtual std::string ToString() const override;
#endif
	};
}
