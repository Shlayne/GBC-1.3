#pragma once

#include "GBC/Core/Joysticks.h"
#include "GBC/Events/Event.h"

namespace gbc
{
	class JoystickConnectEvent : public Event
	{
	public:
		constexpr JoystickConnectEvent(Joystick joystick, bool connect) : joystick(joystick), connect(connect) {}
		EVENT_CLASS_TYPE(EventType::JoystickConnect);
		EVENT_CLASS_CATEGORY(EventCategory_Device | EventCategory_Application);

		constexpr Joystick GetJoystick() const noexcept { return joystick; }
		constexpr bool IsConnected() const noexcept { return connect; }

#if GBC_ENABLE_LOGGING
		virtual std::string ToString() const override;
#endif
	private:
		Joystick joystick;
		bool connect;
	};
}
