#include "gbcpch.h"
#include "OrthographicCameraControllerScript.h"
#include "GBC/Core/Input.h"
#include "GBC/Scene/Components/TransformComponent.h"

namespace gbc
{
	void OrthographicCameraControllerScript::OnUpdate(Timestep timestep)
	{
		int movementDirection = 0;
		if (Input::IsKeyPressed(Keycode::W)) movementDirection++; 
		if (Input::IsKeyPressed(Keycode::S)) movementDirection--;
		if (movementDirection != 0)
			translation += (float)movementDirection * speed * timestep * forward;

		int rotationDirection = 0;
		if (Input::IsKeyPressed(Keycode::A)) rotationDirection--;
		if (Input::IsKeyPressed(Keycode::D)) rotationDirection++;
		if (rotationDirection != 0)
		{
			rotation += (float)rotationDirection * sensitivity * timestep;
			forward = glm::vec2(sinf(rotation), cosf(rotation));
		}

		if (movementDirection != 0 || rotationDirection != 0)
		{
			glm::mat4 rot(1.0f);
			rot[0][0] = forward.y; rot[0][1] = -forward.x;
			rot[1][0] = forward.x; rot[1][1] = forward.y;

			glm::mat4& transform = Get<TransformComponent>();
			transform = glm::translate(glm::mat4(1.0f), glm::vec3(translation, 0.0f)) * rot;
		}
	}
}
