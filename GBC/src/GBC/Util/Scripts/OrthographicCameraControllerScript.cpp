#include "gbcpch.h"
#include "OrthographicCameraControllerScript.h"
#include "GBC/Core/Input.h"
#include "GBC/Scene/Components/TransformComponent.h"

namespace gbc
{
	void OrthographicCameraControllerScript::OnUpdate(Timestep timestep)
	{
		TransformComponent& transform = GetComponent<TransformComponent>();

		int32_t movementDirection = 0;
		if (Input::IsKeyPressed(Keycode::W)) movementDirection++; 
		if (Input::IsKeyPressed(Keycode::S)) movementDirection--;
		if (movementDirection != 0)
			transform.translation += movementDirection * speed * timestep * forward;

		int32_t rotationDirection = 0;
		if (Input::IsKeyPressed(Keycode::A)) rotationDirection--;
		if (Input::IsKeyPressed(Keycode::D)) rotationDirection++;
		if (rotationDirection != 0)
		{
			transform.rotation.z += rotationDirection * sensitivity * timestep;
			forward = { sin(transform.rotation.z), cos(transform.rotation.z), 0.0f };
		}
	}
}
