#include "gbcpch.h"
#include "Event.h"

namespace gbc
{
	EventDispatcher::EventDispatcher(Event& event)
		: event(event) {}

	bool Event::IsInCategory(EventCategory category) const
	{
		return GetCategoryFlags() & category;
	}
}

std::ostream& operator<<(std::ostream& ostr, const gbc::Event& event)
{
	return ostr << event.ToString();
}
