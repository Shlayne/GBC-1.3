#pragma once

#include <string>

namespace gbc
{
	enum class EventType
	{
		WindowClose, WindowResize, WindowMove, WindowFocus, WindowMinimize, WindowMaximize, 
			WindowDrop, WindowFramebufferResize, WindowContentScale, WindowRefresh,
		KeyPress, KeyRepeat, KeyRelease, KeyChar,
		MouseButtonPress, MouseButtonRelease, MouseMove, MouseScroll, MouseEnter,
		JoystickConnect, MonitorConnect
	};

	using EventCategoryFlags = unsigned int;
	enum EventCategory : EventCategoryFlags
	{
		EventCategory_None = 0,
		EventCategory_Application = 1 << 0,
		EventCategory_Window      = 1 << 1,
		EventCategory_Keyboard    = 1 << 2,
		EventCategory_Mouse       = 1 << 3,
		EventCategory_MouseButton = 1 << 4,
		EventCategory_Device      = 1 << 5
	};

#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return type; }\
							   virtual EventType GetType() const override { return GetStaticType(); }

#define EVENT_CLASS_CATEGORY(categories) virtual EventCategoryFlags GetCategoryFlags() const override { return categories; }

	class Event
	{
	public:
		virtual ~Event() = default;

		virtual EventType GetType() const = 0;
		virtual EventCategoryFlags GetCategoryFlags() const = 0;
		bool IsInCategory(EventCategory category) const;

		virtual std::string ToString() const = 0;

		bool handled = false;
	};

	class EventDispatcher
	{
	public:
		EventDispatcher(Event& event);

		template<typename E, typename Func>
		bool Dispatch(const Func& func)
		{
			if (event.GetType() == E::GetStaticType())
			{
				event.handled |= func(static_cast<E&>(event));
				return true;
			}
			return false;
		}
	private:
		Event& event;
	};
}

std::ostream& operator<<(std::ostream& ostr, const gbc::Event& event);
