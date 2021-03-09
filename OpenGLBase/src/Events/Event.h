#pragma once

#include <string>
#include <ostream>

enum class EventType
{
	WindowClose, WindowResize, WindowMove, WindowFocus, WindowMinimize, WindowMaximize, 
		WindowDrop, WindowFramebufferResize, WindowContentScale, WindowRefresh,
	KeyPress, KeyRepeat, KeyRelease, KeyChar,
	MouseButtonPress, MouseButtonRelease, MouseMove, MouseScroll, MouseEnter,
	JoystickConnect, MonitorConnect
};

class Event
{
public:
	Event(EventType type)
		: type(type) {}
	virtual ~Event() = default;

	inline EventType GetType() const { return type; }
	virtual std::string ToString() const = 0;

	inline bool IsHandled() const { return handled; }
	inline void Handle() { handled = true; }
private:
	EventType type;
	bool handled = false;
};

std::ostream& operator<<(std::ostream& ostr, const Event& event);
