#pragma once

namespace gbc
{
	class LocalTexture2D
	{
	public:
		LocalTexture2D() = default;
		LocalTexture2D(const std::string& filePath, int requiredChannels = 0, bool flipVertically = false);
		LocalTexture2D(int width, int height, int channels);
		LocalTexture2D(const LocalTexture2D& texture);
		~LocalTexture2D();

		bool ReadFile(const std::string& filePath, int requiredChannels = 0, bool flipVertically = false);
		bool WriteFile(const std::string& filePath, bool flipVertically = false);
		void Create(int width, int height, int channels);
		bool Copy(const LocalTexture2D& texture);

		bool SetSubregion(const LocalTexture2D& texture, int positionX, int positionY);

		inline const std::string& GetFilePath() const noexcept { return filePath; }
		inline int GetWidth() const noexcept { return width; }
		inline int GetHeight() const noexcept { return height; }
		inline int GetChannels() const noexcept { return channels; }
		inline unsigned char* GetData() const noexcept { return data; }

		inline operator bool() const noexcept { return data != nullptr; }
		inline bool operator==(const LocalTexture2D& texture) const noexcept { return data == texture.data; }
	private:
		std::string filePath;

		int width = 0;
		int height = 0;
		int channels = 0;
		unsigned char* data = nullptr;
	};
}
