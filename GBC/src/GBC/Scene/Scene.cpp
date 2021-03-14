#include "gbcpch.h"
#include "Scene.h"
#include "GBC/Core/Application.h"
#include "GBC/Rendering/Basic/BasicRenderer.h"
#include "GBC/Rendering/Renderer.h"
#include "Components/TransformComponent.h"
#include "Components/TagComponent.h"
#include "Components/MeshComponent.h"
#include "Components/RenderableComponent.h"
#include "Components/CameraComponent.h"
#include "Components/NativeScriptComponent.h"

namespace gbc
{
	Scene::Scene()
	{
		GBC_PROFILE_FUNCTION();

		Window& window = Application::Get().GetWindow();
		viewportSize = {window.GetWidth(), window.GetHeight()};
	}

	Scene::~Scene()
	{
		GBC_PROFILE_FUNCTION();
	}

	Entity Scene::CreateEntity(const std::string& tag)
	{
		GBC_PROFILE_FUNCTION();

		Entity entity(registry.create(), this);
		entity.Add<TransformComponent>();
		entity.Add<TagComponent>(tag.empty() ? "Unknown Entity" : tag);
		return entity;
	}

	void Scene::OnCreate()
	{
		GBC_PROFILE_FUNCTION();

		registry.view<NativeScriptComponent>().each([=](entt::entity entity, NativeScriptComponent& nsc)
		{
			nsc.instance = nsc.createFunc();
			nsc.instance->entity = {entity, this};
			nsc.instance->OnCreate();
		});
	}

	void Scene::OnDestroy()
	{
		GBC_PROFILE_FUNCTION();

		registry.view<NativeScriptComponent>().each([=](entt::entity entity, NativeScriptComponent& nsc)
		{
			nsc.instance->OnDestroy();
			nsc.destroyFunc(&nsc);
		});
	}

	void Scene::OnUpdate(Timestep timestep)
	{
		GBC_PROFILE_FUNCTION();

		if (shouldResizeCameras)
		{
			GBC_PROFILE_SCOPE("Resize Cameras - Scene::OnUpdate(Timestep)");

			shouldResizeCameras = false;

			auto view = registry.view<CameraComponent>();
			for (auto entity : view)
			{
				auto& camera = view.get<CameraComponent>(entity);
				if (!camera.fixedAspectRatio)
					camera.camera.OnViewportResize(viewportSize.x, viewportSize.y);
			}
		}

		{
			GBC_PROFILE_SCOPE("Update Native Script Components - Scene::OnUpdate(Timestep)");

			registry.view<NativeScriptComponent>().each([=](entt::entity entity, NativeScriptComponent& nsc)
			{
				nsc.instance->OnUpdate(timestep);
			});
		}
	}

	void Scene::OnRender()
	{
		GBC_PROFILE_FUNCTION();

		Renderer::SetClearColor({0.1f, 0.1f, 0.1f, 1.0f});
		Renderer::Clear();

		// Get camera
		glm::mat4* primaryCameraTransform = nullptr;
		Camera* primaryCamera = nullptr;
		{
			auto group = registry.group<TransformComponent>(entt::get<CameraComponent>);
			for (auto entity : group)
			{
				auto [transform, camera] = group.get<TransformComponent, CameraComponent>(entity);

				if (camera.primary)
				{
					primaryCameraTransform = &transform.transform;
					primaryCamera = &camera.camera;
					break;
				}
			}
		}

		if (primaryCamera != nullptr)
		{
			BasicRenderer::BeginScene(*primaryCamera, *primaryCameraTransform);

			auto group = registry.group<MeshComponent>(entt::get<RenderableComponent>);
			for (auto entity : group)
			{
				auto [mesh, renderable] = group.get<MeshComponent, RenderableComponent>(entity);
				BasicRenderer::Submit(mesh, glm::mat4(1.0f), renderable);
			}

			BasicRenderer::EndScene();
		}
	}

	void Scene::OnEvent(Event& event)
	{
		GBC_PROFILE_FUNCTION();

		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<WindowResizeEvent>(GBC_BIND_FUNC(OnWindowResizeEvent));
	}

	bool Scene::OnWindowResizeEvent(WindowResizeEvent& event)
	{
		shouldResizeCameras = true;
		viewportSize.x = event.GetWidth();
		viewportSize.y = event.GetHeight();
		return false;
	}
}
