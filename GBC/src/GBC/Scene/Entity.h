#pragma once

#include "entt/entt.hpp"

namespace gbc
{
	class Scene;

	class Entity
	{
	public:
		Entity() = default;
		Entity(entt::entity handle, Scene* context);

		template<typename T, typename... Args>
		T& Add(Args&&... args)
		{
			GBC_CORE_ASSERT(!Has<T>(), "Entity already has component!");
			return context->registry.emplace<T>(handle, std::forward<Args>(args)...);
		}

		template<typename T>
		T& Remove()
		{
			GBC_CORE_ASSERT(Has<T>(), "Entity does not have component!");
			return context->registry.remove<T>(handle);
		}

		template<typename T>
		T& Get()
		{
			GBC_CORE_ASSERT(Has<T>(), "Entity does not have component!");
			return context->registry.get<T>(handle);
		}

		template<typename T>
		bool Has()
		{
			return context->registry.has<T>(handle);
		}

		inline operator bool() const { return handle != entt::null; }
		inline operator ENTT_ID_TYPE() const { return static_cast<ENTT_ID_TYPE>(handle); }
		inline operator entt::entity() const { return handle; }

		inline bool operator==(const Entity& entity) const { return handle == entity.handle && context == entity.context; }
		inline bool operator!=(const Entity& entity) const { return !(*this == entity); }
	private:
		entt::entity handle = entt::null;
		Scene* context = nullptr;
	};
}
