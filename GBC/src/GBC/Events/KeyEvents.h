#pragma once

#include "Event.h"
#include "GBC/Core/Keycodes.h"

namespace gbc
{
	class KeyPressEvent : public Event
	{
	public:
		KeyPressEvent(Keycode keycode, int mods);
		EVENT_CLASS_TYPE(EventType::KeyPress);
		EVENT_CLASS_CATEGORY(EventCategory_Keyboard);

		inline Keycode GetKeycode() const noexcept { return keycode; }
		inline int GetMods() const noexcept { return mods; }

		virtual std::string ToString() const override;
	private:
		Keycode keycode;
		int mods;
	};

	class KeyRepeatEvent : public Event
	{
	public:
		KeyRepeatEvent(Keycode keycode, int mods);
		EVENT_CLASS_TYPE(EventType::KeyRepeat);
		EVENT_CLASS_CATEGORY(EventCategory_Keyboard);

		inline Keycode GetKeycode() const noexcept { return keycode; }
		inline int GetMods() const noexcept { return mods; }

		virtual std::string ToString() const override;
	private:
		Keycode keycode;
		int mods;
	};

	class KeyReleaseEvent : public Event
	{
	public:
		KeyReleaseEvent(Keycode keycode, int mods);
		EVENT_CLASS_TYPE(EventType::KeyRelease);
		EVENT_CLASS_CATEGORY(EventCategory_Keyboard);

		inline Keycode GetKeycode() const noexcept { return keycode; }
		inline int GetMods() const noexcept { return mods; }

		virtual std::string ToString() const override;
	private:
		Keycode keycode;
		int mods;
	};

	class KeyCharEvent : public Event
	{
	public:
		KeyCharEvent(uint32_t codepoint);
		EVENT_CLASS_TYPE(EventType::KeyChar);
		EVENT_CLASS_CATEGORY(EventCategory_Keyboard);

		inline uint32_t GetCodepoint() const noexcept { return codepoint; }

		virtual std::string ToString() const override;
	private:
		uint32_t codepoint;
	};

	class KeyCharModsEvent : public Event
	{
	public:
		KeyCharModsEvent(uint32_t codepoint, int mods);
		EVENT_CLASS_TYPE(EventType::KeyCharMods);
		EVENT_CLASS_CATEGORY(EventCategory_Keyboard);

		inline uint32_t GetCodepoint() const noexcept { return codepoint; }
		inline int GetMods() const noexcept { return mods; }

		virtual std::string ToString() const override;
	private:
		uint32_t codepoint;
		int mods;
	};
}
