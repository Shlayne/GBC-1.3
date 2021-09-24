#pragma once

#include "GBC/Core/Timestep.h"
#include "GBC/Scene/Entity.h"

namespace gbc
{
	class ScriptableEntity
	{
	public:
		virtual ~ScriptableEntity() = default;

		template<typename T, typename... Args>
		T& AddComponent(Args&&... args)
		{
			return entity.AddComponent<T>(std::forward<Args>(args)...);
		}

		template<typename T>
		void RemoveComponent()
		{
			entity.RemoveComponent<T>();
		}

		template<typename T>
		T& GetComponent()
		{
			return entity.GetComponent<T>();
		}

		template<typename T>
		bool HasComponent() const
		{
			return entity.HasComponent<T>();
		}
	protected:
		virtual void OnCreate() {}
		virtual void OnDestroy() {}
		virtual void OnUpdate(Timestep timestep) {}
	private:
		Entity entity;
		friend class Scene;
	};
}
