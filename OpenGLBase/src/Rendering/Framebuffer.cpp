#include "Framebuffer.h"
#include "Platform/OpenGL/OpenGLFramebuffer.h"

Ref<Framebuffer> Framebuffer::Create(FramebufferSpecification specification)
{
	return CreateRef<OpenGLFramebuffer>(specification);
}
