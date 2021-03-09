#include "cbcpch.h"
#include "BasicRenderer.h"
#include "IO/FileIO.h"
#include "Rendering/Renderer.h"
#include "Rendering/Shader.h"

namespace cbc
{
	struct Vertex
	{
		glm::vec3 position;
		glm::vec4 tintColor;
		glm::vec2 texCoord;
		float texIndex;
	};

	struct BasicRendererData
	{
		Ref<Shader> shader;
		Ref<VertexArray> vertexArray;
		Ref<VertexBuffer> vertexBuffer;
		Ref<IndexBuffer> indexBuffer;

		unsigned int localIndexCount = 0;
		unsigned int* localIndexBufferStart = nullptr;
		unsigned int* localIndexBufferCurrent = nullptr;
		unsigned int localVertexCount = 0;
		Vertex* localVertexBufferStart = nullptr;
		Vertex* localVertexBufferCurrent = nullptr;

		Ref<Texture>* textures = nullptr;
		unsigned int textureCount = 1;

		unsigned int maxVertices = 0;
		unsigned int maxIndices = 0;
		unsigned int maxTextures = 0;
	};
	static BasicRendererData data;

	void BasicRenderer::Init()
	{
		Renderer::EnableDepthTest();
		Renderer::EnableBlending();
		Renderer::EnableCullFace();

		// Setup texture slots
		data.maxTextures = (unsigned int)Renderer::GetMaxTextureSlots();
		data.textures = new Ref<Texture>[data.maxTextures];

		// Setup local buffers
		data.maxVertices = 900000; // TODO: query drivers
		data.maxIndices  = 600000; // TODO: query drivers
		data.localVertexBufferStart = new Vertex[data.maxVertices];
		data.localVertexBufferCurrent = data.localVertexBufferStart;
		data.localIndexBufferStart = new unsigned int[data.maxIndices];
		data.localIndexBufferCurrent = data.localIndexBufferStart;

		// Setup internal buffers
		data.vertexBuffer = VertexBuffer::CreateRef(data.maxVertices * sizeof(Vertex), nullptr, BufferUsage::DynamicDraw);
		data.vertexBuffer->SetLayout({
			{BufferElementType::Float3},
			{BufferElementType::Float4},
			{BufferElementType::Float2},
			{BufferElementType::Float }
		});

		data.vertexArray = VertexArray::CreateRef();
		data.vertexArray->AddVertexBuffer(data.vertexBuffer);

		data.indexBuffer = IndexBuffer::CreateRef(data.maxIndices, nullptr, BufferUsage::DynamicDraw);

		// Setup shader
		data.shader = Shader::CreateRef({
			{ShaderType::Vertex, FileIO::ReadFile("resources/shaders/Basic_vertex.glsl")},
			{ShaderType::Fragment, FileIO::ReadFile("resources/shaders/Basic_fragment.glsl")}
		});

		data.shader->Bind();
		int* samplers = new int[data.maxTextures];
		for (unsigned int i = 0; i < data.maxTextures; i++)
			samplers[i] = i;
		data.shader->SetUniforms("textures", samplers, data.maxTextures);
		delete[] samplers;

		// Setup white texture
		Ref<LocalTexture2D> whiteTexture = CreateRef<LocalTexture2D>(1, 1, 4);
		*(unsigned int*)whiteTexture->GetData() = 0xffffffff;
		data.textures[0] = Texture::CreateRef(whiteTexture);
	}

	void BasicRenderer::Shutdown()
	{
		delete[] data.localVertexBufferStart;
		delete[] data.localIndexBufferStart;
		delete[] data.textures;
	}

	void BasicRenderer::BeginScene(const glm::mat4& cameraTransform, const glm::mat4& projection)
	{
		// Set shader uniforms
		data.shader->Bind();
		data.shader->SetUniform("projection", projection);
		data.shader->SetUniform("cameraTransform", cameraTransform);
	}

	void BasicRenderer::EndScene()
	{
		unsigned int vertexBufferSize = (unsigned int)(data.localVertexCount * sizeof(Vertex));
		unsigned int indexBufferSize = (unsigned int)(data.localIndexCount * sizeof(unsigned int));

		if (vertexBufferSize != 0 && indexBufferSize != 0)
		{
			// Copy local buffers to internal buffers
			data.vertexBuffer->SetData(vertexBufferSize, data.localVertexBufferStart);
			data.indexBuffer->SetData(indexBufferSize, data.localIndexBufferStart);

			// Bind textures
			for (unsigned int i = 0; i < data.textureCount; i++)
				data.textures[i]->Bind(i);

			// Actually render
			Renderer::DrawIndexed(data.vertexArray, data.indexBuffer, data.localIndexCount);
			Reset();
		}
	}

	void BasicRenderer::EnsureBatch(unsigned int vertexCount, unsigned int indexCount, unsigned int texIndex)
	{
		if (data.localVertexCount + vertexCount > data.maxVertices ||
			data.localIndexCount + indexCount > data.maxIndices ||
			texIndex >= data.maxTextures)
			EndScene();
	}

	void BasicRenderer::Reset()
	{
		// Reset local buffers
		data.localVertexCount = 0;
		data.localVertexBufferCurrent = data.localVertexBufferStart;

		data.localIndexCount = 0;
		data.localIndexBufferCurrent = data.localIndexBufferStart;

		// Remove the references once the textures has been rendered
		for (unsigned int i = 1; i < data.textureCount; i++)
			data.textures[i] = nullptr;
		data.textureCount = 1;
	}

	unsigned int BasicRenderer::GetTextureIndex(const Ref<Texture>& texture)
	{
		if (texture == nullptr)
			return 0;

		for (unsigned int i = 1; i < data.textureCount; i++)
			if (*(texture->GetTexture()) == *(data.textures[i]->GetTexture()))
				return i;

		return data.textureCount;
	}

	void BasicRenderer::Submit(const BasicModel& model, const glm::mat4& transform, Ref<Texture> texture)
	{
		// Handle textures
		unsigned int texIndex = GetTextureIndex(texture);
		EnsureBatch(model.vertexCount, model.indexCount, texIndex);
		if (texIndex >= data.textureCount)
			data.textures[texIndex = data.textureCount++] = texture;

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

		// Update local counts
		data.localVertexCount += model.vertexCount;
		data.localIndexCount += model.indexCount;
	}
}
