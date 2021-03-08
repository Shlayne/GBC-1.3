#pragma once

#include <memory>
#include "Rendering/Shader.h"
#include "Rendering/OpenGLTexture.h"
#include "Rendering/Renderer.h"
#include "BasicVertex.h"
#include "BasicModel.h"

class BasicRenderer
{
private:
	static void Init();
	static void Shutdown();
	friend class Renderer;
public:
	static void BeginScene(const glm::mat4& cameraTransform, const glm::mat4& projection);
	static void EndScene();

	static void Submit(const BasicModel& model, const glm::mat4& transform = glm::mat4(1.0f), const OpenGLTexture* texture = nullptr);
private:
	static void EnsureBatch(unsigned int vertexCount, unsigned int indexCount, unsigned int texIndex = 0);
	static void Reset();
	static unsigned int GetTextureIndex(const OpenGLTexture* texture);
};
