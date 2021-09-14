#include "gbcpch.h"
#include "MouseEvents.h"

namespace gbc
{
	MouseButtonPressEvent::MouseButtonPressEvent(MouseButton button, int32_t mods)
		: button(button), mods(mods) {}

#if GBC_ENABLE_LOGGING
	std::string MouseButtonPressEvent::ToString() const
	{
		std::stringstream stream;
		stream << "MouseButtonPressEvent: button=" << static_cast<uint16_t>(button) << " mods=" << mods;
		return stream.str();
	}
#endif

	MouseButtonReleaseEvent::MouseButtonReleaseEvent(MouseButton button, int32_t mods)
		: button(button), mods(mods) {}

#if GBC_ENABLE_LOGGING
	std::string MouseButtonReleaseEvent::ToString() const
	{
		std::stringstream stream;
		stream << "MouseButtonReleaseEvent: button=" << static_cast<uint16_t>(button) << " mods=" << mods;
		return stream.str();
	}
#endif

	MouseMoveEvent::MouseMoveEvent(float x, float y)
		: x(x), y(y) {}

#if GBC_ENABLE_LOGGING
	std::string MouseMoveEvent::ToString() const
	{
		std::stringstream stream;
		stream << "MouseMoveEvent: x=" << x << " y=" << y;
		return stream.str();
	}
#endif

	MouseScrollEvent::MouseScrollEvent(float offsetX, float offsetY)
		: offsetX(offsetX), offsetY(offsetY) {}

#if GBC_ENABLE_LOGGING
	std::string MouseScrollEvent::ToString() const
	{
		std::stringstream stream;
		stream << "MouseScrollEvent: offsetX=" << offsetX << " offsetY=" << offsetY;
		return stream.str();
	}
#endif

	MouseEnterEvent::MouseEnterEvent(bool enter)
		: enter(enter) {}

#if GBC_ENABLE_LOGGING
	std::string MouseEnterEvent::ToString() const
	{
		std::stringstream stream;
		stream << "MouseEnterEvent: enter=" << enter;
		return stream.str();
	}
#endif
}
