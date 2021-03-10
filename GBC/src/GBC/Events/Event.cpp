#include "gbcpch.h"
#include "Event.h"

std::ostream& operator<<(std::ostream& ostr, const gbc::Event& event)
{
	return ostr << event.ToString();
}
