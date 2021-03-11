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

#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::type; }\
							   virtual EventType GetType() const override { return GetStaticType(); }

	class Event
	{
	public:
		virtual ~Event() = default;

		virtual EventType GetType() const = 0;
		virtual std::string ToString() const = 0;

		bool handled = false;
	};

	class EventDispatcher
	{
	public:
		EventDispatcher(Event& event);

		template<typename E, typename Fn>
		bool Dispatch(const Fn& func)
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
