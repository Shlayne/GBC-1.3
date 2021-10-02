namespace gbc
{
	inline bool Event::IsInCategory(EventCategoryFlags category) const noexcept { return GetCategoryFlags() & category; }

	EventDispatcher::EventDispatcher(Event& event) : event(event) {}

	template<typename C, typename E>
	bool EventDispatcher::Dispatch(C* object, bool(C::* func)(E&))
	{
		if (event.GetType() == E::GetStaticType())
		{
			event.handled |= (object->*func)(static_cast<E&>(event));
			return true;
		}
		return false;
	}

	template<typename E>
	bool EventDispatcher::Dispatch(bool(*func)(E&))
	{
		if (event.GetType() == E::GetStaticType())
		{
			event.handled |= func(static_cast<E&>(event));
			return true;
		}
		return false;
	}
}
