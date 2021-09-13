#pragma once

#include <entt/entt.hpp>
#include "GBC/Scene/Scene.h"

namespace gbc
{
	class Entity
	{
	public:
		Entity() = default;
		Entity(entt::entity handle, Scene* context) noexcept
			: handle(handle), context(context) {}

		template<typename T, typename... Args>
		T& AddComponent(Args&&... args)
		{
			GBC_CORE_ASSERT(!HasComponent<T>(), "Entity already has component!");
			T& component = context->registry.emplace<T>(handle, std::forward<Args>(args)...);
			context->OnComponentAdded(*this, component);
			return component;
		}

		template<typename T>
		void RemoveComponent()
		{
			GBC_CORE_ASSERT(HasComponent<T>(), "Entity does not have component!");
			context->registry.remove<T>(handle);
		}

		template<typename T>
		T& GetComponent()
		{
			GBC_CORE_ASSERT(HasComponent<T>(), "Entity does not have component!");
			return context->registry.get<T>(handle);
		}

		template<typename T>
		bool HasComponent()
		{
			return context->registry.has<T>(handle);
		}

		inline operator bool() const noexcept { return handle != entt::null; }
		inline operator uint32_t() const noexcept { return static_cast<uint32_t>(handle); }
		inline operator entt::entity() const noexcept { return handle; }

		inline bool operator==(const Entity& entity) const noexcept { return handle == entity.handle && context == entity.context; }
		inline bool operator!=(const Entity& entity) const noexcept { return !(*this == entity); }
	private:
		entt::entity handle = entt::null;
		Scene* context = nullptr;
		friend class Scene;
	};
}
