#include "gbcpch.h"
#include "AssetManager.h"
#include "GBC/Core/Core.h"
#include "GBC/Core/FileTypes.h"
#include "GBC/IO/CommonYAML.h"
#include <yaml-cpp/yaml.h>

namespace gbc
{
	// TODO: same as Assets directory
	static const std::filesystem::path& projectFilepath = L"Projects/ProjectTEMP.gproj";

	AssetManager::AssetManager()
	{
		// This is simply DeserializeProject
		std::ifstream file(projectFilepath);
		if (!file.is_open())
			return;
		YAML::Node data = YAML::Load(file);
		file.close();

		YAML::Node assetsNode = data["Assets"];
		for (auto asset : assetsNode)
		{
			UUID uuid = asset.begin()->as<uint64_t>();
			std::filesystem::path filepath = (++asset.begin())->as<std::string>();

			//if ()
			//assets
		}
	}

	AssetManager::~AssetManager()
	{
		// This is simply SerializeProject
		std::ofstream file(projectFilepath);
		if (!file.is_open())
			return;

		YAML::Emitter out;
		out << YAML::BeginMap << "Assets" << YAML::BeginSeq;
		for (auto& [id, asset] : assets)
			out << YAML::BeginSeq << id << asset->GetFilepath().string() << YAML::EndSeq;
		out << YAML::EndSeq << YAML::EndMap;

		file << out.c_str();
		file.close();
	}

	template<typename T> Ref<T> GetOrLoad(UUID uuid) { /*static_assert(false);*/ return nullptr; }
	template<typename T> Ref<T> GetOrLoad(const std::filesystem::path& filepath) { /*static_assert(false);*/ return nullptr; }

	template<> Ref<Texture2D> AssetManager::GetOrLoad(UUID uuid) { return Ref<Texture2D>(static_cast<Texture2D*>(GetOrLoadImpl(uuid))); }
	template<> Ref<Model3D> AssetManager::GetOrLoad(UUID uuid) { return Ref<Model3D>(static_cast<Model3D*>(GetOrLoadImpl(uuid))); }

	template<> Ref<Texture2D> AssetManager::GetOrLoad(const std::filesystem::path& filepath) { return Ref<Texture2D>(static_cast<Texture2D*>(GetOrLoadImpl(filepath))); }
	template<> Ref<Model3D> AssetManager::GetOrLoad(const std::filesystem::path& filepath) { return Ref<Model3D>(static_cast<Model3D*>(GetOrLoadImpl(filepath))); }

	void* AssetManager::GetOrLoadImpl(UUID uuid)
	{
		auto it = assets.find(uuid);
		return it != assets.end() ? it->second.get() : nullptr;
	}

	void* AssetManager::GetOrLoadImpl(const std::filesystem::path& filepath)
	{
		// this really needs projects to be implemented first, but since I haven't
		// done that yet, I'm going to temporarily store this in the scene file and
		// clear the asset manager upon loading a scene



		return nullptr;
	}

	/*
	bool AssetManager::CreateDefaultTextureMeta(const std::filesystem::path& metaFilepath)
	{
		std::ofstream file(metaFilepath);
		if (!file.is_open())
			return false;

		YAML::Emitter out;

		out << YAML::BeginMap << "UUID" << YAML::Value << UUID()
			<< YAML::Key << "MinFilter" << YAML::Value << TextureFilterModeToString(TextureFilterMode::Linear)
			<< YAML::Key << "MagFilter" << YAML::Value << TextureFilterModeToString(TextureFilterMode::Linear)
			<< YAML::Key << "WrapS" << YAML::Value << TextureWrapModeToString(TextureWrapMode::ClampToEdge)
			<< YAML::Key << "WrapT" << YAML::Value << TextureWrapModeToString(TextureWrapMode::ClampToEdge)
			<< YAML::EndMap;

		file << out.c_str();
		file.close();
		return true;
	}
	*/
	/*
	bool AssetManager::WriteTextureMeta(const Ref<Texture2D>& texture, const std::filesystem::path& metaFilepath)
	{
		std::ofstream file(metaFilepath);
		if (!file.is_open())
			return false;

		const auto& specs = texture->GetSpecification();

		YAML::Emitter out;

		out << YAML::BeginMap << "UUID" << YAML::Value << texture->GetUUID()
			<< YAML::Key << "MinFilter" << YAML::Value << TextureFilterModeToString(specs.minFilter)
			<< YAML::Key << "MagFilter" << YAML::Value << TextureFilterModeToString(specs.magFilter)
			<< YAML::Key << "WrapS" << YAML::Value << TextureWrapModeToString(specs.wrapS)
			<< YAML::Key << "WrapT" << YAML::Value << TextureWrapModeToString(specs.wrapT)
			<< YAML::EndMap;

		file << out.c_str();
		file.close();
		return true;
	}
	*/
	/*
	Ref<Texture2D> AssetManager::ReadTexture(const std::filesystem::path& filepath, const std::filesystem::path& metaFilepath)
	{
		std::ifstream file(metaFilepath);
		if (file.is_open())
		{
			YAML::Node metadataNode = YAML::Load(file);
			file.close();

			if (metadataNode)
			{
				TextureSpecification specs;

				uint64_t uuid = metadataNode["UUID"].as<uint64_t>();
				specs.minFilter = TextureFilterModeFromString(metadataNode["MinFilter"].as<std::string>());
				specs.magFilter = TextureFilterModeFromString(metadataNode["MagFilter"].as<std::string>());
				specs.wrapS = TextureWrapModeFromString(metadataNode["WrapS"].as<std::string>());
				specs.wrapT = TextureWrapModeFromString(metadataNode["WrapT"].as<std::string>());
				specs.texture = LocalTexture2D::Create(filepath);

				return Texture2D::Create(specs, uuid);
			}
		}

		return nullptr;
	}
	*/
	/*
	bool AssetManager::CreateDefault3DModelMeta(const std::filesystem::path& metaFilepath)
	{
		std::ofstream file(metaFilepath);
		if (!file.is_open())
			return false;

		YAML::Emitter out;

		out << YAML::BeginMap << "UUID" << YAML::Value << UUID()
			<< YAML::EndMap;

		file << out.c_str();
		file.close();
		return true;
	}
	*/
	/*
	bool AssetManager::Write3DModelMeta(const Ref<Model3D>& model3D, const std::filesystem::path& metaFilepath)
	{
		std::ofstream file(metaFilepath);
		if (!file.is_open())
			return false;

		YAML::Emitter out;

		out << YAML::BeginMap << "UUID" << YAML::Value << model3D->GetUUID()
			<< YAML::EndMap;

		file << out.c_str();
		file.close();
		return true;
	}
	*/
	/*
	Ref<Model3D> AssetManager::Read3DModel(const std::filesystem::path& filepath, const std::filesystem::path& metaFilepath)
	{
		std::ifstream file(metaFilepath);
		if (file.is_open())
		{
			YAML::Node metadataNode = YAML::Load(file);
			file.close();

			if (metadataNode)
			{
				uint64_t uuid = metadataNode["UUID"].as<uint64_t>();
				return Model3D::Create(filepath, uuid);
			}
		}

		return nullptr;
	}
	*/
}
