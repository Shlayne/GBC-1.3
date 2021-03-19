#include "gbcpch.h"
#include "EditorCamera.h"
#include "GBC/Core/Input.h"

namespace gbc
{
	EditorCamera::EditorCamera(float fov, float nearClip, float farClip)
		: Camera(), fov(fov), nearClip(nearClip), farClip(farClip)
	{
		RecalculateView();
	}

	void EditorCamera::OnUpdate(Timestep timestep)
	{
		if (Input::IsKeyPressed(Keycode::LeftAlt))
		{
			glm::vec2 mousePosition = Input::GetMousePos();
			glm::vec2 delta = (mousePosition - prevMousePosition) * 0.003f;
			prevMousePosition = mousePosition;

			if (Input::IsMouseButtonPressed(Mousecode::ButtonMiddle))
				MousePan(delta);
			else if (Input::IsMouseButtonPressed(Mousecode::ButtonLeft))
				MouseRotate(delta);
			else if (Input::IsMouseButtonPressed(Mousecode::ButtonRight))
				MouseZoom(delta.y);
		}

		RecalculateView();
	}

	void EditorCamera::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<MouseScrollEvent>(GBC_BIND_FUNC(OnMouseScrollEvent));
	}

	bool EditorCamera::OnMouseScrollEvent(MouseScrollEvent& event)
	{
		float delta = event.GetOffsetY() * 0.1f;
		MouseZoom(delta);
		RecalculateView();
		return false;
	}

	void EditorCamera::OnViewportResize(int width, int height)
	{
		viewportSize.x = (float)width;
		viewportSize.y = (float)height;
		RecalculateProjection();
	}

	void EditorCamera::RecalculateProjection()
	{
		aspectRatio = viewportSize.x / viewportSize.y;
		projection = glm::perspective(fov, aspectRatio, nearClip, farClip);
	}

	void EditorCamera::RecalculateView()
	{
		position = focalPoint - GetForwardDirection() * distance;

		glm::quat orientation = GetOrientation();
		view = glm::inverse(glm::translate(glm::mat4(1.0f), position) * glm::toMat4(orientation));
	}

	void EditorCamera::MousePan(const glm::vec2& delta)
	{
		glm::vec2 panSpeed = GetPanSpeed();
		focalPoint -= GetRightDirection() * delta.x * panSpeed.x * distance;
		focalPoint += GetUpDirection() * delta.y * panSpeed.y * distance;
	}

	void EditorCamera::MouseRotate(const glm::vec2& delta)
	{
		float yawSign = GetUpDirection().y < 0.0f ? -1.0f : 1.0f;
		yaw += yawSign * delta.x * GetRotationSpeed();
		pitch += delta.y * GetRotationSpeed();
	}

	void EditorCamera::MouseZoom(float delta)
	{
		distance -= delta * GetZoomSpeed();
		if (distance < 1.0f)
		{
			distance = 1.0f;
			focalPoint += GetForwardDirection();
		}
	}

	glm::vec2 EditorCamera::GetPanSpeed() const
	{
		float x = std::min(viewportSize.x / 1000.0f, 2.4f);
		float xFactor = 0.0366f * x * x - 0.1778f * x + 0.3021f;

		float y = std::min(viewportSize.x / 1000.0f, 2.4f);
		float yFactor = 0.0366f * y * y - 0.1778f * y + 0.3021f;

		return {xFactor, yFactor};
	}

	float EditorCamera::GetRotationSpeed() const
	{
		return 0.8f;
	}

	float EditorCamera::GetZoomSpeed() const
	{
		float dist = std::max(distance * 0.2f, 0.0f);
		return std::min(dist * dist, 100.0f);
	}

	glm::vec3 EditorCamera::GetRightDirection() const
	{
		return glm::rotate(GetOrientation(), glm::vec3(1.0f, 0.0f, 0.0f));
	}

	glm::vec3 EditorCamera::GetUpDirection() const
	{
		return glm::rotate(GetOrientation(), glm::vec3(0.0f, 1.0f, 0.0f));
	}

	glm::vec3 EditorCamera::GetForwardDirection() const
	{
		return glm::rotate(GetOrientation(), glm::vec3(0.0f, 0.0f, -1.0f));
	}

	glm::quat EditorCamera::GetOrientation() const
	{
		return glm::quat(glm::vec3(-pitch, -yaw, 0.0f));
	}
}
