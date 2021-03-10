#include "gbcpch.h"
#include "TextureAtlas.h"
#include "GBC/Rendering/Renderer.h"

namespace gbc
{
	TextureAtlas::TextureAtlas()
		: maxTextureSize(Renderer::GetMaxTextureSize()), root(new TextureNode(glm::ivec2(), maxTextureSize))
	{

	}

	glm::ivec2 TextureAtlas::AddTexture(Ref<LocalTexture2D> texture)
	{
		glm::ivec2 textureSize = glm::ivec2(texture->GetWidth(), texture->GetHeight());

		while ((textureSize.x < atlasTextureSize.x || textureSize.y < atlasTextureSize.y) ||
			(atlasTextureSize.x < maxTextureSize.x || atlasTextureSize.y < maxTextureSize.y))
		{
			TextureNode* node = AddTexture(root.get(), texture, textureSize);
			if (node != nullptr)
				return node->position;
			else
				atlasTextureSize *= 2; // Keep the texture size a power of 2
		}

		// If this happens, the texture can not fit on the GPU, this is very bad
		return glm::ivec2(-1);
	}

	// From: https://straypixels.net/texture-packing-for-fonts/
	TextureAtlas::TextureNode* TextureAtlas::AddTexture(TextureNode* node, Ref<LocalTexture2D> texture, const glm::ivec2& size)
	{
		// If the node has a texture, it can only be a leaf node, so don't bother
		if (node->texture != nullptr)
		{
			return nullptr;
		}
		// If the node has both children, search them for an opening
		else if (node->left != nullptr && node->right != nullptr)
		{
			TextureNode* newLeftNode = AddTexture(node->left.get(), texture, size);
			return newLeftNode != nullptr ? newLeftNode : AddTexture(node->right.get(), texture, size);
		}
		// If an opening has been found
		else
		{
			// Get the real size of the opening
			glm::ivec2 realSize(node->size);
			if (node->position.x + node->size.x == maxTextureSize.x)
				realSize.x = atlasTextureSize.x - node->position.x;
			if (node->position.y + node->size.y == maxTextureSize.y)
				realSize.y = atlasTextureSize.y - node->position.y;

			// If the size of the opening (which may technically be the max texture size) is the same size as the texture
			if (node->size == size)
			{
				node->texture = texture;
				return node;
			}
			// This opening is not big enough
			else if (realSize.x < size.x || realSize.y < size.y)
			{
				return nullptr;
			}
			// This opening is bigger than the texture
			else
			{
				glm::ivec2 remainingSize = realSize - size;
				bool splitVertically = remainingSize.x < remainingSize.y;

				// If the opening is the same size as the texture
				if (remainingSize.x == 0 && remainingSize.y == 0)
					splitVertically = node->size.x <= node->size.y;

				// Get the right position and size of the opening and empty space
				if (splitVertically)
				{
					node->left.reset(new TextureNode(node->position, {node->size.x, size.y}));
					node->right.reset(new TextureNode({node->position.x, node->position.y + size.y}, {node->size.x, node->size.y - size.y}));
				}
				else
				{
					node->left.reset(new TextureNode(node->position, {size.x, node->size.y}));
					node->right.reset(new TextureNode({node->position.x + size.x, node->position.y}, {node->size.x - size.x, node->size.y}));
				}

				return AddTexture(node->left.get(), texture, size);
			}
		}
	}

	LocalTexture2D** TextureAtlas::Create(int mipmapLevels)
	{
		LocalTexture2D** atlasTextures = nullptr;
		if (mipmapLevels > 0)
		{
			atlasTextures = new LocalTexture2D *[mipmapLevels];
			for (int i = 0; i < mipmapLevels; i++)
				atlasTextures[i] = new LocalTexture2D(atlasTextureSize.x >> i, atlasTextureSize.y >> i, 4);
			PutTexture(root.get(), atlasTextures, mipmapLevels);
		}
		return atlasTextures;
	}

	void TextureAtlas::PutTexture(TextureNode* node, LocalTexture2D** atlasTextures, int mipmapLevels)
	{
		if (node != nullptr)
		{
			if (node->texture != nullptr)
			{
				// Yes, copying the original texture is a waste of memory,
				// but it makes coding it easier.
				// TODO: fix this
				LocalTexture2D* texture = new LocalTexture2D(*(node->texture));

				for (int i = 0; i < mipmapLevels && texture != nullptr && texture; i++)
				{
					// This is... finally, where textures are copied
					atlasTextures[i]->SetSubregion(*texture, node->position.x >> i, node->position.y >> i);
					LocalTexture2D* newTexture = texture->CreateMipmap();
					delete texture;
					texture = newTexture;
				}

				delete texture;
			}
			// Since a node with a texture has no children, don't try to copy their
			// non-existent textures, because they themselves don't exist.
			else
			{
				PutTexture(node->left.get(), atlasTextures, mipmapLevels);
				PutTexture(node->right.get(), atlasTextures, mipmapLevels);
			}
		}
	}
}
