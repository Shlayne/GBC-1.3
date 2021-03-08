#pragma once

#include "Texture.h"

class OpenGLTexture
{
public:
	OpenGLTexture(const Texture* texture);
	~OpenGLTexture();

	void Bind(unsigned int slot = 0) const;
	void Unbind(unsigned int slot = 0) const;

	inline const Texture* GetTexture() const { return texture; }

	// Call this when you want to update the internal
	// texture after the the local texture has updated.
	void Update();
private:
	// TODO: when a reference counting system is in place, change this to that
	const Texture* texture;

	unsigned int rendererID = 0;
	int internalFormat = 0; // how opengl will store the texture data
	int format = 0; // how the local copy of the texture data is stored
};
