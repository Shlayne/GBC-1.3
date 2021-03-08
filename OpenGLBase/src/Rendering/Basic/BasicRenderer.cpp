#include "BasicRenderer.h"

#include <vector>

#include "IO/FileIO.h"

struct Vertex
{
	glm::vec3 position;
	glm::vec4 tintColor;
	glm::vec2 texCoord;
	float texIndex;
};

struct BasicRendererData
{
	std::unique_ptr<ShaderProgram> shader;
	std::unique_ptr<VertexArray> vertexArray;
	std::unique_ptr<VertexBuffer> vertexBuffer;
	std::unique_ptr<IndexBuffer> indexBuffer;

	unsigned int localIndexCount = 0;
	unsigned int* localIndexBufferStart = nullptr;
	unsigned int* localIndexBufferCurrent = nullptr;
	unsigned int localVertexCount = 0;
	Vertex* localVertexBufferStart = nullptr;
	Vertex* localVertexBufferCurrent = nullptr;

	const OpenGLTexture** textures = nullptr;
	unsigned int textureCount = 1;

	unsigned int maxVertices = 0;
	unsigned int maxIndices = 0;
	unsigned int maxTextures = 0;
} data;

void BasicRenderer::init()
{
	Renderer::enableDepthTest();
	Renderer::enableBlending();
	Renderer::enableCullFace();

	// Setup texture slots
	data.maxTextures = (unsigned int)Renderer::getMaxTextureSlots();
	data.textures = new const OpenGLTexture*[data.maxTextures];

	// Setup local buffers
	data.maxVertices = 900000; // TODO: query drivers
	data.maxIndices  = 600000; // TODO: query drivers
	data.localVertexBufferStart = new Vertex[data.maxVertices];
	data.localVertexBufferCurrent = data.localVertexBufferStart;
	data.localIndexBufferStart = new unsigned int[data.maxIndices];
	data.localIndexBufferCurrent = data.localIndexBufferStart;

	// Setup internal buffers
	data.vertexBuffer.reset(new VertexBuffer(data.maxVertices * sizeof(Vertex), nullptr, BufferUsage::DynamicDraw));
	data.vertexBuffer->setLayout({
		{ ElementType::Float3 },
		{ ElementType::Float4 },
		{ ElementType::Float2 },
		{ ElementType::Float  }
	});

	data.vertexArray.reset(new VertexArray());
	data.vertexArray->addVertexBuffer(data.vertexBuffer.get());

	data.indexBuffer.reset(new IndexBuffer(data.maxIndices, nullptr, BufferUsage::DynamicDraw));

	// Setup shader
	int* samplers = new int[data.maxTextures];
	for (unsigned int i = 0; i < data.maxTextures; i++)
		samplers[i] = i;

	std::vector<ShaderFile> shaderFiles = {
		{ShaderType::Vertex, FileIO::readFile("resources/shaders/Basic_vertex.glsl")},
		{ShaderType::Fragment, FileIO::readFile("resources/shaders/Basic_fragment.glsl")}
	};
	data.shader.reset(new ShaderProgram(shaderFiles));
	data.shader->bind();
	data.shader->setUniforms("textures", samplers, data.maxTextures);

	Texture whiteTexture(1, 1, 4);
	unsigned int* whiteTextureData = (unsigned int*)whiteTexture.getData();
	*whiteTextureData = 0xffffffff;
	data.textures[0] = new OpenGLTexture(&whiteTexture);

	delete[] samplers;
}

void BasicRenderer::shutdown()
{
	delete[] data.localVertexBufferStart;
	delete[] data.localIndexBufferStart;
	delete data.textures[0];
	delete[] data.textures;
}

void BasicRenderer::beginScene(const glm::mat4& cameraTransform, const glm::mat4& projection)
{
	// Set shader uniforms
	data.shader->bind();
	data.shader->setUniform("projection", projection);
	data.shader->setUniform("cameraTransform", cameraTransform);
}

void BasicRenderer::endScene()
{
	unsigned int vertexBufferSize = (unsigned int)(data.localVertexCount * sizeof(Vertex));
	unsigned int indexBufferSize = (unsigned int)(data.localIndexCount * sizeof(unsigned int));

	if (vertexBufferSize != 0 && indexBufferSize != 0)
	{
		// Copy local buffers to internal buffers
		data.vertexBuffer->setData(vertexBufferSize, data.localVertexBufferStart);
		data.indexBuffer->setData(indexBufferSize, data.localIndexBufferStart);

		// Bind textures
		for (unsigned int i = 0; i < data.textureCount; i++)
			data.textures[i]->bind(i);

		// Actually render
		Renderer::drawIndexed(*data.vertexArray, *data.indexBuffer, data.localIndexCount);
		reset();
	}
}

void BasicRenderer::ensureBatch(unsigned int vertexCount, unsigned int indexCount, unsigned int texIndex)
{
	if (data.localVertexCount + vertexCount >= data.maxVertices ||
		data.localIndexCount + indexCount >= data.maxIndices ||
		texIndex >= data.maxTextures)
		endScene();
}

void BasicRenderer::reset()
{
	// Reset local buffers
	data.localVertexCount = 0;
	data.localVertexBufferCurrent = data.localVertexBufferStart;

	data.localIndexCount = 0;
	data.localIndexBufferCurrent = data.localIndexBufferStart;

	data.textureCount = 1;
}

unsigned int BasicRenderer::getTextureIndex(const OpenGLTexture* texture)
{
	if (texture == nullptr)
		return 0;

	for (unsigned int i = 0; i < data.textureCount; i++)
		if (*(texture->getTexture()) == *(data.textures[i]->getTexture()))
			return i;

	return data.textureCount;
}

void BasicRenderer::submit(const BasicModel& model, const glm::mat4& transform, const OpenGLTexture* texture)
{
	// Handle textures
	unsigned int texIndex = getTextureIndex(texture);
	ensureBatch(model.vertexCount, model.indexCount, texIndex);
	if (texIndex >= data.textureCount)
		data.textures[data.textureCount++] = texture;

	// Handle vertices
	for (unsigned int i = 0; i < model.vertexCount; i++, data.localVertexBufferCurrent++)
	{
		data.localVertexBufferCurrent->position = transform * glm::vec4(model.vertices[i].position, 1.0f);
		data.localVertexBufferCurrent->texCoord = model.vertices[i].texCoord;
		data.localVertexBufferCurrent->tintColor = model.vertices[i].tintColor;
		data.localVertexBufferCurrent->texIndex = (float)texIndex;
	}

	// Handle indices
	for (unsigned int i = 0; i < model.indexCount; i++, data.localIndexBufferCurrent++)
		*data.localIndexBufferCurrent = data.localVertexCount + model.indices[i];

	data.localVertexCount += model.vertexCount;
	data.localIndexCount += model.indexCount;
}
