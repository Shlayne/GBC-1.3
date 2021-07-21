#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include "glm/glm.hpp"

enum class OBJErrorCode : uint8_t
{
	None = 0,

	MissingFile,
	InvalidVertexData,
	InvalidTexCoordData,
	InvalidNormalData,
	InvalidFaceData,
	InvalidLineData
};

struct OBJError
{
	OBJErrorCode code = OBJErrorCode::None;
	size_t line = 0;
};

struct OBJResult
{
	OBJError error{};

	inline operator bool() const noexcept
	{
		return error.code == OBJErrorCode::None;
	}
};

struct OBJModel
{
	OBJModel() = default;
	OBJModel(OBJModel&& model) noexcept;
	OBJModel& operator=(OBJModel&& model) noexcept;

	std::string filepath;

	std::vector<glm::vec3> positions;
	std::vector<glm::vec2> texCoords;
	std::vector<glm::vec3> normals;
	std::vector<glm::uvec3> indices;
	std::vector<uint32_t> edgeIndices;
};

class OBJLoader
{
private:
	OBJLoader() = delete;
	OBJLoader(const OBJLoader&) = delete;
	OBJLoader(OBJLoader&&) = delete;
	OBJLoader& operator=(const OBJLoader&) = delete;
	OBJLoader& operator=(OBJLoader&&) = delete;
	~OBJLoader() = delete;
public:
	static OBJResult LoadOBJ(const std::string& filepath, OBJModel& outModel);
	static void LogError(const OBJResult& result);
};
