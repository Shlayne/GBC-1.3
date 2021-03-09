#pragma once

#include "Rendering/Texture.h"

class OpenGLTexture : public Texture
{
public:
	OpenGLTexture(Ref<LocalTexture2D> texture);
	virtual ~OpenGLTexture();

	virtual void Bind(unsigned int slot = 0) const override;
	virtual void Unbind(unsigned int slot = 0) const override;

	virtual const Ref<LocalTexture2D>& GetTexture() const override { return texture; }

	// Call this when you want to update the internal
	// texture after the the local texture has updated.
	virtual void Update() override;
private:
	Ref<LocalTexture2D> texture;

	unsigned int rendererID = 0;
	int internalFormat = 0;
	int format = 0;
};
