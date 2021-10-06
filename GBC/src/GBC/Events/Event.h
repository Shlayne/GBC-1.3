#pragma once

#include "GBC/Core/Core.h"
#include <functional>
#if GBC_ENABLE_LOGGING
	#include <string>
#endif

namespace gbc
{
	enum class EventType
	{
		WindowClose, WindowResize, WindowMove, WindowFocus, WindowMinimize, WindowMaximize, 
			WindowDrop, WindowFramebufferResize, WindowContentScale, WindowRefresh,
		KeyPress, KeyRepeat, KeyRelease, KeyChar, KeyCharMods,
		MouseButtonPress, MouseButtonRelease, MouseMove, MouseScroll, MouseEnter,
		JoystickConnect
	};

	using EventCategoryFlags = uint32_t;
	enum EventCategory : EventCategoryFlags
	{
		EventCategory_None = 0,
		EventCategory_Application = 1 << 0,
		EventCategory_Window      = 1 << 1,
		EventCategory_Keyboard    = 1 << 2,
		EventCategory_Mouse       = 1 << 3,
		EventCategory_MouseButton = 1 << 4,
		EventCategory_Device      = 1 << 5
	};

#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return type; }\
							   virtual EventType GetType() const override { return GetStaticType(); }

#define EVENT_CLASS_CATEGORY(categories) virtual EventCategoryFlags GetCategoryFlags() const override { return categories; }

	class Event
	{
	public:
		virtual ~Event() = default;

		virtual EventType GetType() const = 0;
		virtual EventCategoryFlags GetCategoryFlags() const = 0;
		inline bool IsInCategory(EventCategoryFlags category) const noexcept;

#if GBC_ENABLE_LOGGING
		virtual std::string ToString() const = 0;
#endif

		bool handled = false;
	};

	class EventDispatcher
	{
	public:
		EventDispatcher(Event& event) : event(event) {}
	public:
		template<typename C, typename E> bool Dispatch(C* object, bool(C::* func)(E&));
		template<typename E> bool Dispatch(bool(*func)(E&));
	private:
		Event& event;
	};

	using EventCallbackFunc = std::function<void(Event&)>;
}

#include "Event.inl"
