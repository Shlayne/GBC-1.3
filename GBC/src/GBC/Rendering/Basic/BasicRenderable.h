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
		BasicRenderable(const BasicRenderable& renderable);
		BasicRenderable(BasicRenderable&& renderable) noexcept;
		BasicRenderable& operator=(const BasicRenderable& renderable);
		BasicRenderable& operator=(BasicRenderable&& renderable) noexcept;

		BasicRenderable(const Ref<Texture>& texture);
		BasicRenderable(const Ref<Framebuffer>& framebuffer, unsigned int attachmentIndex);
		BasicRenderable(Ref<Texture>&& texture) noexcept;
		BasicRenderable(Ref<Framebuffer>&& framebuffer, unsigned int attachmentIndex) noexcept;
		//BasicRenderable& operator=(const Ref<Texture>& texture);
		//BasicRenderable& operator=(const Ref<Framebuffer>& framebuffer, unsigned int attachmentIndex);
		//BasicRenderable& operator=(Ref<Texture>&& texture) noexcept;
		//BasicRenderable& operator=(Ref<Framebuffer>&& framebuffer, unsigned int attachmentIndex) noexcept;

		void Bind(unsigned int slot);
		void Unbind(unsigned int slot);

		void Clear();
		inline Type GetType() const { return type; }
		inline void SetType(Type type) { Clear(); this->type = type; }

		operator bool() const;
		bool operator==(const BasicRenderable& renderable) const;
	private:
		Type type = Type::None;

		std::optional<Ref<Texture>> texture = nullptr;

		std::optional<Ref<Framebuffer>> framebuffer = nullptr;
		unsigned int attachmentIndex = 0;
	};
}
