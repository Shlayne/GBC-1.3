#pragma once

#include <glm/glm.hpp>
#include <string>
#include "GBC/Core/Timestep.h"
#include "GBC/Events/Event.h"
#include "GBC/Events/WindowEvents.h"
#include "GBC/Rendering/EditorCamera.h"

// Forward declare entt stuff so we don't have to unclude the entire header file
namespace entt
{
	template<typename Entity>
	class basic_registry;

	using id_type = std::uint32_t;
	enum class entity : id_type;

	using registry = basic_registry<entity>;
}

class b2World;

namespace gbc
{
	class Entity;
	struct TransformComponent;
	struct Rigidbody2DComponent;

	class Scene
	{
	public:
		Scene();
		~Scene();

		Entity CreateEntity(const std::string& tag = {});
		void DestroyEntity(Entity entity);

		void OnRuntimePlay();
		void OnRuntimeStop();
		void OnRuntimeUpdate(Timestep timestep);
		void OnRuntimeRender();
		void OnEditorUpdate(Timestep timestep);
		void OnEditorRender(const EditorCamera& camera);

		void OnViewportResize(int32_t width, int32_t height);
		inline const glm::ivec2& GetViewportSize() const noexcept { return viewportSize; }

		Entity GetPrimaryCameraEntity();
	private:
		template<typename T>
		void OnComponentAdded(Entity entity, T& component);
		template<typename T>
		void OnComponentRemoved(Entity entity, T& component);

		entt::registry* registry;
		glm::ivec2 viewportSize;
	private:
		void UpdatePhysicsEntity(Entity entity);
		void InitializePhysicsEntityRigidbody2D(Entity entity);
		void InitializePhysicsEntityBoxCollider2D(Entity entity, TransformComponent& transform, Rigidbody2DComponent& rigidbody);
		void DestroyPhysicsEntityRigidbody2D(Entity entity);
		void DestroyPhysicsEntityBoxCollider2D(Entity entity, Rigidbody2DComponent& rigidbody);

		b2World* physicsWorld = nullptr;
	private:
		friend class Entity;
		friend class SceneSerializer;
		friend class SceneHierarchyPanel;
	};
}
