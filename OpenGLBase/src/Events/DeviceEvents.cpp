#include "cbcpch.h"
#include "DeviceEvents.h"

namespace cbc
{
	JoystickConnectEvent::JoystickConnectEvent(int jid, bool connect)
		: Event(EventType::JoystickConnect), jid(jid), connect(connect)
	{

	}

	std::string JoystickConnectEvent::ToString() const
	{
		std::stringstream stream;
		stream << "JoystickConnectEvent: connected=" << connect;
		return stream.str();
	}

	MonitorConnectEvent::MonitorConnectEvent(bool connect)
		: Event(EventType::MonitorConnect), connect(connect)
	{

	}

	std::string MonitorConnectEvent::ToString() const
	{
		std::stringstream stream;
		stream << "MonitorConnectEvent: connected=" << connect;
		return stream.str();
	}
}