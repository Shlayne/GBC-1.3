#include "gbcpch.h"
#include "TransformComponent.h"

namespace gbc
{
	TransformComponent::TransformComponent(const glm::vec3& translation) noexcept
		: translation(translation) {}

	TransformComponent::operator glm::mat4() const noexcept
	{
		return glm::translate(glm::mat4(1.0f), translation) *
			glm::toMat4(glm::quat(rotation)) *
			glm::scale(glm::mat4(1.0f), scale);
	}
}
