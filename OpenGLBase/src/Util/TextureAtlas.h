#pragma once

#include <glm/glm.hpp>
#include "Core/Core.h"
#include "Rendering/LocalTexture2D.h"

namespace cbc
{
	class TextureAtlas
	{
	public:
		TextureAtlas();

		// Returns the texture coordinates, in pixel space, of the texture atlas.
		// These coordinates are for the first texture in the texture array returned
		// by create. For successive textures in that array, their subtexture coordinates
		// are just this divided by a power of two each time.
		// After you have added all of your textures, before you even call create(),
		// you can get normalized texture coordinates by dividing this return value
		// by getTextureSize(). Don't forget to cast to a float or double!
		glm::ivec2 AddTexture(Ref<LocalTexture2D> texture);

		// Returns mipmapLevels number of textures in an array. Each texture has
		// the smallest possible size that contains every texture provided through
		// addTexture. The returned textures will have an aspect ratio of 1, and
		// their size will be constrained to powers of 2, e.g.: 256x256, 512x512...
		LocalTexture2D** Create(int mipmapLevels = 1);

		inline const glm::ivec2& GetTextureSize() const { return atlasTextureSize; }
	private:
		struct TextureNode
		{
			TextureNode(const glm::ivec2& position, const glm::ivec2& size)
				: position(position), size(size) {}

			Ref<LocalTexture2D> texture = nullptr;
			bool childrenFull = false;

			// These are unnormalized texture coordinates and sizes
			glm::ivec2 position;
			glm::ivec2 size;

			// Lazy memory management
			Scope<TextureNode> left;
			Scope<TextureNode> right;
		};

		TextureNode* AddTexture(TextureNode* node, Ref<LocalTexture2D> texture, const glm::ivec2& size);
		void PutTexture(TextureNode* node, LocalTexture2D** atlasTextures, int mipmapLevels);

		glm::ivec2 maxTextureSize;
		Scope<TextureNode> root;
		glm::ivec2 atlasTextureSize{256, 256};
	};
}
