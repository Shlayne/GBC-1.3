#include "cbcpch.h"
#include "MouseEvents.h"

namespace cbc
{
	MouseButtonPressEvent::MouseButtonPressEvent(Mousecode button, int mods)
		: Event(EventType::MouseButtonPress), button(button), mods(mods)
	{

	}

	std::string MouseButtonPressEvent::ToString() const
	{
		std::stringstream stream;
		stream << "MouseButtonPressEvent: button=" << static_cast<unsigned short>(button) << " mods=" << mods;
		return stream.str();
	}

	MouseButtonReleaseEvent::MouseButtonReleaseEvent(Mousecode button, int mods)
		: Event(EventType::MouseButtonRelease), button(button), mods(mods)
	{

	}

	std::string MouseButtonReleaseEvent::ToString() const
	{
		std::stringstream stream;
		stream << "MouseButtonReleaseEvent: button=" << static_cast<unsigned short>(button) << " mods=" << mods;
		return stream.str();
	}

	MouseMoveEvent::MouseMoveEvent(float x, float y)
		: Event(EventType::MouseMove), x(x), y(y)
	{

	}

	std::string MouseMoveEvent::ToString() const
	{
		std::stringstream stream;
		stream << "MouseMoveEvent: x=" << x << " y=" << y;
		return stream.str();
	}

	MouseScrollEvent::MouseScrollEvent(float offsetX, float offsetY)
		: Event(EventType::MouseScroll), offsetX(offsetX), offsetY(offsetY)
	{

	}

	std::string MouseScrollEvent::ToString() const
	{
		std::stringstream stream;
		stream << "MouseScrollEvent: offsetX=" << offsetX << " offsetY=" << offsetY;
		return stream.str();
	}

	MouseEnterEvent::MouseEnterEvent(bool enter)
		: Event(EventType::MouseEnter), enter(enter)
	{

	}

	std::string MouseEnterEvent::ToString() const
	{
		std::stringstream stream;
		stream << "MouseEnterEvent: enter=" << enter;
		return stream.str();
	}
}
