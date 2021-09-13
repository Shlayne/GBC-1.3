#include "gbcpch.h"
#include "TagComponent.h"

namespace gbc
{
	TagComponent::TagComponent(const std::string& tag)
		: tag(tag) {}

	TagComponent::TagComponent(std::string&& tag) noexcept
		: tag(std::move(tag)) {}

	TagComponent& TagComponent::operator=(const std::string& tag)
	{
		this->tag = tag;
		return *this;
	}

	TagComponent& TagComponent::operator=(std::string&& tag) noexcept
	{
		this->tag = std::move(tag);
		return *this;
	}
}
