#pragma once

namespace gbc
{
	struct Rigidbody2DComponent
	{
		enum class BodyType { Static, Dynamic, Kinematic };

		Rigidbody2DComponent() = default;
		Rigidbody2DComponent(BodyType bodyType) : bodyType(bodyType) {}
		Rigidbody2DComponent(const Rigidbody2DComponent&) = default;

		BodyType bodyType = BodyType::Dynamic;
		bool fixedRotation = false;

		void* runtimeBody = nullptr;
	};
}
