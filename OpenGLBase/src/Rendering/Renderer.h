#pragma once

#include <glm/glm.hpp>
#include "VertexArray.h"
#include "Buffer.h"

class Renderer
{
private:
	static void Init();
	static void Shutdown();
	friend class Application;
public:
	// Query commands
	// TODO: these belong in an opengl context class
	static int GetMaxTextureSlots();
	static int GetMaxTextureSize();
	static int GetMaxFramebufferSize();
	static int GetMaxFramebufferColorAttachments();

	// Event-based commands
	static void OnViewportResized(int width, int height);

	// General commands
	static void SetClearColor(const glm::vec4& color);
	static void Clear();

	// Enable/disable commands
	static void EnableDepthTest(bool enable = true);
	static void EnableDepthMask(bool enable = true);
	static void EnableCullFace(bool enable = true);
	static void EnableBlending(bool enable = true);
	
	// Rendering commands
	static void DrawIndexed(const VertexArray& vertexArray, const IndexBuffer& indexBuffer, unsigned int maxIndexCount = 0);
};
