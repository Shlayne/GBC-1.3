#include "Event.h"

std::ostream& operator<<(std::ostream& ostr, const Event& event)
{
	return ostr << event.ToString();
}
