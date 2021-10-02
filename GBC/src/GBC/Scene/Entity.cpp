#include "gbcpch.h"
#include "Entity.h"

namespace gbc
{
	Entity::Entity(entt::entity handle, Scene* context) noexcept
		: handle(handle), context(context) {}

	glm::mat4 Entity::GetAbsoluteTransform() const
	{
		glm::mat4 transform{ 1.0f };
		GetAbsoluteTransformInternal(transform);
		return transform;
	}

	void Entity::GetAbsoluteTransformInternal(glm::mat4& transform) const
	{
		const auto& relationship = Get<RelationshipComponent>();

		if (relationship.parent != entt::null)
		{
			Entity entity{ relationship.parent, context };
			entity.GetAbsoluteTransformInternal(transform);
		}

		transform *= (glm::mat4)GetTransform();
	}

	bool Entity::HasParent() const
	{
		return Get<RelationshipComponent>().parent != entt::null;
	}

	Entity Entity::GetParent() const
	{
		return { Get<RelationshipComponent>().parent, context };
	}

	void Entity::SetParent(Entity newParent)
	{
		if (!newParent)
			return RemoveParent();

		GBC_CORE_ASSERT(context == newParent.context, "Entity's parent must be in the same context!");
		GBC_CORE_ASSERT(handle != newParent.handle, "Entity's parent must not be itself!");

		Entity oldParent = GetParent();
		if (oldParent)
		{
			if (oldParent == newParent)
				return;

			auto& oldParentRelationship = oldParent.Get<RelationshipComponent>();
			auto& childRelationship = Get<RelationshipComponent>();
			RemoveParentInternal(oldParent, oldParentRelationship, childRelationship);
		}

		auto& newParentRelationship = newParent.Get<RelationshipComponent>();
		auto& childRelationship = Get<RelationshipComponent>();
		if (newParentRelationship.head == entt::null)
		{
			newParentRelationship.head = handle;
			childRelationship.prev = handle;
		}
		else
		{
			auto& newParentHeadRelationship = context->registry->get<RelationshipComponent>(newParentRelationship.head);
			auto& newParentHeadPrevRelationship = context->registry->get<RelationshipComponent>(newParentHeadRelationship.prev);
			newParentHeadPrevRelationship.next = handle;
			childRelationship.prev = newParentHeadRelationship.prev;
			newParentHeadRelationship.prev = handle;
		}

		childRelationship.parent = newParent.handle;

		context->OnParentChanged(*this, oldParent, newParent);
	}

	void Entity::RemoveParent()
	{
		Entity oldParent = GetParent();
		if (!oldParent)
			return;

		auto& oldParentRelationship = oldParent.Get<RelationshipComponent>();
		auto& childRelationship = Get<RelationshipComponent>();
		RemoveParentInternal(oldParent, oldParentRelationship, childRelationship);

		context->OnParentChanged(*this, oldParent, {});
	}

	bool Entity::HasRelationship() const
	{
		auto& relationship = Get<RelationshipComponent>();
		return relationship.parent != entt::null || relationship.head != entt::null;
	}

	void Entity::RemoveParentInternal(Entity currentParent, RelationshipComponent& currentParentRelationship, RelationshipComponent& childRelationship)
	{
		// since prev will never be null, but next will be at the end of the list, check if prev->next is null
		auto& childPrevRelationship = context->registry->get<RelationshipComponent>(childRelationship.prev);

		// only child
		if (childPrevRelationship.next == entt::null && childRelationship.next == entt::null)
		{
			currentParentRelationship.head = entt::null;
		}
		// only next sibling, this == parent->head
		else if (childPrevRelationship.next == entt::null)
		{
			auto& childNextRelationship = context->registry->get<RelationshipComponent>(childRelationship.next);
			currentParentRelationship.head = childRelationship.next;
			childNextRelationship.prev = childRelationship.prev;
			childRelationship.next = entt::null;
		}
		// only prev sibling, this == tail
		else if (childRelationship.next == entt::null)
		{
			auto& currentParentHeadRelationship = context->registry->get<RelationshipComponent>(currentParentRelationship.head);
			currentParentHeadRelationship.prev = childRelationship.prev;
			childPrevRelationship.next = entt::null;
		}
		// both prev and next siblings
		else
		{
			auto& childNextRelationship = context->registry->get<RelationshipComponent>(childRelationship.next);
			childPrevRelationship.next = childRelationship.next;
			childNextRelationship.prev = childRelationship.prev;
			childRelationship.next = entt::null;
		}

		childRelationship.parent = entt::null;
		childRelationship.prev = entt::null;
	}

	bool Entity::HasChildren() const
	{
		return Get<RelationshipComponent>().head != entt::null;
	}

	Entity Entity::GetFirstChild() const
	{
		return { Get<RelationshipComponent>().head, context };
	}

	Entity Entity::GetNextSibling() const
	{
		return { Get<RelationshipComponent>().next, context };
	}

	Entity Entity::GetPreviousSibling() const
	{
		return { Get<RelationshipComponent>().prev, context };
	}

	bool Entity::IsChildOf(Entity entity) const
	{
		return GetParent() == entity;
	}

	bool Entity::IsSubChildOf(Entity entity) const
	{
		if (!HasParent())
			return false;
		Entity parent = GetParent();
		if (parent == entity)
			return true;
		return parent.IsSubChildOf(entity);
	}
}
