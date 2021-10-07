#pragma once

namespace gbc
{
	struct Rigidbody2DComponent
	{
		enum class BodyType { Static, Dynamic, Kinematic };

		constexpr Rigidbody2DComponent() = default;
		constexpr Rigidbody2DComponent(const Rigidbody2DComponent&) = default;
		constexpr Rigidbody2DComponent(BodyType bodyType) : bodyType(bodyType) {}

		BodyType bodyType = BodyType::Dynamic;
		bool fixedRotation = false;

		void* runtimeBody = nullptr;
	};
}
