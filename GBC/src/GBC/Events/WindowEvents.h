#pragma once

#include "Event.h"

namespace gbc
{
	class WindowCloseEvent : public Event
	{
	public:
		EVENT_CLASS_TYPE(EventType::WindowClose)
		EVENT_CLASS_CATEGORY(EventCategory_Window | EventCategory_Application)

		virtual std::string ToString() const override;
	};

	class WindowResizeEvent : public Event
	{
	public:
		WindowResizeEvent(int width, int height);
		EVENT_CLASS_TYPE(EventType::WindowResize)
		EVENT_CLASS_CATEGORY(EventCategory_Window | EventCategory_Application)

		inline int GetWidth() const { return width; }
		inline int GetHeight() const { return height; }

		virtual std::string ToString() const override;
	private:
		int width;
		int height;
	};

	class WindowMoveEvent : public Event
	{
	public:
		WindowMoveEvent(int x, int y);
		EVENT_CLASS_TYPE(EventType::WindowMove)
		EVENT_CLASS_CATEGORY(EventCategory_Window)

		inline int GetX() const { return x; }
		inline int GetY() const { return y; }

		virtual std::string ToString() const override;
	private:
		int x;
		int y;
	};

	class WindowFocusEvent : public Event
	{
	public:
		WindowFocusEvent(bool focus);
		EVENT_CLASS_TYPE(EventType::WindowFocus)
		EVENT_CLASS_CATEGORY(EventCategory_Window)

		inline bool IsFocused() const { return focus; }

		virtual std::string ToString() const override;
	private:
		bool focus;
	};

	class WindowMinimizeEvent : public Event
	{
	public:
		WindowMinimizeEvent(bool minimize);
		EVENT_CLASS_TYPE(EventType::WindowMinimize)
		EVENT_CLASS_CATEGORY(EventCategory_Window | EventCategory_Application)

		inline bool IsMinimized() const { return minimize; }

		virtual std::string ToString() const override;
	private:
		bool minimize;
	};

	class WindowMaximizeEvent : public Event
	{
	public:
		WindowMaximizeEvent(bool maximize);
		EVENT_CLASS_TYPE(EventType::WindowMaximize)
		EVENT_CLASS_CATEGORY(EventCategory_Window)

		inline bool IsMaximized() const { return maximize; }

		virtual std::string ToString() const override;
	private:
		bool maximize;
	};

	class WindowDropEvent : public Event
	{
	public:
		WindowDropEvent(int pathCount, const char** paths);
		~WindowDropEvent();
		EVENT_CLASS_TYPE(EventType::WindowDrop)
		EVENT_CLASS_CATEGORY(EventCategory_Window | EventCategory_Application)

		inline int GetPathCount() const { return pathCount; }
		inline char** GetPaths() const { return paths; }

		virtual std::string ToString() const override;
	private:
		int pathCount;
		char** paths;
	};

	class WindowFramebufferResizeEvent : public Event
	{
	public:
		WindowFramebufferResizeEvent(int width, int height);
		EVENT_CLASS_TYPE(EventType::WindowFramebufferResize)
		EVENT_CLASS_CATEGORY(EventCategory_Window)

		inline int GetWidth() const { return width; }
		inline int GetHeight() const { return height; }

		virtual std::string ToString() const override;
	private:
		int width;
		int height;
	};

	class WindowContentScaleEvent : public Event
	{
	public:
		WindowContentScaleEvent(float scaleX, float scaleY);
		EVENT_CLASS_TYPE(EventType::WindowContentScale)
		EVENT_CLASS_CATEGORY(EventCategory_Window)

		inline float GetScaleX() const { return scaleX; }
		inline float GetScaleY() const { return scaleY; }

		virtual std::string ToString() const override;
	private:
		float scaleX;
		float scaleY;
	};

	class WindowRefreshEvent : public Event
	{
	public:
		EVENT_CLASS_TYPE(EventType::WindowRefresh)
		EVENT_CLASS_CATEGORY(EventCategory_Window)

		virtual std::string ToString() const override;
	};
}
