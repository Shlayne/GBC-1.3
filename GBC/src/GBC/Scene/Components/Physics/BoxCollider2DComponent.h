#pragma once

#include <glm/glm.hpp>

namespace gbc
{
	struct BoxCollider2DComponent
	{
		glm::vec2 size{ 0.5f };
		glm::vec2 offset{ 0.0f };

		// TODO: move these four into a physics material
		float density = 1.0f;
		float friction = 0.5f;
		float restitution = 0.0f;
		float restitutionThreshold = 0.5f;

		void* runtimeFixture = nullptr;
	};
}
