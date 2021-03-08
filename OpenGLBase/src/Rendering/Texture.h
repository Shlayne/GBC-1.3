#pragma once

#include <string>

class Texture
{
public:
	Texture() = default;
	Texture(const std::string& filePath, int requiredChannels = 0, bool flipVertically = false);
	Texture(int width, int height, int channels);
	Texture(const Texture& texture);
	~Texture();

	bool ReadFile(const std::string& filePath, int requiredChannels = 0, bool flipVertically = false);
	bool WriteFile(const std::string& filePath, bool flipVertically = false);
	void Create(int width, int height, int channels);
	bool Copy(const Texture& texture);

	bool SetSubregion(const Texture& texture, int positionX, int positionY);

	Texture* CreateMipmap();

	inline const std::string& GetFilePath() const { return filePath; }
	inline int GetWidth() const { return width; }
	inline int GetHeight() const { return height; }
	inline int GetChannels() const { return channels; }
	inline unsigned char* GetData() const { return data; }

	inline operator bool() const { return data != nullptr; }
	inline bool operator==(const Texture& texture) const { return data == texture.data; }
private:
	std::string filePath;

	int width = 0;
	int height = 0;
	int channels = 0;
	unsigned char* data = nullptr;
};
