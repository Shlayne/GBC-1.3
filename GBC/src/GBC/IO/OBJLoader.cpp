#include "gbcpch.h"
#include "OBJLoader.h"

void OBJModel::Clear()
{
	filepath.clear();
	positions.clear();
	texCoords.clear();
	normals.clear();
	indices.clear();
}

// These were fun to make... okay maybe not...
static const std::regex positionRegex("^\\s*v\\s+(?:[-+]?(?:\\d+(?:\\.\\d*)?|\\.\\d+)(?:[eE][-+]?\\d+)?)\\s+(?:[-+]?(?:\\d+(?:\\.\\d*)?|\\.\\d+)(?:[eE][-+]?\\d+)?)\\s+(?:[-+]?(?:\\d+(?:\\.\\d*)?|\\.\\d+)(?:[eE][-+]?\\d+)?)\\s*(?:\\s#.*?)?$");
static const std::regex texCoordRegex("^\\s*vt\\s+(?:[-+]?(?:\\d+(?:\\.\\d*)?|\\.\\d+)(?:[eE][-+]?\\d+)?)\\s+(?:[-+]?(?:\\d+(?:\\.\\d*)?|\\.\\d+)(?:[eE][-+]?\\d+)?)\\s*(?:\\s#.*?)?$");
static const std::regex normalRegex("^\\s*vn\\s+(?:[-+]?(?:\\d+(?:\\.\\d*)?|\\.\\d+)(?:[eE][-+]?\\d+)?)\\s+(?:[-+]?(?:\\d+(?:\\.\\d*)?|\\.\\d+)(?:[eE][-+]?\\d+)?)\\s+(?:[-+]?(?:\\d+(?:\\.\\d*)?|\\.\\d+)(?:[eE][-+]?\\d+)?)\\s*(?:\\s#.*?)?$");
static const std::regex faceRegex1("^\\s*f\\s+.*?\\s*(?:\\s#.*?)?$");
static const std::regex faceRegex2("^(?:\\d+/\\d+/\\d+|\\d+//?\\d+|\\d+)$");

OBJModel OBJLoader::LoadOBJ(const std::string& filepath, bool triangulateFaces)
{
	OBJModel modelOut;

	std::ifstream file(filepath);
	if (file.is_open())
	{
		modelOut.filepath = filepath;

		while (!file.eof())
		{
			std::string originalLine;
			std::getline(file, originalLine);
			std::stringstream line(originalLine);

			std::string token0;
			line >> token0;

			if (token0 == "v")
			{
				if (!std::regex_match(originalLine, positionRegex)) { modelOut.Clear(); return modelOut; }
				float x, y, z;
				line >> x >> y >> z;
				if (!line) { modelOut.Clear(); return modelOut; }
				modelOut.positions.emplace_back(x, y, z);
			}
			else if (token0 == "vt")
			{
				if (!std::regex_match(originalLine, texCoordRegex)) { modelOut.Clear(); return modelOut; }
				float x, y;
				line >> x >> y;
				if (!line) { modelOut.Clear(); return modelOut; }
				modelOut.texCoords.emplace_back(x, y);
			}
			else if (token0 == "vn")
			{
				if (!std::regex_match(originalLine, normalRegex)) { modelOut.Clear(); return modelOut; }
				float x, y, z;
				line >> x >> y >> z;
				if (!line) { modelOut.Clear(); return modelOut; }
				modelOut.normals.emplace_back(x, y, z);
			}
			else if (token0 == "f")
			{
				if (!std::regex_match(originalLine, faceRegex1)) { modelOut.Clear(); return modelOut; }
				
				std::string originalVertex;
				glm::uvec3 index(0);
				std::vector<glm::uvec3> indices;
				unsigned int vertexCount = 0;

				for (line >> originalVertex; line && originalVertex[0] != '#'; line >> originalVertex)
				{
					if (!std::regex_match(originalVertex, faceRegex2)) { modelOut.Clear(); return modelOut; }
					std::stringstream vertex(originalVertex);

					// Must exist
					std::string positionIndex;
					std::getline(vertex, positionIndex, '/');
					if (positionIndex.empty()) { modelOut.Clear(); return modelOut; }
					index.x = (unsigned int)std::stoul(positionIndex) - 1;

					// Might not exist
					std::string texCoordIndex;
					std::getline(vertex, texCoordIndex, '/');
					if (!texCoordIndex.empty())
						index.y = (unsigned int)std::stoul(texCoordIndex) - 1;

					// Might not exist
					std::string normalIndex;
					std::getline(vertex, normalIndex, '/');
					if (!normalIndex.empty())
						index.z = (unsigned int)std::stoul(normalIndex) - 1;

					indices.push_back(index);
					vertexCount++;
				}

				if (vertexCount == 0 || (triangulateFaces && vertexCount < 3)) { modelOut.Clear(); return modelOut; }

				if (triangulateFaces && vertexCount != 3)
				{
					for (unsigned int i = 2; i < (unsigned int)indices.size(); i++)
					{
						modelOut.indices.push_back(indices[0]);
						modelOut.indices.push_back(indices[i - 1]);
						modelOut.indices.push_back(indices[i]);
					}
				}
				else
					modelOut.indices.insert(modelOut.indices.end(), indices.begin(), indices.end());
			}
			else if (!token0.empty() && token0[0] != '#')
			{
				modelOut.Clear();
				return modelOut;
			}
		}

		file.close();
	}
	return modelOut;
}
