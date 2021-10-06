#pragma once

#include "GBC/Events/Event.h"
#include "GBC/Core/MouseButtons.h"

namespace gbc
{
	class MouseButtonPressEvent : public Event
	{
	public:
		MouseButtonPressEvent(MouseButton button, int32_t mods);
		EVENT_CLASS_TYPE(EventType::MouseButtonPress);
		EVENT_CLASS_CATEGORY(EventCategory_Mouse | EventCategory_MouseButton);

		inline MouseButton GetButton() const noexcept { return button; }
		inline int32_t GetMods() const noexcept { return mods; }

#if GBC_ENABLE_LOGGING
		virtual std::string ToString() const override;
#endif
	private:
		MouseButton button;
		int32_t mods;
	};

	class MouseButtonReleaseEvent : public Event
	{
	public:
		MouseButtonReleaseEvent(MouseButton button, int32_t mods);
		EVENT_CLASS_TYPE(EventType::MouseButtonRelease);
		EVENT_CLASS_CATEGORY(EventCategory_Mouse | EventCategory_MouseButton);

		inline MouseButton GetButton() const noexcept { return button; }
		inline int32_t GetMods() const noexcept { return mods; }

#if GBC_ENABLE_LOGGING
		virtual std::string ToString() const override;
#endif
	private:
		MouseButton button;
		int32_t mods;
	};

	class MouseMoveEvent : public Event
	{
	public:
		MouseMoveEvent(float x, float y);
		EVENT_CLASS_TYPE(EventType::MouseMove);
		EVENT_CLASS_CATEGORY(EventCategory_Mouse);

		inline float GetX() const noexcept { return x; }
		inline float GetY() const noexcept { return y; }

#if GBC_ENABLE_LOGGING
		virtual std::string ToString() const override;
#endif
	private:
		float x;
		float y;
	};

	class MouseScrollEvent : public Event
	{
	public:
		MouseScrollEvent(float offsetX, float offsetY);
		EVENT_CLASS_TYPE(EventType::MouseScroll);
		EVENT_CLASS_CATEGORY(EventCategory_Mouse);

		inline float GetOffsetX() const noexcept { return offsetX; }
		inline float GetOffsetY() const noexcept { return offsetY; }

#if GBC_ENABLE_LOGGING
		virtual std::string ToString() const override;
#endif
	private:
		float offsetX;
		float offsetY;
	};

	class MouseEnterEvent : public Event
	{
	public:
		MouseEnterEvent(bool enter);
		EVENT_CLASS_TYPE(EventType::MouseEnter);
		EVENT_CLASS_CATEGORY(EventCategory_Mouse);

		inline bool HasEntered() const noexcept { return enter; }

#if GBC_ENABLE_LOGGING
		virtual std::string ToString() const override;
#endif
	private:
		bool enter;
	};
}
