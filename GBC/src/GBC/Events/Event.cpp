#include "gbcpch.h"
#include "Event.h"

namespace gbc
{
	EventDispatcher::EventDispatcher(Event& event)
		: event(event) {}

	bool Event::IsInCategory(EventCategoryFlags category) const noexcept
	{
		return GetCategoryFlags() & category;
	}
}

std::ostream& operator<<(std::ostream& ostr, const gbc::Event& event)
{
	return ostr << event.ToString();
}
