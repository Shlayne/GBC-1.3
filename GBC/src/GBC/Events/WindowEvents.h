#pragma once

#include "Event.h"

namespace gbc
{
	class WindowCloseEvent : public Event
	{
	public:
		EVENT_CLASS_TYPE(WindowClose)

		virtual std::string ToString() const override;
	};

	class WindowResizeEvent : public Event
	{
	public:
		WindowResizeEvent(int width, int height);
		EVENT_CLASS_TYPE(WindowResize)

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
		EVENT_CLASS_TYPE(WindowMove)

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
		EVENT_CLASS_TYPE(WindowFocus)

		inline bool IsFocused() const { return focus; }

		virtual std::string ToString() const override;
	private:
		bool focus;
	};

	class WindowMinimizeEvent : public Event
	{
	public:
		WindowMinimizeEvent(bool minimize);
		EVENT_CLASS_TYPE(WindowMinimize)

		inline bool IsMinimized() const { return minimize; }

		virtual std::string ToString() const override;
	private:
		bool minimize;
	};

	class WindowMaximizeEvent : public Event
	{
	public:
		WindowMaximizeEvent(bool maximize);
		EVENT_CLASS_TYPE(WindowMaximize)

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
		EVENT_CLASS_TYPE(WindowDrop)

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
		EVENT_CLASS_TYPE(WindowFramebufferResize)

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
		EVENT_CLASS_TYPE(WindowContentScale)

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
		EVENT_CLASS_TYPE(WindowRefresh)

		virtual std::string ToString() const override;
	};
}
