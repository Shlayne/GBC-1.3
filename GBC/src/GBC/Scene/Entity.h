#pragma once

#include <entt/entt.hpp>
#include "GBC/Core/Core.h"
#include "GBC/Scene/Scene.h"
#include "GBC/Scene/Components/IDComponent.h"
#include "GBC/Scene/Components/RelationshipComponent.h"
#include "GBC/Scene/Components/TagComponent.h"
#include "GBC/Scene/Components/TransformComponent.h"

namespace gbc
{
	class Entity
	{
	public:
		Entity() = default;
		Entity(const Entity&) = default;
		Entity(entt::entity handle, Scene* context) noexcept;
	public:
		template<typename Component, typename... Args> Component& Add(Args&&... args);
		template<typename Component, typename... Args> Component& AddOrReplace(Args&&... args);
		template<typename Component> Component& AddOrGet();
		template<typename Component> void Remove();
		template<typename Component> Component& Get();
		template<typename Component> const Component& Get() const;
		template<typename Component> bool Has() const;
	public:
		inline UUID GetUUID() const;
		inline std::string& GetName();
		inline const std::string& GetName() const;
		inline TransformComponent& GetTransform();
		inline const TransformComponent& GetTransform() const;
		glm::mat4 GetAbsoluteTransform() const;
	public:
		bool HasParent() const;
		Entity GetParent() const;
		void SetParent(Entity newParent);
		void RemoveParent();
		bool HasRelationship() const;
		bool HasChildren() const;
		Entity GetFirstChild() const;
		Entity GetNextSibling() const;
		Entity GetPreviousSibling() const;
		bool IsChildOf(Entity entity) const;
		bool IsSubChildOf(Entity entity) const;
	public:
		inline operator bool() const noexcept;
		inline operator uint32_t() const noexcept;
		inline operator entt::entity() const noexcept;
	public:
		inline bool operator==(const Entity& entity) const noexcept;
		inline bool operator!=(const Entity& entity) const noexcept;
	private:
		void GetAbsoluteTransformInternal(glm::mat4& transform) const;
		void RemoveParentInternal(Entity currentParent, RelationshipComponent& currentParentRelationship, RelationshipComponent& childRelationship);
	private:
		entt::entity handle = entt::null;
		Scene* context = nullptr;
	};
}

#include "Entity.inl"
