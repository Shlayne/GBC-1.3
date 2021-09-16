#include "gbcpch.h"
#include "OBJLoader.h"

OBJModel::OBJModel(OBJModel&& model) noexcept
	: filepath(std::move(filepath)), positions(std::move(positions)), texCoords(std::move(texCoords)), normals(std::move(normals)), indices(std::move(indices)), edgeIndices(std::move(edgeIndices)) {}

OBJModel& OBJModel::operator=(OBJModel&& model) noexcept
{
	if (this != &model)
	{
		filepath = std::move(filepath);
		positions = std::move(positions);
		texCoords = std::move(texCoords);
		normals = std::move(normals);
		indices = std::move(indices);
		edgeIndices = std::move(edgeIndices);
	}
	return *this;
}

#define OBJERROR(errorCode) do { result.error.code = (errorCode); result.error.line = lineNumber; return result; } while (false)
#define OBJERROR_LINE(errorCode, errorLine) do { result.error.code = (errorCode); result.error.line = (errorLine); return result; } while (false)

OBJResult OBJLoader::LoadOBJ(const std::string& filepath, OBJModel& outModel)
{
	OBJResult result;
	outModel.filepath = filepath;

	std::ifstream file(filepath);
	if (!file.is_open())
		OBJERROR_LINE(OBJErrorCode::MissingFile, 0);

	size_t lineNumber = 0;

	while (!file.eof())
	{
		lineNumber++;
		std::string originalLine;
		std::getline(file, originalLine);
		std::stringstream line(originalLine);

		std::string token0;
		line >> token0;

		if (token0 == "v")
		{
			float x, y, z;
			line >> x >> y >> z;
			if (!line)
				OBJERROR(OBJErrorCode::InvalidVertexData);
			outModel.positions.emplace_back(x, y, z);
		}
		else if (token0 == "vt")
		{
			float x, y;
			line >> x >> y;
			if (!line)
				OBJERROR(OBJErrorCode::InvalidTexCoordData);
			outModel.texCoords.emplace_back(x, y);
		}
		else if (token0 == "vn")
		{
			float x, y, z;
			line >> x >> y >> z;
			if (!line)
				OBJERROR(OBJErrorCode::InvalidNormalData);
			outModel.normals.emplace_back(x, y, z);
		}
		else if (token0 == "f")
		{
			std::string originalVertex;
			glm::uvec3 index{ 0 };
			std::vector<glm::uvec3> indices;
			uint32_t vertexCount = 0;

			for (line >> originalVertex; line && originalVertex[0] != '#'; line >> originalVertex)
			{
				std::stringstream vertex(originalVertex);

				// Must exist
				std::string positionIndex;
				std::getline(vertex, positionIndex, '/');
				if (positionIndex.empty())
					OBJERROR(OBJErrorCode::InvalidFaceData);
				index.x = static_cast<uint32_t>(std::stoul(positionIndex) - 1);

				// Might not exist
				std::string texCoordIndex;
				std::getline(vertex, texCoordIndex, '/');
				if (!texCoordIndex.empty())
					index.y = static_cast<uint32_t>(std::stoul(texCoordIndex) - 1);

				// Might not exist
				std::string normalIndex;
				std::getline(vertex, normalIndex, '/');
				if (!normalIndex.empty())
					index.z = static_cast<uint32_t>(std::stoul(normalIndex) - 1);

				indices.push_back(index);
				vertexCount++;
			}

			if (!vertexCount)
				OBJERROR(OBJErrorCode::InvalidFaceData);

			outModel.indices.insert(outModel.indices.end(), indices.begin(), indices.end());
		}
		else if (token0 == "l")
		{
			uint32_t index1;
			uint32_t index2;
			line >> index1 >> index2;
			if (!line)
				OBJERROR(OBJErrorCode::InvalidLineData);
			outModel.edgeIndices.push_back(index1);
			outModel.edgeIndices.push_back(index2);
		}
		// For now, just ignore unknown commands
		//else if (!token0.empty() && token0[0] != '#')
		//	OBJERROR(OBJErrorCode::InvalidCommand);
	}

	file.close();
	return result;
}

void OBJLoader::LogError(const OBJResult& result)
{
	const char* message;
	switch (result.error.code)
	{
		case OBJErrorCode::None: return;
		case OBJErrorCode::MissingFile: message = "Missing file."; break;
		case OBJErrorCode::InvalidVertexData: message = "Invalid vertex position data."; break;
		case OBJErrorCode::InvalidTexCoordData: message = "Invalid texture coordinate data."; break;
		case OBJErrorCode::InvalidNormalData: message = "Invalid normal data."; break;
		case OBJErrorCode::InvalidFaceData: message = "Invalid face data."; break;
		case OBJErrorCode::InvalidLineData: message = "Invalid line data."; break;
		default: message = "Unknown error."; break;
	}

	GBC_CORE_INFO("Failed to load 3D model on line {0}. Error: {1}", result.error.line, message);
}
