#include "Event.h"

std::ostream& operator<<(std::ostream& ostr, const cbc::Event& event)
{
	return ostr << event.ToString();
}
