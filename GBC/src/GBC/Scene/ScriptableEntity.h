#pragma once

#include "GBC/Core/Timestep.h"
#include "GBC/Scene/Entity.h"

namespace gbc
{
	class ScriptableEntity
	{
	public:
		virtual ~ScriptableEntity() = default;
	public:
		template<typename Component, typename... Args> Component& Add(Args&&... args);
		template<typename Component, typename... Args> Component& AddOrReplace(Args&&... args);
		template<typename Component> Component& AddOrGet();
		template<typename Component> void Remove();
		template<typename Component> Component& Get();
		template<typename Component> const Component& Get() const;
		template<typename Component> bool Has() const;
	public:
		inline UUID GetUUID();
		inline std::string& GetName();
		inline const std::string& GetName() const;
		inline TransformComponent& GetTransform();
		inline const TransformComponent& GetTransform() const;
		inline glm::mat4 GetAbsoluteTransform() const;
	public:
		inline bool HasParent() const;
		inline Entity GetParent() const;
		inline void SetParent(Entity newParent);
		inline void RemoveParent();
		inline bool HasChildren() const;
		inline Entity GetFirstChild() const;
		inline Entity GetNextSibling() const;
		inline Entity GetPreviousSibling() const;
		inline bool IsChildOf(Entity entity) const;
		inline bool IsSubChildOf(Entity entity) const;
	protected:
		virtual void OnCreate() {}
		virtual void OnDestroy() {}
		virtual void OnUpdate(Timestep timestep) {}
	private:
		Entity entity;
		friend class Scene;
	};
}

#include "ScriptableEntity.inl"
