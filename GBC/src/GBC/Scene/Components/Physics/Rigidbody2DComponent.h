#pragma once

namespace gbc
{
	struct Rigidbody2DComponent
	{
		enum class BodyType { Static, Dynamic, Kinematic };

		BodyType type = BodyType::Dynamic;
		bool fixedRotation = false;

		void* runtimeBody = nullptr;
	};
}
