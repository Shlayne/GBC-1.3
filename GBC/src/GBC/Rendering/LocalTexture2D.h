#pragma once

#include <string>

namespace gbc
{
	class LocalTexture2D
	{
	public:
		LocalTexture2D() = default;
		LocalTexture2D(const std::string& filepath, int requiredChannels = 0);
		LocalTexture2D(const std::string& filepath, bool flipVertically, int requiredChannels = 0);
		LocalTexture2D(int width, int height, int channels);
		LocalTexture2D(const LocalTexture2D& texture);
		~LocalTexture2D();

		bool ReadFile(const std::string& filePath, int requiredChannels = 0);
		bool ReadFile(const std::string& filePath, bool flipVertically, int requiredChannels = 0);
		bool WriteFile(const std::string& filePath);
		bool WriteFile(const std::string& filePath, bool flipVertically);
		void Create(int width, int height, int channels);
		bool Copy(const LocalTexture2D& texture);

		bool SetSubregion(const LocalTexture2D& texture, int positionX, int positionY);

		inline const std::string& GetFilepath() const noexcept { return filepath; }
		inline int GetWidth() const noexcept { return width; }
		inline int GetHeight() const noexcept { return height; }
		inline int GetChannels() const noexcept { return channels; }
		inline unsigned char* GetData() const noexcept { return data; }

		inline operator bool() const noexcept { return data != nullptr; }
		inline bool operator==(const LocalTexture2D& texture) const noexcept { return data == texture.data; }
	private:
		std::string filepath;

		int width = 0;
		int height = 0;
		int channels = 0;
		unsigned char* data = nullptr;
	};
}
