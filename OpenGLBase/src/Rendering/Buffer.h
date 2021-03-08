#pragma once

#include <vector>

enum class ElementType : unsigned char
{
	// If more are added, just set them to something like this
	Int    = 0x81,
	Int2   = 0x82,
	Int3   = 0x83,
	Int4   = 0x84,
	Float  = 0x01,
	Float2 = 0x02,
	Float3 = 0x03,
	Float4 = 0x04
};

struct BufferElement
{
	BufferElement(ElementType type, bool normalized = false)
		: type(type), normalized(normalized) {}

	ElementType type;
	bool normalized;
	int size = 0;
	int count = 0;
	int offset = 0;
};

class BufferLayout
{
public:
	BufferLayout() = default;
	BufferLayout(std::initializer_list<BufferElement> elements);

	inline const std::vector<BufferElement>& getElements() const { return elements; }
	inline int getStride() const { return stride; }
private:
	std::vector<BufferElement> elements;
	int stride = 0;
};

enum class BufferUsage
{
	StreamDraw,
	StreamRead,
	StreamCopy,
	StaticDraw,
	StaticRead,
	StaticCopy,
	DynamicDraw,
	DynamicRead,
	DynamicCopy
};

class VertexBuffer
{
public:
	VertexBuffer(unsigned int size, const void* data = nullptr, BufferUsage usage = BufferUsage::StaticDraw);
	~VertexBuffer();

	void bind() const;
	void unbind() const;

	void setData(unsigned int size, const void* data);

	inline const BufferLayout& getLayout() const { return layout; }
	inline void setLayout(const BufferLayout& layout) { this->layout = layout; }
private:
	unsigned int rendererID = 0;
	BufferLayout layout;
};

class IndexBuffer
{
public:
	IndexBuffer(unsigned int count, const void* data = nullptr, BufferUsage usage = BufferUsage::StaticDraw);
	~IndexBuffer();

	void bind() const;
	void unbind() const;

	void setData(unsigned int count, const void* data);

	inline unsigned int getCount() const { return count; }
private:
	unsigned int rendererID = 0;
	unsigned int count = 0;
};
