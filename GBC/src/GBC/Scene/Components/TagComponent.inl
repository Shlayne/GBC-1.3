namespace gbc
{
	constexpr TagComponent::TagComponent(const std::string& tag)
		: tag(tag) {}

	constexpr TagComponent::TagComponent(std::string&& tag) noexcept
		: tag(std::move(tag)) {}

	constexpr TagComponent& TagComponent::operator=(const std::string& tag)
	{
		this->tag = tag;
		return *this;
	}

	constexpr TagComponent& TagComponent::operator=(std::string&& tag) noexcept
	{
		this->tag = std::move(tag);
		return *this;
	}
}
