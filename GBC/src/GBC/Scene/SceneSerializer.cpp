#include "gbcpch.h"
#include "SceneSerializer.h"
#include "GBC/Core/Application.h"
#include "GBC/Core/FileTypes.h"
#include "GBC/IO/CommonYAML.h"
#include "GBC/Model/MeshFactory3D.h"
#include "GBC/Scene/Entity.h"
#include "GBC/Scene/Components/CameraComponent.h"
#include "GBC/Scene/Components/CircleRendererComponent.h"
#include "GBC/Scene/Components/Model3DComponent.h"
#include "GBC/Scene/Components/NativeScriptComponent.h"
#include "GBC/Scene/Components/SpriteRendererComponent.h"
#include "GBC/Scene/Components/TagComponent.h"
#include "GBC/Scene/Components/TransformComponent.h"
#include "GBC/Scene/Components/Physics/BoxCollider2DComponent.h"
#include "GBC/Scene/Components/Physics/Rigidbody2DComponent.h"
#include <yaml-cpp/yaml.h>

namespace gbc
{
	static std::string Rigidbody2DComponentBodyTypeToString(Rigidbody2DComponent::BodyType rigidbody2DComponentType)
	{
		switch (rigidbody2DComponentType)
		{
			case Rigidbody2DComponent::BodyType::Static:    return "Static";
			case Rigidbody2DComponent::BodyType::Dynamic:   return "Dynamic";
			case Rigidbody2DComponent::BodyType::Kinematic: return "Kinematic";
		}

		GBC_CORE_ASSERT("Unknown Rigidbody 2D Component Body Type!");
		return std::string();
	}

	static Rigidbody2DComponent::BodyType Rigidbody2DComponentBodyTypeFromString(std::string_view string)
	{
		if (string == "Static")         return Rigidbody2DComponent::BodyType::Static;
		else if (string == "Dynamic")   return Rigidbody2DComponent::BodyType::Dynamic;
		else if (string == "Kinematic") return Rigidbody2DComponent::BodyType::Kinematic;

		GBC_CORE_ASSERT("Unknown Rigidbody 2D Component Body Type!");
		return static_cast<Rigidbody2DComponent::BodyType>(0);
	}

#pragma region serialization

	template<typename Component, typename Func>
	static void Serialize(YAML::Emitter& out, Entity entity, const char* name, Func func)
	{
		if (entity.Has<Component>())
		{
			out << YAML::Key << name << YAML::Value << YAML::BeginMap;
			func(out, entity.Get<Component>());
			out << YAML::EndMap;
		}
	}

	static void SerializeEntity(YAML::Emitter& out, Entity entity)
	{
		out << YAML::BeginMap
			<< YAML::Key << "Entity" << YAML::Value << entity.GetUUID();

		// An entity always has these two components in addition to a UUID
		Serialize<TagComponent>(out, entity, "TagComponent", [](YAML::Emitter& out, TagComponent& component)
		{
			out << YAML::Key << "Tag" << YAML::Value << component.tag;
		});
		Serialize<TransformComponent>(out, entity, "TransformComponent", [](YAML::Emitter& out, TransformComponent& component)
		{
			out << YAML::Key << "Translation" << YAML::Value << component.translation
				<< YAML::Key << "Rotation" << YAML::Value << component.rotation
				<< YAML::Key << "Scale" << YAML::Value << component.scale;
		});

		// Optional components
		Serialize<CameraComponent>(out, entity, "CameraComponent", [](YAML::Emitter& out, CameraComponent& component)
		{
			auto& camera = component.camera;
			out << YAML::Key << "Camera" << YAML::Value << YAML::BeginMap
				<< YAML::Key << "ProjectionType" << YAML::Value << SceneCameraProjectionTypeToString(camera.GetProjectionType())
				<< YAML::Key << "PerspectiveFOV" << YAML::Value << camera.GetPerspectiveFOV()
				<< YAML::Key << "PerspectiveNear" << YAML::Value << camera.GetPerspectiveNearClip()
				<< YAML::Key << "PerspectiveFar" << YAML::Value << camera.GetPerspectiveFarClip()
				<< YAML::Key << "OrthographicSize" << YAML::Value << camera.GetOrthographicSize()
				<< YAML::Key << "OrthographicNear" << YAML::Value << camera.GetOrthographicNearClip()
				<< YAML::Key << "OrthographicFar" << YAML::Value << camera.GetOrthographicFarClip()
				<< YAML::EndMap
				<< YAML::Key << "Primary" << YAML::Value << component.primary;
		});
		Serialize<CircleRendererComponent>(out, entity, "CircleRendererComponent", [](YAML::Emitter& out, CircleRendererComponent& component)
		{
			out << YAML::Key << "Thickness" << YAML::Value << component.thickness
				<< YAML::Key << "Color" << YAML::Value << component.color;
		});
		Serialize<Model3DComponent>(out, entity, "Model3DComponent", [](YAML::Emitter& out, Model3DComponent& component)
		{
			out << YAML::Key << "Model" << YAML::Value << (component.model ? component.model->GetUUID() : UUID(0));
		});
		// TODO: how do this ???
		//Serialize<NativeScriptComponent>(out, entity, "NativeScriptComponent", [](YAML::Emitter& out, NativeScriptComponent& component) {});
		if (entity.HasRelationship())
		{
			Serialize<RelationshipComponent>(out, entity, "RelationshipComponent", [entity](YAML::Emitter& out, RelationshipComponent& component)
			{
				if (entity.HasParent())
					out << YAML::Key << "Parent" << YAML::Value << entity.GetParent().GetUUID();
				if (entity.HasChildren())
				{
					out << YAML::Key << "Children" << YAML::BeginSeq;
					for (Entity child = entity.GetFirstChild(); child; child = child.GetNextSibling())
						out << YAML::Value << child.GetUUID();
					out << YAML::EndSeq;
				}
			});
		}
		Serialize<SpriteRendererComponent>(out, entity, "SpriteRendererComponent", [](YAML::Emitter& out, SpriteRendererComponent& component)
		{
			out << YAML::Key << "TintColor" << YAML::Value << component.color
				<< YAML::Key << "TilingFactor" << YAML::Value << component.tilingFactor
				//<< YAML::Key << "Texture" << YAML::Value << (component.texture ? component.texture->GetUUID() : UUID(0));
				;
			if (component.texture)
				out << YAML::Key << "Texture" << YAML::Value << component.texture->GetFilepath().string();
		});

		Serialize<BoxCollider2DComponent>(out, entity, "BoxCollider2DComponent", [](YAML::Emitter& out, BoxCollider2DComponent& component)
		{
			out << YAML::Key << "Size" << YAML::Value << component.size
				<< YAML::Key << "Offset" << YAML::Value << component.offset
				<< YAML::Key << "Density" << YAML::Value << component.density
				<< YAML::Key << "Friction" << YAML::Value << component.friction
				<< YAML::Key << "Restitution" << YAML::Value << component.restitution
				<< YAML::Key << "RestitutionThreshold" << YAML::Value << component.restitutionThreshold;
		});
		Serialize<Rigidbody2DComponent>(out, entity, "Rigidbody2DComponent", [](YAML::Emitter& out, Rigidbody2DComponent& component)
		{
			out << YAML::Key << "Type" << YAML::Value << Rigidbody2DComponentBodyTypeToString(component.bodyType)
				<< YAML::Key << "FixedRotation" << YAML::Value << component.fixedRotation;
		});

		out << YAML::EndMap;

		// Serialize all child entities and their children
		for (Entity child = entity.GetFirstChild(); child; child = child.GetNextSibling())
			SerializeEntity(out, child);
	}

	bool SceneSerializer::Serialize(const std::filesystem::path& filepath)
	{
		std::ofstream file(filepath);
		if (!file.is_open())
			return false;

		YAML::Emitter out;
		out << YAML::BeginMap
			<< YAML::Key << "Scene" << YAML::Value << "Untitled" // TODO: scene names, projects, etc.
			<< YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;

		for (auto handle : scene->entities)
		{
			Entity entity{ handle, scene.get() };
			if (entity)
				SerializeEntity(out, entity);
		}

		out << YAML::EndSeq << YAML::EndMap;

		file << out.c_str();
		file.close();
		return !file.fail();
	}

	bool SceneSerializer::SerializeRuntime(const std::filesystem::path& filepath)
	{
		// TODO: implement
		return false;
	}
#pragma endregion

#pragma region deserialization
	bool SceneSerializer::Deserialize(const std::filesystem::path& filepath)
	{
		std::ifstream file(filepath);
		if (!file.is_open())
			return false;

		YAML::Node data = YAML::Load(file);
		file.close();

		YAML::Node sceneNode = data["Scene"];

		//std::string sceneName = sceneNode.as<std::string>(); // TODO: use this when projects are implemented

		std::unordered_map<UUID, entt::entity> entityIDMappings;

		if (auto entities = data["Entities"])
		{
			// Create all entities and setup mappings for RelationshipComponent
			for (auto entityNode : entities)
			{
				std::string name;
				if (auto tagComponent = entityNode["TagComponent"])
					name = tagComponent["Tag"].as<std::string>();

				UUID uuid = entityNode["Entity"].as<uint64_t>();
				entityIDMappings[uuid] = scene->CreateEntity(uuid, name);
			}

			auto& assetManager = Application::Get().GetAssetManager();
			for (auto entityNode : entities)
			{
				Entity entity{ entityIDMappings[entityNode["Entity"].as<uint64_t>()], scene.get() };

				// Entities always have transform components
				{
					auto transformComponentNode = entityNode["TransformComponent"];
					auto& transformComponent = entity.Get<TransformComponent>();
					transformComponent.translation = transformComponentNode["Translation"].as<glm::vec3>();
					transformComponent.rotation = transformComponentNode["Rotation"].as<glm::vec3>();
					transformComponent.scale = transformComponentNode["Scale"].as<glm::vec3>();
				}

				if (auto cameraComponentNode = entityNode["CameraComponent"])
				{
					auto& cameraComponent = entity.Add<CameraComponent>();

					auto cameraNode = cameraComponentNode["Camera"];
					auto& camera = cameraComponent.camera;
					camera.SetProjectionType(SceneCameraProjectionTypeFromString(cameraNode["ProjectionType"].as<std::string>()));
					camera.SetPerspectiveFOV(cameraNode["PerspectiveFOV"].as<float>());
					camera.SetPerspectiveNearClip(cameraNode["PerspectiveNear"].as<float>());
					camera.SetPerspectiveFarClip(cameraNode["PerspectiveFar"].as<float>());
					camera.SetOrthographicSize(cameraNode["OrthographicSize"].as<float>());
					camera.SetOrthographicNearClip(cameraNode["OrthographicNear"].as<float>());
					camera.SetOrthographicFarClip(cameraNode["OrthographicFar"].as<float>());

					cameraComponent.primary = cameraComponentNode["Primary"].as<bool>();
				}
				if (auto circleRendererComponentNode = entityNode["CircleRendererComponent"])
				{
					auto& circleRendererComponent = entity.Add<CircleRendererComponent>();
					circleRendererComponent.thickness = circleRendererComponentNode["Thickness"].as<float>();
					circleRendererComponent.color = circleRendererComponentNode["Color"].as<glm::vec4>();
				}
				if (auto model3DComponentNode = entityNode["Model3DComponent"])
				{
					auto& model3DComponent = entity.Add<Model3DComponent>();

					if (auto modelNode = model3DComponentNode["Model"])
						if (UUID modelUUID = modelNode.as<std::uint64_t>())
							model3DComponent.model = assetManager.GetOrLoad<Model3D>(modelUUID);
				}
				if (auto relationshipComponentNode = entityNode["RelationshipComponent"])
				{
					auto& relationshipComponent = entity.Get<RelationshipComponent>();

					if (auto parentNode = relationshipComponentNode["Parent"])
						relationshipComponent.parent = entityIDMappings[parentNode.as<uint64_t>()];

					if (auto childrenNode = relationshipComponentNode["Children"])
					{
						for (auto childNode : childrenNode)
						{
							Entity child{ entityIDMappings[childNode.as<uint64_t>()], scene.get() };
							child.SetParent(entity);
						}
					}
				}
				if (auto spriteRendererComponentNode = entityNode["SpriteRendererComponent"])
				{
					auto& spriteRendererComponent = entity.Add<SpriteRendererComponent>();

					spriteRendererComponent.color = spriteRendererComponentNode["TintColor"].as<glm::vec4>();
					spriteRendererComponent.tilingFactor = spriteRendererComponentNode["TilingFactor"].as<glm::vec2>();

					if (auto textureNode = spriteRendererComponentNode["Texture"])
					{
						//if (UUID textureUUID = textureNode.as<uint64_t>())
						//	spriteRendererComponent.texture = assetManager.GetOrLoad<Texture2D>(textureUUID);
						std::string filepath = textureNode.as<std::string>();
						spriteRendererComponent.texture = Texture2D::Create(LocalTexture2D::Create(filepath, 4));
					}
				}

				if (auto boxCollider2DComponentNode = entityNode["BoxCollider2DComponent"])
				{
					auto& boxCollider2DComponent = entity.Add<BoxCollider2DComponent>();
					boxCollider2DComponent.size = boxCollider2DComponentNode["Size"].as<glm::vec2>();
					boxCollider2DComponent.offset = boxCollider2DComponentNode["Offset"].as<glm::vec2>();
					boxCollider2DComponent.density = boxCollider2DComponentNode["Density"].as<float>();
					boxCollider2DComponent.friction = boxCollider2DComponentNode["Friction"].as<float>();
					boxCollider2DComponent.restitution = boxCollider2DComponentNode["Restitution"].as<float>();
					boxCollider2DComponent.restitutionThreshold = boxCollider2DComponentNode["RestitutionThreshold"].as<float>();
				}
				if (auto rigidbody2DComponentNode = entityNode["Rigidbody2DComponent"])
				{
					auto& rigidbody2DComponent = entity.Add<Rigidbody2DComponent>();
					rigidbody2DComponent.bodyType = Rigidbody2DComponentBodyTypeFromString(rigidbody2DComponentNode["Type"].as<std::string>());
					rigidbody2DComponent.fixedRotation = rigidbody2DComponentNode["FixedRotation"].as<bool>();
				}
			}
		}

		file.close();
		return !file.fail();
	}

	bool SceneSerializer::DeserializeRuntime(const std::filesystem::path& filepath)
	{
		// TODO: implement
		return false;
	}
#pragma endregion
}
