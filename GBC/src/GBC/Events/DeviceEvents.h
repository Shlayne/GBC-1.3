#pragma once

#include "Event.h"

namespace gbc
{
	class JoystickConnectEvent : public Event
	{
	public:
		JoystickConnectEvent(int jid, bool connect);
		EVENT_CLASS_TYPE(EventType::JoystickConnect);
		EVENT_CLASS_CATEGORY(EventCategory_Device | EventCategory_Application);

		inline int GetJID() const noexcept { return jid; }
		inline bool IsConnected() const noexcept { return connect; }

#if GBC_ENABLE_LOGGING
		virtual std::string ToString() const override;
#endif
	private:
		int jid;
		bool connect;
	};
}
