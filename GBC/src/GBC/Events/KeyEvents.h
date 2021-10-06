#pragma once

#include "GBC/Events/Event.h"
#include "GBC/Core/Keycodes.h"

namespace gbc
{
	class KeyPressEvent : public Event
	{
	public:
		KeyPressEvent(Keycode keycode, int32_t mods);
		EVENT_CLASS_TYPE(EventType::KeyPress);
		EVENT_CLASS_CATEGORY(EventCategory_Keyboard);

		inline Keycode GetKeycode() const noexcept { return keycode; }
		inline int32_t GetMods() const noexcept { return mods; }

#if GBC_ENABLE_LOGGING
		virtual std::string ToString() const override;
#endif
	private:
		Keycode keycode;
		int32_t mods;
	};

	class KeyRepeatEvent : public Event
	{
	public:
		KeyRepeatEvent(Keycode keycode, int32_t mods);
		EVENT_CLASS_TYPE(EventType::KeyRepeat);
		EVENT_CLASS_CATEGORY(EventCategory_Keyboard);

		inline Keycode GetKeycode() const noexcept { return keycode; }
		inline int32_t GetMods() const noexcept { return mods; }

#if GBC_ENABLE_LOGGING
		virtual std::string ToString() const override;
#endif
	private:
		Keycode keycode;
		int32_t mods;
	};

	class KeyReleaseEvent : public Event
	{
	public:
		KeyReleaseEvent(Keycode keycode, int32_t mods);
		EVENT_CLASS_TYPE(EventType::KeyRelease);
		EVENT_CLASS_CATEGORY(EventCategory_Keyboard);

		inline Keycode GetKeycode() const noexcept { return keycode; }
		inline int32_t GetMods() const noexcept { return mods; }

#if GBC_ENABLE_LOGGING
		virtual std::string ToString() const override;
#endif
	private:
		Keycode keycode;
		int32_t mods;
	};

	class KeyCharEvent : public Event
	{
	public:
		KeyCharEvent(uint32_t codepoint);
		EVENT_CLASS_TYPE(EventType::KeyChar);
		EVENT_CLASS_CATEGORY(EventCategory_Keyboard);

		inline uint32_t GetCodepoint() const noexcept { return codepoint; }

#if GBC_ENABLE_LOGGING
		virtual std::string ToString() const override;
#endif
	private:
		uint32_t codepoint;
	};

	class KeyCharModsEvent : public Event
	{
	public:
		KeyCharModsEvent(uint32_t codepoint, int32_t mods);
		EVENT_CLASS_TYPE(EventType::KeyCharMods);
		EVENT_CLASS_CATEGORY(EventCategory_Keyboard);

		inline uint32_t GetCodepoint() const noexcept { return codepoint; }
		inline int32_t GetMods() const noexcept { return mods; }

#if GBC_ENABLE_LOGGING
		virtual std::string ToString() const override;
#endif
	private:
		uint32_t codepoint;
		int32_t mods;
	};
}
