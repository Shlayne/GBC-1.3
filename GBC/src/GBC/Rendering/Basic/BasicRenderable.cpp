#include "gbcpch.h"
#include "BasicRenderable.h"

namespace gbc
{

	BasicRenderable::BasicRenderable(BasicRenderable&& renderable) noexcept
		: type(renderable.type), texture(std::move(renderable.texture)), framebuffer(std::move(renderable.framebuffer)), attachmentIndex(renderable.attachmentIndex)
	{
		renderable.Clear();
	}
	BasicRenderable::BasicRenderable(const BasicRenderable& renderable)
		: type(renderable.type), texture(renderable.texture), framebuffer(renderable.framebuffer), attachmentIndex(renderable.attachmentIndex) {}
	BasicRenderable& BasicRenderable::operator=(const BasicRenderable& renderable)
	{
		type = renderable.type;
		texture = renderable.texture;
		framebuffer = renderable.framebuffer;
		attachmentIndex = renderable.attachmentIndex;
		return *this;
	}

	BasicRenderable& BasicRenderable::operator=(BasicRenderable&& renderable) noexcept
	{
		type = renderable.type;
		texture = std::move(renderable.texture);
		framebuffer = std::move(renderable.framebuffer);
		attachmentIndex = renderable.attachmentIndex;
		renderable.Clear();
		return *this;
	}

	BasicRenderable::BasicRenderable(const Ref<Texture>& texture)
		: type(Type::Texture), texture(texture) {}
	BasicRenderable::BasicRenderable(const Ref<Framebuffer>& framebuffer, unsigned int attachmentIndex)
		: type(Type::Framebuffer), framebuffer(framebuffer), attachmentIndex(attachmentIndex) {}
	BasicRenderable::BasicRenderable(Ref<Texture>&& texture) noexcept
		: type(Type::Texture), texture(std::move(texture)) {}
	BasicRenderable::BasicRenderable(Ref<Framebuffer>&& framebuffer, unsigned int attachmentIndex) noexcept
		: type(Type::Framebuffer), framebuffer(std::move(framebuffer)), attachmentIndex(attachmentIndex) {}

	void BasicRenderable::Bind(unsigned int slot)
	{
		switch (type)
		{
			case Type::Texture:
				(*texture)->Bind(slot);
				return;
			case Type::Framebuffer:
				(*framebuffer)->BindColorTexture(attachmentIndex, slot);
				return;
		}

		GBC_CORE_ASSERT(false, "Unknown Renderable Type!");
	}

	void BasicRenderable::Unbind(unsigned int slot)
	{
		switch (type)
		{
			case Type::Texture:
				(*texture)->Unbind(slot);
				return;
			case Type::Framebuffer:
				(*framebuffer)->UnbindColorTexture(slot);
				return;
		}

		GBC_CORE_ASSERT(false, "Unknown Renderable Type!");
	}

	void BasicRenderable::Clear()
	{
		switch (type)
		{
			case Type::Texture:     texture = nullptr; break;
			case Type::Framebuffer: framebuffer = nullptr; break;
			case Type::None: return;
		}
		type = Type::None;
	}

	BasicRenderable::operator bool() const
	{
		switch (type)
		{
			case Type::None:        return false;
			case Type::Texture:     return texture->operator bool();
			case Type::Framebuffer: return framebuffer->operator bool();
		}

		GBC_CORE_ASSERT(false, "Unknown Renderable Type!");
		return false;
	}

	bool BasicRenderable::operator==(const BasicRenderable& renderable) const
	{
		if (type != renderable.type || type == Type::None)
			return false;

		switch (type)
		{
			case Type::Texture:     return *texture == renderable.texture;
			case Type::Framebuffer: return *framebuffer == renderable.framebuffer;
		}

		GBC_CORE_ASSERT(false, "Unknown Renderable Type!");
		return false;
	}
}
