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

	MonitorConnectEvent::MonitorConnectEvent(bool connect)
		: connect(connect) {}

	std::string MonitorConnectEvent::ToString() const
	{
		std::stringstream stream;
		stream << "MonitorConnectEvent: connected=" << connect;
		return stream.str();
	}
}