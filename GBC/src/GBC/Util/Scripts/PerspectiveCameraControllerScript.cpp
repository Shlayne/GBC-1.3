#include "gbcpch.h"
#include "PerspectiveCameraControllerScript.h"
#include "GBC/Core/Input.h"
#include "GBC/Scene/Components/TransformComponent.h"

namespace gbc
{
	void PerspectiveCameraControllerScript::OnCreate()
	{
		lastMousePos = Input::GetMousePos();
	}

	void PerspectiveCameraControllerScript::OnUpdate(Timestep timestep)
	{
		float movementSpeed = speed * timestep;

		int movementForward = 0;
		int movementRight = 0;
		int movementUp = 0;
		glm::vec3 forward = movementSpeed * glm::vec3(cosf(rotation.y), 0.0f, sinf(rotation.y));
		glm::vec3 right = glm::cross(forward, glm::vec3(0.0f, 1.0f, 0.0f));

		if (Input::IsKeyPressed(Keycode::W)) movementForward--;
		if (Input::IsKeyPressed(Keycode::S)) movementForward++;
		if (Input::IsKeyPressed(Keycode::A)) movementRight--;
		if (Input::IsKeyPressed(Keycode::D)) movementRight++;
		if (Input::IsKeyPressed(Keycode::LeftShift)) movementUp--;
		if (Input::IsKeyPressed(Keycode::Space)) movementUp++;

		if (movementForward != 0)
		{
			translation.z += (float)movementForward * forward.x;
			translation.x += (float)movementForward * forward.z;
		}
		if (movementRight != 0)
		{
			translation.x += (float)movementRight * right.z;
			translation.z += (float)movementRight * right.x;
		}
		if (movementUp != 0)
			translation.y += (float)movementUp * movementSpeed;

		glm::vec2 mousePos = Input::GetMousePos();
		glm::vec2 mouseDPos = mousePos - lastMousePos;
		lastMousePos = mousePos;

		if (Input::IsMouseButtonPressed(Mousecode::ButtonLeft))
		{
			float rotationSpeed = sensitivity * timestep;
			rotation.y -= mouseDPos.x * rotationSpeed;
			rotation.x = std::min(std::max(rotation.x - mouseDPos.y * rotationSpeed, minPitch), maxPitch);
		}

		glm::mat4& transform = Get<TransformComponent>();
		transform = glm::translate(glm::mat4(1.0f), translation) * glm::toMat4(glm::quat(rotation));
	}
}
