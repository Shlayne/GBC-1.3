#pragma once

#include "Rendering/LocalTexture2D.h"
#include "Core/RefScope.h"

class Texture
{
public:
	virtual ~Texture() = default;

	virtual void Bind(unsigned int slot) const = 0;
	virtual void Unbind(unsigned int slot) const = 0;

	virtual const Ref<LocalTexture2D>& GetTexture() const = 0;

	// Call this when you want to update the internal
	// texture after the the local texture has updated.
	virtual void Update() = 0;

	static Ref<Texture> Create(Ref<LocalTexture2D> texture);
};
