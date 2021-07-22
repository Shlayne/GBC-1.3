#include "gbcpch.h"
#include "MouseEvents.h"

namespace gbc
{
	MouseButtonPressEvent::MouseButtonPressEvent(MouseButton button, int mods)
		: button(button), mods(mods) {}

	std::string MouseButtonPressEvent::ToString() const
	{
		std::stringstream stream;
		stream << "MouseButtonPressEvent: button=" << static_cast<unsigned short>(button) << " mods=" << mods;
		return stream.str();
	}

	MouseButtonReleaseEvent::MouseButtonReleaseEvent(MouseButton button, int mods)
		: button(button), mods(mods) {}

	std::string MouseButtonReleaseEvent::ToString() const
	{
		std::stringstream stream;
		stream << "MouseButtonReleaseEvent: button=" << static_cast<unsigned short>(button) << " mods=" << mods;
		return stream.str();
	}

	MouseMoveEvent::MouseMoveEvent(float x, float y)
		: x(x), y(y) {}

	std::string MouseMoveEvent::ToString() const
	{
		std::stringstream stream;
		stream << "MouseMoveEvent: x=" << x << " y=" << y;
		return stream.str();
	}

	MouseScrollEvent::MouseScrollEvent(float offsetX, float offsetY)
		: offsetX(offsetX), offsetY(offsetY) {}

	std::string MouseScrollEvent::ToString() const
	{
		std::stringstream stream;
		stream << "MouseScrollEvent: offsetX=" << offsetX << " offsetY=" << offsetY;
		return stream.str();
	}

	MouseEnterEvent::MouseEnterEvent(bool enter)
		: enter(enter) {}

	std::string MouseEnterEvent::ToString() const
	{
		std::stringstream stream;
		stream << "MouseEnterEvent: enter=" << enter;
		return stream.str();
	}
}
