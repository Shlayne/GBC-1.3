#include "gbcpch.h"
#if GBC_ENABLE_LOGGING
#include "DeviceEvents.h"

namespace gbc
{
	std::string JoystickConnectEvent::ToString() const
	{
		std::stringstream stream;
		stream << "JoystickConnectEvent: connected=" << connect;
		return stream.str();
	}
}
#endif
