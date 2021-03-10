#pragma once

#include "Event.h"
#include "GBC/Core/Mousecodes.h"

namespace gbc
{
	class MouseButtonPressEvent : public Event
	{
	public:
		MouseButtonPressEvent(Mousecode button, int mods);

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

		inline bool HasEntered() const { return enter; }

		virtual std::string ToString() const override;
	private:
		bool enter;
	};
}
