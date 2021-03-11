#include "gbcpch.h"
#include "Event.h"

namespace gbc
{
	EventDispatcher::EventDispatcher(Event& event)
		: event(event)
	{

	}
}

std::ostream& operator<<(std::ostream& ostr, const gbc::Event& event)
{
	return ostr << event.ToString();
}
