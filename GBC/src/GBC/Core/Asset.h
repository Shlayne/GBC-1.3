#pragma once

#include "GBC/Core/UUID.h"
#include <filesystem>

namespace gbc
{
	class Asset
	{
	public:
		Asset() noexcept = default;
		virtual ~Asset() noexcept = default;
	public:
		virtual UUID GetUUID() const = 0;
		virtual const std::filesystem::path& GetFilepath() const = 0;
	private:
		friend class AssetManager;
		friend class AssetManagerPanel;

		enum class Type
		{
			None = 0,
			Texture2D,
			Mesh3D,

			_count_internal,
			Count = _count_internal - 1
		};

		Type type = Type::None;
	};
}
