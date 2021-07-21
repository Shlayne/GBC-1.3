#include "gbcpch.h"
#include "OrthographicCameraControllerScript.h"
#include "GBC/Core/Input.h"
#include "GBC/Scene/Components/TransformComponent.h"

namespace gbc
{
	void OrthographicCameraControllerScript::OnUpdate(Timestep timestep)
	{
		TransformComponent& transform = GetComponent<TransformComponent>();

		int movementDirection = 0;
		if (Input::IsKeyPressed(Keycode::W)) movementDirection++; 
		if (Input::IsKeyPressed(Keycode::S)) movementDirection--;
		if (movementDirection != 0)
			transform.translation += movementDirection * speed * timestep * forward;

		int rotationDirection = 0;
		if (Input::IsKeyPressed(Keycode::A)) rotationDirection--;
		if (Input::IsKeyPressed(Keycode::D)) rotationDirection++;
		if (rotationDirection != 0)
		{
			transform.rotation.z += rotationDirection * sensitivity * timestep;
			forward = glm::vec3(sinf(transform.rotation.z), cosf(transform.rotation.z), 0.0f);
		}
	}
}
