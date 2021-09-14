#pragma once

#include <entt/entt.hpp>
#include <glm/glm.hpp>
#include <string>
#include "GBC/Core/Timestep.h"
#include "GBC/Events/Event.h"
#include "GBC/Events/WindowEvents.h"
#include "GBC/Rendering/EditorCamera.h"

namespace gbc
{
	class Entity;

	class Scene
	{
	public:
		Scene();
		~Scene();

		Entity CreateEntity(const std::string& tag = {});
		void DestroyEntity(Entity entity);

		void OnPlay();
		void OnStop();
		void OnUpdateRuntime(Timestep timestep);
		void OnRenderRuntime();
		void OnUpdateEditor(Timestep timestep);
		void OnRenderEditor(const EditorCamera& camera);

		void OnViewportResize(int32_t width, int32_t height);
		inline const glm::ivec2& GetViewportSize() const noexcept { return viewportSize; }

		Entity GetPrimaryCameraEntity();
	private:
		template<typename T>
		void OnComponentAdded(Entity entity, T& component);

		entt::registry registry;

		glm::ivec2 viewportSize;

		friend class Entity;
		friend class SceneSerializer;
		friend class SceneHierarchyPanel;
	};
}
