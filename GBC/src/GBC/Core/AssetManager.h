#pragma once

#include "GBC/Core/Asset.h"
#include "GBC/Rendering/Texture.h"
#include "GBC/Model/Model3D.h"
#include <filesystem>
#if GBC_CONFIG_DIST
	#include <unordered_map>
#else
	#include <map>
#endif

namespace gbc
{
	class AssetManager
	{
	public:
		template<typename T> Ref<T> GetOrLoad(UUID uuid);
		template<typename T> Ref<T> GetOrLoad(const std::filesystem::path& filepath);
	private:
		void* GetOrLoadImpl(UUID uuid);
		void* GetOrLoadImpl(const std::filesystem::path& filepath);

		//bool CreateDefaultTextureMeta(const std::filesystem::path& metaFilepath);
		//bool WriteTextureMeta(const Ref<Texture2D>& texture, const std::filesystem::path& metaFilepath);
		//Ref<Texture2D> ReadTexture(const std::filesystem::path& filepath, const std::filesystem::path& metaFilepath);

		//bool CreateDefault3DModelMeta(const std::filesystem::path& metaFilepath);
		//bool Write3DModelMeta(const Ref<Model3D>& model3D, const std::filesystem::path& metaFilepath);
		//Ref<Model3D> Read3DModel(const std::filesystem::path& filepath, const std::filesystem::path& metaFilepath);
	private:
		friend class Application;
		friend class AssetManagerPanel;

#if GBC_CONFIG_DIST
		std::unordered_map<UUID, Asset> assets;
#else
		// Only for AssetManagerPanel to show the assets in a particular order, but does not matter for runtime
		std::map<UUID, Ref<Asset>> assets;
#endif
	private:
		AssetManager();
		~AssetManager();

		AssetManager(const AssetManager&) = delete;
		AssetManager(AssetManager&&) = delete;
		AssetManager& operator=(const AssetManager&) = delete;
		AssetManager& operator=(AssetManager&&) = delete;
	};
}
