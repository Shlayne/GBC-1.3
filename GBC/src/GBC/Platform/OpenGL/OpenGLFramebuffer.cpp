#include "gbcpch.h"
#include "OpenGLFramebuffer.h"
#include "gl/glew.h"
#include "GBC/Rendering/Renderer.h"

namespace gbc
{
	static GLenum GetOpenGLFilterMode(FramebufferFilterMode filter)
	{
		switch (filter)
		{
			case FramebufferFilterMode::Linear:  return GL_LINEAR;
			case FramebufferFilterMode::Nearest: return GL_NEAREST;
		}

		GBC_ASSERT(false, "Unknown Framebuffer Filter Mode!");
		return 0;
	}

	static GLenum GetOpenGLWrapMode(FramebufferWrapMode wrap)
	{
		switch (wrap)
		{
			case FramebufferWrapMode::ClampToEdge: return GL_CLAMP_TO_EDGE;
			case FramebufferWrapMode::Repeat:      return GL_REPEAT;
		}

		GBC_ASSERT(false, "Unknown Framebuffer Wrap Mode!");
		return 0;
	}

	void GetOpenGLTypes(FramebufferTextureFormat inFormat, GLenum& internalFormat, GLenum& format, GLenum& type)
	{
		switch (inFormat)
		{
			case FramebufferTextureFormat::RGBA8:
				internalFormat = GL_RGBA8;
				format = GL_RGBA;
				type = GL_UNSIGNED_BYTE;
				return;
			case FramebufferTextureFormat::RedInteger:
				internalFormat = GL_R32I;
				format = GL_RED_INTEGER;
				type = GL_INT;
				return;
			case FramebufferTextureFormat::Depth24Stencil8:
				internalFormat = GL_DEPTH24_STENCIL8;
				format = GL_DEPTH24_STENCIL8;
				type = GL_DEPTH_STENCIL_ATTACHMENT;
				return;
		}

		GBC_ASSERT(false, "Unknown Framebuffer Texture Format!");
	}

	static GLenum GetTextureTarget(bool multisampled)
	{
		return multisampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
	}

	static void CreateTextures(GLenum textureTarget, unsigned int* id, unsigned int count)
	{
		glCreateTextures(textureTarget, count, id);
	}

	static void BindTextures(GLenum textureTarget, unsigned int id)
	{
		glBindTexture(textureTarget, id);
	}

	static void AttachColorTexture(unsigned int id, int samples, int width, int height, FramebufferTextureSpecification textureTpecification, int index)
	{
		GLenum internalFormat, format, type;
		GetOpenGLTypes(textureTpecification.format, internalFormat, format, type);

		bool multisampled = samples > 1;
		if (multisampled)
		{
			glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, internalFormat, width, height, GL_FALSE);
		}
		else
		{
			glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, type, nullptr);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GetOpenGLFilterMode(textureTpecification.minFilter));
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GetOpenGLFilterMode(textureTpecification.magFilter));
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GetOpenGLWrapMode(textureTpecification.wrapS));
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GetOpenGLWrapMode(textureTpecification.wrapT));
		}

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, GetTextureTarget(multisampled), id, 0);
	}

	static void AttachDepthTexture(unsigned int id, int samples, int width, int height, FramebufferTextureSpecification textureTpecification)
	{
		GLenum internalFormat, format, type;
		GetOpenGLTypes(textureTpecification.format, internalFormat, format, type);

		bool multisampled = samples > 1;
		if (multisampled)
		{
			glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, format, width, height, GL_FALSE);
		}
		else
		{
			glTexStorage2D(GL_TEXTURE_2D, 1, format, width, height);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GetOpenGLFilterMode(textureTpecification.minFilter));
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GetOpenGLFilterMode(textureTpecification.magFilter));
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GetOpenGLWrapMode(textureTpecification.wrapS));
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GetOpenGLWrapMode(textureTpecification.wrapT));
		}

		glFramebufferTexture2D(GL_FRAMEBUFFER, type, GetTextureTarget(multisampled), id, 0);
	}

	OpenGLFramebuffer::OpenGLFramebuffer(FramebufferSpecification specification)
		: specification(specification)
	{
		for (const auto& specification : this->specification.attachments.attachments)
		{
			if (IsDepthFormat(specification.format))
				depthAttachmentSpecification = specification;
			else
				colorAttachmentSpecifications.push_back(specification);
		}

		Recreate();
	}

	OpenGLFramebuffer::~OpenGLFramebuffer()
	{
		Clear();
	}

	bool OpenGLFramebuffer::GetColorPixel(void* pixel, int x, int y, unsigned int index) const
	{
		if (index < colorAttachmentSpecifications.size() &&
			x >= 0 && x < specification.width && y >= 0 && y < specification.height)
		{
			glReadBuffer(GL_COLOR_ATTACHMENT0 + index);
			GLenum internalFormat, format, type;
			GetOpenGLTypes(colorAttachmentSpecifications[index].format, internalFormat, format, type);
			glReadPixels(x, y, 1, 1, format, type, pixel);
			return true;
		}
		return false;
	}

	void OpenGLFramebuffer::ClearColorAttachment(const void* value, unsigned int index)
	{
		GLenum internalFormat, format, type;
		GetOpenGLTypes(colorAttachmentSpecifications[index].format, internalFormat, format, type);
		glClearTexImage(colorAttachments[index], 0, format, type, value);
	}

	void OpenGLFramebuffer::Recreate()
	{
		Clear();

		glCreateFramebuffers(1, &rendererID);
		glBindFramebuffer(GL_FRAMEBUFFER, rendererID);

		// Attachments
		bool multisampled = specification.samples > 1;
		GLenum textureTarget = GetTextureTarget(multisampled);

		if (colorAttachmentSpecifications.size() != 0)
		{
			colorAttachments.resize(colorAttachmentSpecifications.size());
			CreateTextures(textureTarget, colorAttachments.data(), (unsigned int)colorAttachments.size());

			for (size_t i = 0; i < colorAttachments.size(); i++)
			{
				BindTextures(textureTarget, colorAttachments[i]);
				AttachColorTexture(colorAttachments[i], specification.samples, specification.width, specification.height, colorAttachmentSpecifications[i], (int)i);
			}
		}
	
		if (depthAttachmentSpecification.format != FramebufferTextureFormat::None)
		{
			CreateTextures(textureTarget, &depthAttachment, 1);
			BindTextures(textureTarget, depthAttachment);
			AttachDepthTexture(depthAttachment, specification.samples, specification.width, specification.height, depthAttachmentSpecification);
		}

		int maxColorAttachments = Renderer::GetMaxFramebufferColorAttachments();
		if (colorAttachments.size() > 1)
		{
			GBC_ASSERT(colorAttachments.size() <= maxColorAttachments, "Too many Framebuffer color attachments!");
			GLenum* buffers = (GLenum*)alloca(colorAttachments.size() * sizeof(GLenum));
			for (size_t i = 0; i < colorAttachments.size(); i++)
				buffers[i] = (GLenum)(GL_COLOR_ATTACHMENT0 + i);

			glDrawBuffers((int)colorAttachments.size(), buffers);
		}
		else if (colorAttachments.empty())
		{
			// Only depth-pass
			glDrawBuffer(GL_NONE);
		}

		GBC_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer incomplete!");

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFramebuffer::Clear()
	{
		if (rendererID != 0)
		{
			glDeleteFramebuffers(1, &rendererID);
			glDeleteTextures((int)colorAttachments.size(), colorAttachments.data());
			glDeleteTextures(1, &depthAttachment);

			rendererID = 0;
			colorAttachments.clear();
			depthAttachment = 0;
		}
	}

	void OpenGLFramebuffer::Bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, rendererID);
		glViewport(0, 0, specification.width, specification.height);
	}

	void OpenGLFramebuffer::Unbind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFramebuffer::OnViewportResize(int width, int height)
	{
		GBC_ASSERT(width <= Renderer::GetMaxFramebufferWidth() && height <= Renderer::GetMaxFramebufferHeight(), "Framebuffer too large!");
		specification.width = width;
		specification.height = height;
		Recreate();
	}
}
