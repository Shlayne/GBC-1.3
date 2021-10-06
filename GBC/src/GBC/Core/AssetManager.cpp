#include "gbcpch.h"
#include "AssetManager.h"
#include "GBC/Core/FileTypes.h"
#include "GBC/IO/CommonYAML.h"
#include <yaml-cpp/yaml.h>

namespace gbc
{
	AssetManager::~AssetManager()
	{
		for (auto& [uuid, texture] : textures)
			if (texture.second)
				WriteTextureMeta(texture.first, AppendMetadataType(texture.first->GetTexture()->GetFilepath()));
	}

	Ref<Texture2D> AssetManager::GetOrLoadTexture(const std::filesystem::path& filepath)
	{
		if (auto it = textureFilepaths.find(filepath); it != textureFilepaths.end())
			return textures.at(it->second).first;

		if (!std::filesystem::exists(filepath))
			return nullptr;

		std::filesystem::path metaFilepath = AppendMetadataType(filepath);
		if (!std::filesystem::exists(metaFilepath))
			CreateDefaultTextureMeta(metaFilepath);

		Ref<Texture2D> texture = ReadTexture(filepath, metaFilepath);
		if (texture)
		{
			UUID uuid = texture->GetUUID();
			textureFilepaths[filepath] = uuid;
			textures[uuid] = { texture, false };
		}
		return texture;
	}

	Ref<Texture2D> AssetManager::GetExistingTexture(UUID uuid)
	{
		auto it = textures.find(uuid);
		return it != textures.end() ? it->second.first : nullptr;
	}

	bool AssetManager::UpdateSpecifications(UUID uuid, const TextureSpecification& specs)
	{
		if (auto it = textures.find(uuid); it != textures.end())
		{
			it->second.second = true;
			it->second.first->ChangeSampling(specs);
			return true;
		}

		return false;
	}

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
}
