#pragma once

#include "GBC/Core/Core.h"
#include "GBC/Scene/Scene.h"
#include "GBC/Scene/Components/IDComponent.h"
#include "GBC/Scene/Components/RelationshipComponent.h"
#include "GBC/Scene/Components/TagComponent.h"
#include "GBC/Scene/Components/TransformComponent.h"
#include <entt/entt.hpp>

namespace gbc
{
	class Entity
	{
	public:
		constexpr Entity() noexcept = default;
		constexpr Entity(const Entity&) noexcept = default;
		constexpr Entity(entt::entity handle, Scene* context) noexcept : handle(handle), context(context) {}
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
		constexpr operator bool() const noexcept;
		constexpr operator uint32_t() const noexcept;
		constexpr operator entt::entity() const noexcept;
	public:
		constexpr bool operator==(const Entity& entity) const noexcept;
		constexpr bool operator!=(const Entity& entity) const noexcept;
	private:
		void GetAbsoluteTransformInternal(glm::mat4& transform) const;
		void RemoveParentInternal(Entity currentParent, RelationshipComponent& currentParentRelationship, RelationshipComponent& childRelationship);
	private:
		entt::entity handle = entt::null;
		Scene* context = nullptr;
	};
}

#include "Entity.inl"
