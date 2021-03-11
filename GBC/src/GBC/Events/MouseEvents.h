#pragma once

#include "Event.h"
#include "GBC/Core/Mousecodes.h"

namespace gbc
{
	class MouseButtonPressEvent : public Event
	{
	public:
		MouseButtonPressEvent(Mousecode button, int mods);
		EVENT_CLASS_TYPE(MouseButtonPress)

		inline Mousecode GetButton() const { return button; }
		inline int GetMods() const { return mods; }

		virtual std::string ToString() const override;
	private:
		Mousecode button;
		int mods;
	};

	class MouseButtonReleaseEvent : public Event
	{
	public:
		MouseButtonReleaseEvent(Mousecode button, int mods);
		EVENT_CLASS_TYPE(MouseButtonRelease)

		inline Mousecode GetButton() const { return button; }
		inline int GetMods() const { return mods; }

		virtual std::string ToString() const override;
	private:
		Mousecode button;
		int mods;
	};

	class MouseMoveEvent : public Event
	{
	public:
		MouseMoveEvent(float x, float y);
		EVENT_CLASS_TYPE(MouseMove)

		inline float GetX() const { return x; }
		inline float GetY() const { return y; }

		virtual std::string ToString() const override;
	private:
		float x;
		float y;
	};

	class MouseScrollEvent : public Event
	{
	public:
		MouseScrollEvent(float offsetX, float offsetY);
		EVENT_CLASS_TYPE(MouseScroll)

		inline float GetOffsetX() const { return offsetX; }
		inline float GetOffsetY() const { return offsetY; }

		virtual std::string ToString() const override;
	private:
		float offsetX;
		float offsetY;
	};

	class MouseEnterEvent : public Event
	{
	public:
		MouseEnterEvent(bool enter);
		EVENT_CLASS_TYPE(MouseEnter)

		inline bool HasEntered() const { return enter; }

		virtual std::string ToString() const override;
	private:
		bool enter;
	};
}
