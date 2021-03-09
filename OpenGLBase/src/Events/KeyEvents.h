#pragma once

#include "Event.h"
#include "Core/Keycodes.h"

namespace cbc
{
	class KeyPressEvent : public Event
	{
	public:
		KeyPressEvent(Keycode keycode, int mods);

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

		inline unsigned int GetCodepoint() const { return codepoint; }
		inline int GetMods() const { return mods; }

		virtual std::string ToString() const override;
	private:
		unsigned int codepoint;
		int mods;
	};
}
