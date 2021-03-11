#include "gbcpch.h"
#include "KeyEvents.h"

namespace gbc
{
	KeyPressEvent::KeyPressEvent(Keycode keycode, int mods)
		: keycode(keycode), mods(mods)
	{

	}

	std::string KeyPressEvent::ToString() const
	{
		std::stringstream stream;
		stream << "KeyPressEvent: keycode=" << static_cast<unsigned short>(keycode) << " mods=" << mods;
		return stream.str();
	}

	KeyRepeatEvent::KeyRepeatEvent(Keycode keycode, int mods)
		: keycode(keycode), mods(mods)
	{

	}

	std::string KeyRepeatEvent::ToString() const
	{
		std::stringstream stream;
		stream << "KeyRepeatEvent: keycode=" << static_cast<unsigned short>(keycode) << " mods=" << mods;
		return stream.str();
	}

	KeyReleaseEvent::KeyReleaseEvent(Keycode keycode, int mods)
		: keycode(keycode), mods(mods)
	{

	}

	std::string KeyReleaseEvent::ToString() const
	{
		std::stringstream stream;
		stream << "KeyReleaseEvent: keycode=" << static_cast<unsigned short>(keycode) << " mods=" << mods;
		return stream.str();
	}

	KeyCharEvent::KeyCharEvent(unsigned int codepoint, int mods)
		: codepoint(codepoint), mods(mods)
	{

	}

	std::string KeyCharEvent::ToString() const
	{
		std::stringstream stream;
		stream << "KeyCharEvent: codepoint=" << codepoint << " mods=" << mods;
		return stream.str();
	}
}
