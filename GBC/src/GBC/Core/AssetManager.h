#pragma once

#include "GBC/Core/UUID.h"
#include "GBC/Rendering/Texture.h"
#include <filesystem>
#include <map>
#include <unordered_map>

namespace gbc
{
	class AssetManager
	{
	public:
		Ref<Texture2D> GetOrLoadTexture(const std::filesystem::path& filepath);
		Ref<Texture2D> GetExistingTexture(UUID uuid);
		bool UpdateSpecifications(UUID uuid, const TextureSpecification& specs);
	private:
		bool CreateDefaultTextureMeta(const std::filesystem::path& metaFilepath);
		bool WriteTextureMeta(const Ref<Texture2D>& texture, const std::filesystem::path& metaFilepath);
		Ref<Texture2D> ReadTexture(const std::filesystem::path& filepath, const std::filesystem::path& metaFilepath);
	private:
		// Has to be a map instead of an unordered_map because std::filesystem::path doesn't
		// have a hash<std::filesystem::path> associated with it and im to lazy to write it.
		// Plus, this will most likely be completely rewritten later as this is my frist ever
		// attempt at making an asset manager.
		std::map<std::filesystem::path, UUID> textureFilepaths;
		std::unordered_map<UUID, std::pair<Ref<Texture2D>, bool>> textures;
	private:
		friend class Application;
		AssetManager() = default;

		AssetManager(const AssetManager&) = delete;
		AssetManager(AssetManager&&) = delete;
		AssetManager& operator=(const AssetManager&) = delete;
		AssetManager& operator=(AssetManager&&) = delete;
	public:
		~AssetManager();
	};
}
