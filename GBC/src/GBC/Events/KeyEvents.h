#pragma once

#include "Event.h"
#include "GBC/Core/Keycodes.h"

namespace gbc
{
	class KeyPressEvent : public Event
	{
	public:
		KeyPressEvent(Keycode keycode, int mods);
		EVENT_CLASS_TYPE(KeyPress)

		inline Keycode GetKeycode() const { return keycode; }
		inline int GetMods() const { return mods; }

		virtual std::string ToString() const override;
	private:
		Keycode keycode;
		int mods;
	};

	class KeyRepeatEvent : public Event
	{
	public:
		KeyRepeatEvent(Keycode keycode, int mods);
		EVENT_CLASS_TYPE(KeyRepeat)

		inline Keycode GetKeycode() const { return keycode; }
		inline int GetMods() const { return mods; }

		virtual std::string ToString() const override;
	private:
		Keycode keycode;
		int mods;
	};

	class KeyReleaseEvent : public Event
	{
	public:
		KeyReleaseEvent(Keycode keycode, int mods);
		EVENT_CLASS_TYPE(KeyRelease)

		inline Keycode GetKeycode() const { return keycode; }
		inline int GetMods() const { return mods; }

		virtual std::string ToString() const override;
	private:
		Keycode keycode;
		int mods;
	};

	class KeyCharEvent : public Event
	{
	public:
		KeyCharEvent(unsigned int codepoint, int mods);
		EVENT_CLASS_TYPE(KeyChar)

		inline unsigned int GetCodepoint() const { return codepoint; }
		inline int GetMods() const { return mods; }

		virtual std::string ToString() const override;
	private:
		unsigned int codepoint;
		int mods;
	};
}
