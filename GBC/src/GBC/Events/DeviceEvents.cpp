#include "gbcpch.h"
#include "DeviceEvents.h"

namespace gbc
{
	JoystickConnectEvent::JoystickConnectEvent(int32_t jid, bool connect)
		: jid(jid), connect(connect) {}

#if GBC_ENABLE_LOGGING
	std::string JoystickConnectEvent::ToString() const
	{
		std::stringstream stream;
		stream << "JoystickConnectEvent: connected=" << connect;
		return stream.str();
	}
#endif
}
