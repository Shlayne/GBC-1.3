#include "gbcpch.h"
#include "KeyEvents.h"

namespace gbc
{
	KeyPressEvent::KeyPressEvent(Keycode keycode, int32_t mods)
		: keycode(keycode), mods(mods) {}

#if GBC_ENABLE_LOGGING
	std::string KeyPressEvent::ToString() const
	{
		std::stringstream stream;
		stream << "KeyPressEvent: keycode=" << static_cast<uint16_t>(keycode) << " mods=" << mods;
		return stream.str();
	}
#endif

	KeyRepeatEvent::KeyRepeatEvent(Keycode keycode, int32_t mods)
		: keycode(keycode), mods(mods) {}

#if GBC_ENABLE_LOGGING
	std::string KeyRepeatEvent::ToString() const
	{
		std::stringstream stream;
		stream << "KeyRepeatEvent: keycode=" << static_cast<uint16_t>(keycode) << " mods=" << mods;
		return stream.str();
	}
#endif

	KeyReleaseEvent::KeyReleaseEvent(Keycode keycode, int32_t mods)
		: keycode(keycode), mods(mods) {}

#if GBC_ENABLE_LOGGING
	std::string KeyReleaseEvent::ToString() const
	{
		std::stringstream stream;
		stream << "KeyReleaseEvent: keycode=" << static_cast<uint16_t>(keycode) << " mods=" << mods;
		return stream.str();
	}
#endif

	KeyCharEvent::KeyCharEvent(uint32_t codepoint)
		: codepoint(codepoint) {}

#if GBC_ENABLE_LOGGING
	std::string KeyCharEvent::ToString() const
	{
		std::stringstream stream;
		stream << "KeyCharEvent: codepoint=" << codepoint;
		return stream.str();
	}
#endif

	KeyCharModsEvent::KeyCharModsEvent(uint32_t codepoint, int32_t mods)
		: codepoint(codepoint), mods(mods) {}

#if GBC_ENABLE_LOGGING
	std::string KeyCharModsEvent::ToString() const
	{
		std::stringstream stream;
		stream << "KeyCharEvent: codepoint=" << codepoint << " mods=" << mods;
		return stream.str();
	}
#endif
}
