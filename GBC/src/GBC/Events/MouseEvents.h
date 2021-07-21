#pragma once

#include "Event.h"
#include "GBC/Core/Mousecodes.h"

namespace gbc
{
	class MouseButtonPressEvent : public Event
	{
	public:
		MouseButtonPressEvent(Mousecode button, int mods);
		EVENT_CLASS_TYPE(EventType::MouseButtonPress);
		EVENT_CLASS_CATEGORY(EventCategory_Mouse | EventCategory_MouseButton);

		inline Mousecode GetButton() const noexcept { return button; }
		inline int GetMods() const noexcept { return mods; }

		virtual std::string ToString() const override;
	private:
		Mousecode button;
		int mods;
	};

	class MouseButtonReleaseEvent : public Event
	{
	public:
		MouseButtonReleaseEvent(Mousecode button, int mods);
		EVENT_CLASS_TYPE(EventType::MouseButtonRelease);
		EVENT_CLASS_CATEGORY(EventCategory_Mouse | EventCategory_MouseButton);

		inline Mousecode GetButton() const noexcept { return button; }
		inline int GetMods() const noexcept { return mods; }

		virtual std::string ToString() const override;
	private:
		Mousecode button;
		int mods;
	};

	class MouseMoveEvent : public Event
	{
	public:
		MouseMoveEvent(float x, float y);
		EVENT_CLASS_TYPE(EventType::MouseMove);
		EVENT_CLASS_CATEGORY(EventCategory_Mouse);

		inline float GetX() const noexcept { return x; }
		inline float GetY() const noexcept { return y; }

		virtual std::string ToString() const override;
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

		virtual std::string ToString() const override;
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

		virtual std::string ToString() const override;
	private:
		bool enter;
	};
}
