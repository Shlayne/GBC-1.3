#pragma once

#include <optional>
#include "GBC/Rendering/Texture.h"
#include "GBC/Rendering/Framebuffer.h"
#include "GBC/Core/Core.h"

namespace gbc
{
	// Lets BasicRenderer use textures and framebuffers interchangably instead of just textures
	// Can be expanded by simply adding more std::optional<T> and appropriate constructors,
	// but remember to update Bind and Unbind accordingly.
	class BasicRenderable
	{
	public:
		enum class Type { None = 0, Texture, Framebuffer };

		BasicRenderable() = default;
		BasicRenderable(const Ref<Texture>& texture);
		BasicRenderable(const Ref<Framebuffer>& framebuffer, unsigned int attachmentIndex);

		void Bind(unsigned int slot);
		void Unbind(unsigned int slot);

		void Clear();

		operator bool() const;
		bool operator==(const BasicRenderable& renderable) const;
	private:
		Type type = Type::None;

		std::optional<Ref<Texture>> texture;

		std::optional<Ref<Framebuffer>> framebuffer;
		unsigned int attachmentIndex = 0;
	};
}
