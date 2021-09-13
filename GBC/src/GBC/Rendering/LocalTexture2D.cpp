#include "gbcpch.h"
#include "LocalTexture2D.h"
#include <stb/stb_image.h>
#include <stb/stb_image_write.h>
#include "GBC/Rendering/RendererAPI.h"

namespace gbc
{
	static bool GetDefaultVerticalFlip()
	{
		return RendererAPI::GetAPI() == RendererAPI::API::OpenGL;
	}

	LocalTexture2D::LocalTexture2D(const std::string& filepath, int32_t requiredChannels)
	{
		ReadFile(filepath, requiredChannels);
	}

	LocalTexture2D::LocalTexture2D(const std::string& filepath, bool flipVertically, int32_t requiredChannels)
	{
		ReadFile(filepath, flipVertically, requiredChannels);
	}

	LocalTexture2D::LocalTexture2D(int32_t width, int32_t height, int32_t channels)
	{
		Create(width, height, channels);
	}

	LocalTexture2D::LocalTexture2D(const LocalTexture2D& texture)
	{
		Copy(texture);
	}

	LocalTexture2D::LocalTexture2D(LocalTexture2D&& texture) noexcept
		: filepath(std::move(texture.filepath)), width(width), height(height), channels(channels), data(data)
	{
		width = 0;
		height = 0;
		channels = 0;
		texture.data = nullptr;
	}

	LocalTexture2D& LocalTexture2D::operator=(const LocalTexture2D& texture)
	{
		if (this != &texture)
			Copy(texture);
		return *this;
	}

	LocalTexture2D& LocalTexture2D::operator=(LocalTexture2D&& texture) noexcept
	{
		if (this != &texture)
		{
			filepath = std::move(texture.filepath);
			width = texture.width;
			height = texture.height;
			channels = texture.channels;
			data = texture.data;

			width = 0;
			height = 0;
			channels = 0;
			texture.data = nullptr;
		}
		return *this;
	}

	LocalTexture2D::~LocalTexture2D()
	{
		delete[] data;
	}

	bool LocalTexture2D::ReadFile(const std::string& filepath, int32_t requiredChannels)
	{
		return ReadFile(filepath, GetDefaultVerticalFlip(), requiredChannels);
	}

	bool LocalTexture2D::ReadFile(const std::string& filepath, bool flipVertically, int32_t requiredChannels)
	{
		GBC_PROFILE_FUNCTION();

		stbi_set_flip_vertically_on_load(flipVertically);
		uint8_t* stbiData = stbi_load(filepath.c_str(), &width, &height, &channels, requiredChannels);
		if (stbiData != nullptr)
		{
			if (data != nullptr)
				delete[] data;
			data = stbiData;

			if (requiredChannels != 0)
				channels = requiredChannels;
			this->filepath = filepath;
			return true;
		}
		return false;
	}

	bool LocalTexture2D::WriteFile(const std::string& filepath)
	{
		return WriteFile(filepath, GetDefaultVerticalFlip());
	}

	bool LocalTexture2D::WriteFile(const std::string& filepath, bool flipVertically)
	{
		GBC_PROFILE_FUNCTION();

		stbi_flip_vertically_on_write(flipVertically);
		// TODO: let the user decide the output file format
		return data != nullptr && stbi_write_png(filepath.c_str(), width, height, channels, data, 0) != 0;
	}

	void LocalTexture2D::Create(int32_t width, int32_t height, int32_t channels)
	{
		if (data != nullptr)
		{
			delete[] data;
			filepath.clear();
		}

		size_t size = (size_t)width * height * channels;
		data = new uint8_t[size]{ 0 };
		this->width = width;
		this->height = height;
		this->channels = channels;
	}

	bool LocalTexture2D::Copy(const LocalTexture2D& texture)
	{
		if (texture)
		{
			if (data != nullptr)
				delete[] data;

			width = texture.width;
			height = texture.height;
			channels = texture.channels;
			filepath = texture.filepath;

			size_t size = (size_t)width * height * channels;
			data = new unsigned char[size];
			memcpy_s(data, size, texture.data, size);
			return true;
		}
		return false;
	}

	bool LocalTexture2D::SetSubregion(const LocalTexture2D& texture, int32_t positionX, int32_t positionY)
	{
		GBC_PROFILE_FUNCTION();

		if (channels == texture.channels && positionX + texture.width <= width && positionX >= 0 && positionY + texture.height <= height && positionY >= 0)
		{
			size_t textureIncrement = static_cast<size_t>(texture.width) * channels;
			size_t increment = static_cast<size_t>(width) * channels;

			uint8_t* source = texture.data;
			uint8_t* destination = data + ((static_cast<size_t>(positionY) * width + positionX) * channels);
			for (int32_t y = 0; y < texture.height; y++, source += textureIncrement, destination += increment)
				memcpy_s(destination, textureIncrement, source, textureIncrement);
			return true;
		}
		return false;
	}
}
