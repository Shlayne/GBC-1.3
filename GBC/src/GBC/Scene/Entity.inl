namespace gbc
{
	template<typename Component, typename... Args>
	Component& Entity::Add(Args&&... args)
	{
		GBC_CORE_ASSERT(!Has<Component>(), "Entity already has component!");
		Component& component = context->registry->emplace<Component>(handle, std::forward<Args>(args)...);
		context->OnComponentAdded(*this, component);
		return component;
	}

	template<typename Component, typename... Args>
	Component& Entity::AddOrReplace(Args&&... args)
	{
		if (Has<Component>())
		{
			context->OnComponentRemoved(*this, context->registry->get<Component>(handle));
			Component& component = context->registry->replace<Component>(handle, std::forward<Args>(args)...);
			context->OnComponentAdded(*this, component);
			return component;
		}
		else
		{
			Component& component = context->registry->emplace<Component>(handle, std::forward<Args>(args)...);
			context->OnComponentAdded(*this, component);
			return component;
		}
	}

	template<typename Component>
	Component& Entity::AddOrGet()
	{
		if (Has<Component>())
			return context->registry->get<Component>(handle);

		Component& component = context->registry->emplace<Component>(handle);
		context->OnComponentAdded(*this, component);
		return component;
	}

	template<typename Component>
	void Entity::Remove()
	{
		GBC_CORE_ASSERT(Has<Component>(), "Entity does not have component to remove!");
		context->OnComponentRemoved(*this, context->registry->get<Component>(handle));
		context->registry->remove<Component>(handle);
	}

	template<typename Component>
	Component& Entity::Get()
	{
		GBC_CORE_ASSERT(Has<Component>(), "Entity does not have component to get!");
		return context->registry->get<Component>(handle);
	}

	template<typename Component>
	const Component& Entity::Get() const
	{
		GBC_CORE_ASSERT(Has<Component>(), "Entity does not have component to get!");
		return context->registry->get<Component>(handle);
	}

	template<typename Component>
	bool Entity::Has() const
	{
		return context->registry->has<Component>(handle);
	}

	inline UUID Entity::GetUUID() const { return Get<IDComponent>().id; }
	inline std::string& Entity::GetName() { return Get<TagComponent>().tag; }
	inline const std::string& Entity::GetName() const { return Get<TagComponent>().tag; }
	inline TransformComponent& Entity::GetTransform() { return Get<TransformComponent>(); }
	inline const TransformComponent& Entity::GetTransform() const { return Get<TransformComponent>(); }

	inline Entity::operator bool() const noexcept { return handle != entt::null && context != nullptr; }
	inline Entity::operator uint32_t() const noexcept { return static_cast<uint32_t>(handle); }
	inline Entity::operator entt::entity() const noexcept { return handle; }

	inline bool Entity::operator==(const Entity& entity) const noexcept { return handle == entity.handle && context == entity.context; }
	inline bool Entity::operator!=(const Entity& entity) const noexcept { return !(*this == entity); }
}
