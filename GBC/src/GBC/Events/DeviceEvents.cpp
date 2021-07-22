#include "gbcpch.h"
#include "DeviceEvents.h"

namespace gbc
{
	JoystickConnectEvent::JoystickConnectEvent(int jid, bool connect)
		: jid(jid), connect(connect) {}

	std::string JoystickConnectEvent::ToString() const
	{
		std::stringstream stream;
		stream << "JoystickConnectEvent: connected=" << connect;
		return stream.str();
	}
}