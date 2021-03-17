#include "gbcpch.h"
#include "Math.h"

namespace gbc
{
	// From "glm/gtx/matrix_decompose.inl"
	bool Math::Decompose(const glm::mat4& transform, glm::vec3& translation, glm::vec3& rotation, glm::vec3& scale)
	{
		glm::mat4 localMatrix = transform;

		// Normalize the matrix
		if (glm::epsilonEqual(localMatrix[3][3], 0.0f, glm::epsilon<float>()))
			return false;

		for (glm::length_t i = 0; i < 4; i++)
			for (glm::length_t j = 0; j < 4; j++)
				localMatrix[i][j] /= localMatrix[3][3];

		// Isolate perspective
		if (glm::epsilonNotEqual(localMatrix[0][3], 0.0f, glm::epsilon<float>()) ||
			glm::epsilonNotEqual(localMatrix[1][3], 0.0f, glm::epsilon<float>()) ||
			glm::epsilonNotEqual(localMatrix[2][3], 0.0f, glm::epsilon<float>()))
		{
			// Clear perspective
			localMatrix[0][3] = 0.0f;
			localMatrix[1][3] = 0.0f;
			localMatrix[2][3] = 0.0f;
			localMatrix[3][3] = 1.0f;
		}

		translation = localMatrix[3];
		localMatrix[3] = glm::vec4(0.0f, 0.0f, 0.0f, localMatrix[3].w);

		// Get scale and shear
		glm::vec3 row[3];
		for (glm::length_t i = 0; i < 3; i++)
			row[i] = localMatrix[i];

		scale[0] = glm::length(row[0]);
		row[0] = glm::detail::scale(row[0], 1.0f);

		row[1] = glm::detail::combine(row[1], row[0], 1.0f, -glm::dot(row[0], row[1]));

		scale[1] = glm::length(row[1]);
		row[1] = glm::detail::scale(row[1], 1.0f);

		row[2] = glm::detail::combine(row[2], row[0], 1.0f, -glm::dot(row[0], row[2]));
		row[2] = glm::detail::combine(row[2], row[1], 1.0f, -glm::dot(row[1], row[2]));

		scale[2] = glm::length(row[2]);
		row[2] = glm::detail::scale(row[2], 1.0f);

		// Get rotation
		rotation.y = asinf(-row[0].z);
		if (cosf(rotation.y) != 0.0f)
		{
			rotation.x = atan2f(row[1].z, row[2].z);
			rotation.z = atan2f(row[0].y, row[0].x);
		}
		else
		{
			rotation.x = atan2f(-row[2].x, row[1].y);
			rotation.z = 0.0f;
		}

		return true;
	}
}
