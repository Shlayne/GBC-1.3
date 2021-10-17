#include "gbcpch.h"
#include "Scene.h"
#include "GBC/Core/Application.h"
#include "GBC/Rendering/Renderer.h"
#include "GBC/Rendering/Renderers/Renderer2D.h"
#include "GBC/Rendering/Renderers/Renderer3D.h"
#include "GBC/Scene/Entity.h"
#include "GBC/Scene/ScriptableEntity.h"
#include "GBC/Scene/Components/AllComponents.h"
#include "GBC/Math/Math.h"
#include <box2d/b2_body.h>
#include <box2d/b2_fixture.h>
#include <box2d/b2_polygon_shape.h>
#include <box2d/b2_world.h>
#include <entt/entt.hpp>

namespace gbc
{
	static b2BodyType Rigidbody2DTypeToB2BodyType(Rigidbody2DComponent::BodyType type)
	{
		switch (type)
		{
			case Rigidbody2DComponent::BodyType::Static:    return b2BodyType::b2_staticBody;
			case Rigidbody2DComponent::BodyType::Dynamic:   return b2BodyType::b2_dynamicBody;
			case Rigidbody2DComponent::BodyType::Kinematic: return b2BodyType::b2_kinematicBody;
		}

		GBC_CORE_ASSERT(false, "Unknown Rigidbody 2D Component Type!");
		return static_cast<b2BodyType>(0);
	}

	Scene::Scene()
	{
		GBC_PROFILE_FUNCTION();

		Window& window = Application::Get().GetWindow();
		viewportSize = { window.GetWidth(), window.GetHeight() };

		registry = new entt::registry();
	}

	Scene::~Scene()
	{
		GBC_PROFILE_FUNCTION();

		if (physicsWorld)
		{
			delete physicsWorld;
			physicsWorld = nullptr; // TODO: not sure if this is necessary for smart pointer usage but whatever.
		}
		delete registry;
	}

	template<typename... Component>
	static void CopyComponent(ComponentGroup<Component...>, entt::registry* destination, entt::registry* source, Scene* scene, const std::unordered_map<UUID, entt::entity>& entities)
	{
		([&]()
		{
			auto view = source->view<Component>();
			for (auto handle : view)
			{
				UUID uuid = source->get<IDComponent>(handle).id;
				GBC_CORE_ASSERT(entities.find(uuid) != entities.cend(), "Entity UUID not found when copying scene!");
				auto destinationHandle = entities.at(uuid);
				auto& sourceComponent = view.get<Component>(handle);
				destination->emplace_or_replace<Component>(destinationHandle, sourceComponent);
			}
		}(), ...);
	}
	inline static void CopyAllComponents(entt::registry* destination, entt::registry* source, Scene* scene, const std::unordered_map<UUID, entt::entity>& entities)
	{ CopyComponent(AllCopyableComponents{}, destination, source, scene, entities); }

	template<typename... Component>
	static void CopyComponentIfExists(ComponentGroup<Component...>, Entity destination, Entity source)
	{
		([&]()
		{
			if (source.Has<Component>())
				destination.AddOrReplace<Component>(source.Get<Component>());
		}(), ...);
	}
	inline static void CopyAllExistingComponents(Entity destination, Entity source)
	{ CopyComponentIfExists(AllCopyableComponents{}, destination, source); }

	Ref<Scene> Scene::Copy(const Ref<Scene>& scene)
	{
		GBC_PROFILE_FUNCTION();

		Ref<Scene> newScene = CreateRef<Scene>();
		newScene->viewportSize = scene->viewportSize;

		Scene* sourceScene = scene.get();
		entt::registry* destination = newScene->registry;
		entt::registry* source = scene->registry;
		std::unordered_map<UUID, entt::entity> entities;

		// Create new entities
		for (auto handle : scene->entities)
			CopyEntity(handle, source, sourceScene, newScene, entities, false);
		CopyAllComponents(destination, source, sourceScene, entities);

		return newScene;
	}

	Entity Scene::CopyEntity(entt::entity handle, entt::registry* source, Scene* sourceScene, Ref<Scene>& newScene, std::unordered_map<UUID, entt::entity>& entities, bool hasParent)
	{
		UUID uuid = source->get<IDComponent>(handle).id;
		const auto& name = source->get<TagComponent>(handle).tag;
		Entity newEntity = newScene->CreateEntity(uuid, name);
		entities[uuid] = newEntity;
		// TODO: find a better way to not add children to the entities list
		if (hasParent)
			newScene->entities.pop_back();

		Entity entity{ handle, sourceScene };

		// Copy child entities
		for (Entity child = entity.GetFirstChild(); child; child = child.GetNextSibling())
		{
			Entity newChild = CopyEntity(child, source, sourceScene, newScene, entities, true);
			newChild.SetParent(newEntity);
		}

		return newEntity;
	}

	Entity Scene::CreateEntity(UUID uuid, const std::string& tag)
	{
		GBC_PROFILE_FUNCTION();

		Entity entity{ registry->create(), this };
		entity.Add<IDComponent>(uuid);
		entity.Add<RelationshipComponent>();
		entity.Add<TagComponent>(tag.empty() ? "Unknown Entity" : tag);
		entity.Add<TransformComponent>();

		entities.push_back(entity);
		return entity;
	}

	Entity Scene::CreateEntity(const std::string& tag)
	{
		return CreateEntity(UUID(), tag);
	}

	Entity Scene::DuplicateEntity(Entity entity)
	{
		auto name = entity.GetName();
		Entity newEntity = CreateEntity(name);

		// Copy RelationshipComponent manually because it requires extra attention
		newEntity.SetParent(entity.GetParent());
		for (Entity child = entity.GetFirstChild(); child; child = child.GetNextSibling())
		{
			Entity newChild = DuplicateEntity(child);
			newChild.SetParent(newEntity);
		}

		CopyAllExistingComponents(newEntity, entity);

		return newEntity;
	}

	Entity Scene::GetExistingEntity(UUID uuid)
	{
		auto view = registry->view<IDComponent>();
		for (auto handle : view)
			if (uuid == registry->get<IDComponent>(handle).id)
				return { handle, this };
		return {};
	}

	void Scene::DestroyEntity(Entity entity)
	{
		GBC_PROFILE_FUNCTION();

		if (physicsWorld && entity.Has<Rigidbody2DComponent>())
			DestroyPhysicsEntityRigidbody2D(entity);

		for (Entity child = entity.GetFirstChild(), nextSibling; child; child = nextSibling)
		{
			nextSibling = child.GetNextSibling();
			DestroyEntity(child);
		}
		if (entity.HasParent())
			entity.RemoveParent();

		RemoveEntity(entity.GetUUID());
		registry->destroy(entity);
	}

	void Scene::RemoveEntity(UUID uuid)
	{
		for (auto it = entities.begin(); it != entities.end(); ++it)
		{
			if (uuid == registry->get<IDComponent>(*it).id)
			{
				entities.erase(it);
				break;
			}
		}
	}

	void Scene::OnRuntimePlay()
	{
		GBC_PROFILE_FUNCTION();

		registry->view<NativeScriptComponent>().each([this](entt::entity entity, NativeScriptComponent& nsc)
		{
			nsc.instance = nsc.createFunc();
			nsc.instance->entity = { entity, this };
			nsc.instance->OnCreate();
		});

		physicsWorld = new b2World({ 0.0f, -9.80665f });
		auto view = registry->view<Rigidbody2DComponent>();
		for (auto handle : view)
			InitializePhysicsEntityRigidbody2D({ handle, this });
	}

	void Scene::OnRuntimeStop()
	{
		GBC_PROFILE_FUNCTION();

		delete physicsWorld;
		physicsWorld = nullptr;

		registry->view<NativeScriptComponent>().each([](entt::entity entity, NativeScriptComponent& nsc)
		{
			nsc.instance->OnDestroy();
			nsc.destroyFunc(&nsc);
		});
	}

	void Scene::OnRuntimeUpdate(Timestep timestep)
	{
		GBC_PROFILE_FUNCTION();

		// Update scripts
		registry->view<NativeScriptComponent>().each([timestep = timestep](entt::entity entity, NativeScriptComponent& nsc)
		{
			nsc.instance->OnUpdate(timestep);
		});

		// Update physics
		{
			// TODO: Add these two to project physics settings
			int32_t velocityIterations = 6;
			int32_t positionIterations = 2;
			physicsWorld->Step(timestep, velocityIterations, positionIterations);

			auto view = registry->view<Rigidbody2DComponent>();
			for (auto handle : view)
				UpdatePhysicsEntity({ handle, this });
		}
	}

	void Scene::OnRuntimeRender()
	{
		GBC_PROFILE_FUNCTION();

		if (Entity primaryCamera = GetPrimaryCameraEntity())
		{
			const auto& camera = primaryCamera.Get<CameraComponent>();
			Render(camera.camera.GetProjection() * glm::inverse(primaryCamera.GetAbsoluteTransform()));
		}
	}

	void Scene::OnEditorRender(const EditorCamera& camera)
	{
		GBC_PROFILE_FUNCTION();

		Render(camera.GetViewProjection());
	}

	void Scene::Render(const glm::mat4& viewProjection)
	{
		// 2D Rendering
		{
			auto spriteView = registry->view<SpriteRendererComponent>();
			auto circleView = registry->view<CircleRendererComponent>();

			if (!spriteView.empty() || !circleView.empty())
			{
				Renderer2D::BeginScene(viewProjection);

				for (auto handle : spriteView)
				{
					Entity entity{ handle, this };
					glm::mat4 transform = entity.GetAbsoluteTransform();

					auto& sprite = spriteView.get<SpriteRendererComponent>(handle);
					Renderer2D::DrawQuad(transform, sprite);
				}

				for (auto handle : circleView)
				{
					Entity entity{ handle, this };
					glm::mat4 transform = entity.GetAbsoluteTransform();

					auto& circle = circleView.get<CircleRendererComponent>(handle);
					Renderer2D::DrawCircle(transform, circle);
				}

				Renderer2D::EndScene();
			}
		}

		// 3D Rendering
		if (auto view = registry->view<Model3DComponent>(); !view.empty())
		{
			Renderer3D::BeginScene(viewProjection);

			for (auto handle : view)
			{
				Entity entity{ handle, this };
				glm::mat4 transform = entity.GetAbsoluteTransform();

				auto& model3DComponent = view.get<Model3DComponent>(handle);
				if (model3DComponent.model)
					Renderer3D::Submit(model3DComponent.model, transform);
			}

			Renderer3D::EndScene();
		}
	}

	void Scene::OnViewportResize(int32_t width, int32_t height)
	{
		GBC_PROFILE_FUNCTION();

		viewportSize.x = width;
		viewportSize.y = height;

		auto view = registry->view<CameraComponent>();
		for (auto handle : view)
		{
			auto& cameraComponent = view.get<CameraComponent>(handle);
			if (!cameraComponent.fixedAspectRatio)
				cameraComponent.camera.OnViewportResize(viewportSize.x, viewportSize.y);
		}
	}

	Entity Scene::GetPrimaryCameraEntity()
	{
		// TODO: primary camera entity should be stored in Scene, not in CameraComponent
		auto view = registry->view<CameraComponent>();
		for (auto handle : view)
			if (view.get<CameraComponent>(handle).primary)
				return { handle, this };
		return {};
	}

	void Scene::UpdatePhysicsEntity(Entity entity)
	{
		auto& transform = entity.GetTransform();
		auto& rigidbody = entity.Get<Rigidbody2DComponent>();

		b2Body* body = static_cast<b2Body*>(rigidbody.runtimeBody);
		const auto& position = body->GetPosition();
		// FIXME: some how, get the local transform from Box2D's absolute transform and set this entity's local transform
		transform.translation.x = position.x;
		transform.translation.y = position.y;
		transform.rotation.z = body->GetAngle();
	}

	void Scene::InitializePhysicsEntityRigidbody2D(Entity entity)
	{
		glm::mat4 transform = entity.GetTransform();
		glm::vec3 translation, rotation, scale;
		bool decomposed = Math::Decompose(transform, translation, rotation, scale);
		GBC_CORE_ASSERT(decomposed, "Unable to decompose physics entity's absolute transform!");

		// FIXME: Box2D needs to be given the absolute transform, but needs to give back the relative transform

		auto& rigidbody = entity.Get<Rigidbody2DComponent>();

		b2BodyDef bodyDef;
		bodyDef.type = Rigidbody2DTypeToB2BodyType(rigidbody.bodyType);
		bodyDef.position.Set(translation.x, translation.y);
		bodyDef.angle = rotation.z;

		b2Body* body = physicsWorld->CreateBody(&bodyDef);
		body->SetFixedRotation(rigidbody.fixedRotation);
		rigidbody.runtimeBody = body;

		if (entity.Has<BoxCollider2DComponent>())
			InitializePhysicsEntityBoxCollider2D(entity, scale, rigidbody);
	}

	void Scene::InitializePhysicsEntityBoxCollider2D(Entity entity, const glm::vec2& scale, Rigidbody2DComponent& rigidbody)
	{
		auto& collider = entity.Get<BoxCollider2DComponent>();
		if (collider.runtimeFixture == nullptr)
		{
			b2PolygonShape shape;
			glm::vec2 size = scale * collider.size;
			glm::vec2 offset = scale * collider.offset;
			shape.SetAsBox(size.x, size.y, { offset.x, offset.y }, 0.0f);

			b2FixtureDef fixtureDef;
			fixtureDef.shape = &shape;
			fixtureDef.density = collider.density;
			fixtureDef.friction = collider.friction;
			fixtureDef.restitution = collider.restitution;
			fixtureDef.restitutionThreshold = collider.restitutionThreshold;
			static_cast<b2Body*>(rigidbody.runtimeBody)->CreateFixture(&fixtureDef);
		}
	}

	void Scene::DestroyPhysicsEntityRigidbody2D(Entity entity)
	{
		auto& rigidbody = entity.Get<Rigidbody2DComponent>();

		physicsWorld->DestroyBody(static_cast<b2Body*>(rigidbody.runtimeBody));

		if (entity.Has<BoxCollider2DComponent>())
			DestroyPhysicsEntityBoxCollider2D(entity, rigidbody);
	}

	void Scene::DestroyPhysicsEntityBoxCollider2D(Entity entity, Rigidbody2DComponent& rigidbody)
	{
		auto body = static_cast<b2Body*>(rigidbody.runtimeBody);
		auto fixture = static_cast<b2Fixture*>(entity.Get<BoxCollider2DComponent>().runtimeFixture);
		body->DestroyFixture(fixture);
	}

	// OnComponentAdded
	template<typename T> void Scene::OnComponentAdded(Entity entity, T& component) { static_assert(false); }

	template<> void Scene::OnComponentAdded<CircleRendererComponent>(Entity entity, CircleRendererComponent& component) {}
	template<> void Scene::OnComponentAdded<CameraComponent>(Entity entity, CameraComponent& component)
	{
		component.camera.OnViewportResize(viewportSize.x, viewportSize.y);
	}
	template<> void Scene::OnComponentAdded<IDComponent>(Entity entity, IDComponent& component) {}
	template<> void Scene::OnComponentAdded<RelationshipComponent>(Entity entity, RelationshipComponent& component) {}
	template<> void Scene::OnComponentAdded<TagComponent>(Entity entity, TagComponent & component) {}
	template<> void Scene::OnComponentAdded<Model3DComponent>(Entity entity, Model3DComponent& component) {}
	template<> void Scene::OnComponentAdded<TransformComponent>(Entity entity, TransformComponent & component) {}
	template<> void Scene::OnComponentAdded<SpriteRendererComponent>(Entity entity, SpriteRendererComponent & component) {}
	template<> void Scene::OnComponentAdded<NativeScriptComponent>(Entity entity, NativeScriptComponent & component) {}

	// OnComponentAdded::Physics
	template<> void Scene::OnComponentAdded<BoxCollider2DComponent>(Entity entity, BoxCollider2DComponent& component)
	{
		if (physicsWorld && entity.Has<Rigidbody2DComponent>())
		{
			glm::mat4 transform = entity.GetAbsoluteTransform();
			glm::vec3 translation, rotation, scale;
			bool decomposed = Math::Decompose(transform, translation, rotation, scale);
			GBC_CORE_ASSERT(decomposed, "Unable to decompose physics entity's absolute transform!");

			auto& rigidbody = entity.Get<Rigidbody2DComponent>();
			InitializePhysicsEntityBoxCollider2D(entity, scale, rigidbody);
		}
	}
	template<> void Scene::OnComponentAdded<Rigidbody2DComponent>(Entity entity, Rigidbody2DComponent& component)
	{
		if (physicsWorld)
			InitializePhysicsEntityRigidbody2D(entity);
	}

	// OnComponentRemoved
	template<typename T> void Scene::OnComponentRemoved(Entity entity, T& component) { static_assert(false); }

	template<> void Scene::OnComponentRemoved<CircleRendererComponent>(Entity entity, CircleRendererComponent& component) {}
	template<> void Scene::OnComponentRemoved<CameraComponent>(Entity entity, CameraComponent& component) {}
	template<> void Scene::OnComponentRemoved<IDComponent>(Entity entity, IDComponent& component) {}
	template<> void Scene::OnComponentRemoved<Model3DComponent>(Entity entity, Model3DComponent& component) {}
	template<> void Scene::OnComponentRemoved<NativeScriptComponent>(Entity entity, NativeScriptComponent& component) {}
	template<> void Scene::OnComponentRemoved<RelationshipComponent>(Entity entity, RelationshipComponent& component) {}
	template<> void Scene::OnComponentRemoved<SpriteRendererComponent>(Entity entity, SpriteRendererComponent& component) {}
	template<> void Scene::OnComponentRemoved<TagComponent>(Entity entity, TagComponent& component) {}
	template<> void Scene::OnComponentRemoved<TransformComponent>(Entity entity, TransformComponent& component) {}

	// OnComponentRemoved::Physics
	template<> void Scene::OnComponentRemoved<BoxCollider2DComponent>(Entity entity, BoxCollider2DComponent& component)
	{
		if (physicsWorld && entity.Has<Rigidbody2DComponent>())
			DestroyPhysicsEntityBoxCollider2D(entity, entity.Get<Rigidbody2DComponent>());
	}
	template<> void Scene::OnComponentRemoved<Rigidbody2DComponent>(Entity entity, Rigidbody2DComponent& component)
	{
		if (physicsWorld)
			DestroyPhysicsEntityRigidbody2D(entity);
	}

	// child is always valid
	// if either oldParent or newParent are invalid, the other is valid
	void Scene::OnParentChanged(Entity child, Entity oldParent, Entity newParent)
	{
		if (!oldParent)
		{
			RemoveEntity(child.GetUUID());
		}
		else if (!newParent)
		{
			entities.push_back(child);
		}
		else
		{
			// for now do nothing
		}
	}
}
