#include "gbcpch.h"
#include "Scene.h"
#include "GBC/Rendering/Basic/BasicRenderer.h"
#include "GBC/Rendering/Renderer.h"
#include "Components/TransformComponent.h"
#include "Components/RenderableComponent.h"
#include "Components/MeshComponent.h"

namespace gbc
{
	Scene::Scene()
	{
		GBC_PROFILE_FUNCTION();

	}

	Scene::~Scene()
	{
		GBC_PROFILE_FUNCTION();

	}

	Entity Scene::CreateEntity()
	{
		Entity entity(registry.create(), this);
		entity.Add<TransformComponent>();
		return entity;
	}

	void Scene::OnUpdate(Timestep timestep)
	{
		GBC_PROFILE_FUNCTION();
	}

	void Scene::OnRender()
	{
		GBC_PROFILE_FUNCTION();

		Renderer::SetClearColor({0.1f, 0.1f, 0.1f, 1.0f});
		Renderer::Clear();

		auto group = registry.group<MeshComponent>(entt::get<RenderableComponent>);
		for (auto entity : group)
		{
			auto& [mesh, renderable] = group.get<MeshComponent, RenderableComponent>(entity);
			BasicRenderer::Submit(mesh, glm::mat4(1.0f), renderable);
		}
	}
}
