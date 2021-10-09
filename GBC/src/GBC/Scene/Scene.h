#pragma once

#include "GBC/Core/Timestep.h"
#include "GBC/Core/UUID.h"
#include "GBC/Events/Event.h"
#include "GBC/Events/WindowEvents.h"
#include "GBC/Rendering/EditorCamera.h"
#include <glm/glm.hpp>
#include <string>
#include <unordered_map>

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

		static Ref<Scene> Copy(const Ref<Scene>& scene);
	private:
		static Entity CopyEntity(entt::entity handle, entt::registry* source, Scene* sourceScene, Ref<Scene>& newScene, std::unordered_map<UUID, entt::entity>& entities, bool hasParent);
	public:
		Entity CreateEntity(UUID uuid, const std::string& tag = {});
		Entity CreateEntity(const std::string& tag = {});
		Entity DuplicateEntity(Entity entity);
		Entity GetExistingEntity(UUID uuid);
		void DestroyEntity(Entity entity);
	private:
		void RemoveEntity(UUID uuid);
	public:
		void OnRuntimePlay();
		void OnRuntimeStop();
		void OnRuntimeUpdate(Timestep timestep);
		// TODO: move these to SceneRenderer
		void OnRuntimeRender();
		void OnEditorRender(const EditorCamera& camera);
	public:
		void OnViewportResize(int32_t width, int32_t height);
		inline const glm::ivec2& GetViewportSize() const noexcept { return viewportSize; }

		Entity GetPrimaryCameraEntity();
	private:
		void Render(const glm::mat4& viewProjection);
	private:
		template<typename T>
		void OnComponentAdded(Entity entity, T& component);
		template<typename T>
		void OnComponentRemoved(Entity entity, T& component);

		void OnParentChanged(Entity child, Entity oldParent, Entity newParent);

		entt::registry* registry;
		glm::ivec2 viewportSize;
		std::vector<entt::entity> entities;
	private:
		void UpdatePhysicsEntity(Entity entity);
		void InitializePhysicsEntityRigidbody2D(Entity entity);
		void InitializePhysicsEntityBoxCollider2D(Entity entity, const glm::vec2& scale, Rigidbody2DComponent& rigidbody);
		void DestroyPhysicsEntityRigidbody2D(Entity entity);
		void DestroyPhysicsEntityBoxCollider2D(Entity entity, Rigidbody2DComponent& rigidbody);

		b2World* physicsWorld = nullptr;
	private:
		friend class Entity;
		friend class SceneSerializer;
		friend class SceneHierarchyPanel;
	};
}
