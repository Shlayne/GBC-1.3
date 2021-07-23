#pragma once

#include "Event.h"

namespace gbc
{
	class WindowCloseEvent : public Event
	{
	public:
		EVENT_CLASS_TYPE(EventType::WindowClose);
		EVENT_CLASS_CATEGORY(EventCategory_Window | EventCategory_Application);

#if GBC_ENABLE_LOGGING
		virtual std::string ToString() const override;
#endif
	};

	class WindowResizeEvent : public Event
	{
	public:
		WindowResizeEvent(int width, int height);
		EVENT_CLASS_TYPE(EventType::WindowResize);
		EVENT_CLASS_CATEGORY(EventCategory_Window | EventCategory_Application);

		inline int GetWidth() const noexcept { return width; }
		inline int GetHeight() const noexcept { return height; }

#if GBC_ENABLE_LOGGING
		virtual std::string ToString() const override;
#endif
	private:
		int width;
		int height;
	};

	class WindowMoveEvent : public Event
	{
	public:
		WindowMoveEvent(int x, int y);
		EVENT_CLASS_TYPE(EventType::WindowMove);
		EVENT_CLASS_CATEGORY(EventCategory_Window);

		inline int GetX() const noexcept { return x; }
		inline int GetY() const noexcept { return y; }

#if GBC_ENABLE_LOGGING
		virtual std::string ToString() const override;
#endif
	private:
		int x;
		int y;
	};

	class WindowFocusEvent : public Event
	{
	public:
		WindowFocusEvent(bool focus);
		EVENT_CLASS_TYPE(EventType::WindowFocus);
		EVENT_CLASS_CATEGORY(EventCategory_Window);

		inline bool IsFocused() const noexcept { return focus; }

#if GBC_ENABLE_LOGGING
		virtual std::string ToString() const override;
#endif
	private:
		bool focus;
	};

	class WindowMinimizeEvent : public Event
	{
	public:
		WindowMinimizeEvent(bool minimize);
		EVENT_CLASS_TYPE(EventType::WindowMinimize);
		EVENT_CLASS_CATEGORY(EventCategory_Window | EventCategory_Application);

		inline bool IsMinimized() const noexcept { return minimize; }

#if GBC_ENABLE_LOGGING
		virtual std::string ToString() const override;
#endif
	private:
		bool minimize;
	};

	class WindowMaximizeEvent : public Event
	{
	public:
		WindowMaximizeEvent(bool maximize);
		EVENT_CLASS_TYPE(EventType::WindowMaximize);
		EVENT_CLASS_CATEGORY(EventCategory_Window);

		inline bool IsMaximized() const noexcept { return maximize; }

#if GBC_ENABLE_LOGGING
		virtual std::string ToString() const override;
#endif
	private:
		bool maximize;
	};

	class WindowDropEvent : public Event
	{
	public:
		WindowDropEvent(int pathCount, const char** paths);
		EVENT_CLASS_TYPE(EventType::WindowDrop);
		EVENT_CLASS_CATEGORY(EventCategory_Window);

		inline int GetPathCount() const noexcept { return pathCount; }
		inline const char** GetPaths() const noexcept { return paths; }

#if GBC_ENABLE_LOGGING
		virtual std::string ToString() const override;
#endif
	private:
		int pathCount;
		const char** paths;
	};

	class WindowFramebufferResizeEvent : public Event
	{
	public:
		WindowFramebufferResizeEvent(int width, int height);
		EVENT_CLASS_TYPE(EventType::WindowFramebufferResize);
		EVENT_CLASS_CATEGORY(EventCategory_Window);

		inline int GetWidth() const noexcept { return width; }
		inline int GetHeight() const noexcept { return height; }

#if GBC_ENABLE_LOGGING
		virtual std::string ToString() const override;
#endif
	private:
		int width;
		int height;
	};

	class WindowContentScaleEvent : public Event
	{
	public:
		WindowContentScaleEvent(float scaleX, float scaleY);
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

	class WindowRefreshEvent : public Event
	{
	public:
		EVENT_CLASS_TYPE(EventType::WindowRefresh);
		EVENT_CLASS_CATEGORY(EventCategory_Window);

#if GBC_ENABLE_LOGGING
		virtual std::string ToString() const override;
#endif
	};
}
