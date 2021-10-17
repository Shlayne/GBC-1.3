#include "gbcpch.h"
#include "EditorCamera.h"
#include "GBC/Core/Application.h"
#include "GBC/Core/Input.h"

namespace gbc
{
	EditorCamera::EditorCamera()
	{
		RecalculateView();
	}

	EditorCamera::EditorCamera(float fov, float nearClip, float farClip)
		: fov(fov), nearClip(nearClip), farClip(farClip)
	{
		RecalculateView();
	}

	bool EditorCamera::IsUsing() const
	{
		return !blocked && activatorKeyPressed;
	}

	void EditorCamera::OnUpdate(Timestep timestep)
	{
		// Always update mouse position regardless if the camera is in use
		glm::vec2 delta = (mousePosition - prevMousePosition) * 0.003f;
		prevMousePosition = mousePosition;

		if (timestep && IsUsing())
		{
			if (middleMouseButtonPressed)
				MousePan(delta);
			else if (leftMouseButtonPressed)
				MouseRotate(delta);
			else if (rightMouseButtonPressed)
				MouseZoom(delta.y);

			RecalculateView();
		}
	}

	void EditorCamera::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch(this, &EditorCamera::OnKeyPressEvent);
		dispatcher.Dispatch(this, &EditorCamera::OnKeyReleaseEvent);
		dispatcher.Dispatch(this, &EditorCamera::OnMouseButtonPressEvent);
		dispatcher.Dispatch(this, &EditorCamera::OnMouseButtonReleaseEvent);
		dispatcher.Dispatch(this, &EditorCamera::OnMouseMoveEvent);
		dispatcher.Dispatch(this, &EditorCamera::OnMouseScrollEvent);
	}

	void EditorCamera::OnKeyEvent(Keycode keycode, bool pressed)
	{
		if (keycode == activatorKey)
			activatorKeyPressed = pressed;
	}

	void EditorCamera::OnMouseButtonEvent(MouseButton button, bool pressed)
	{
		switch (button)
		{
			case MouseButton::ButtonMiddle: middleMouseButtonPressed = pressed; break;
			case MouseButton::ButtonLeft:   leftMouseButtonPressed = pressed;   break;
			case MouseButton::ButtonRight:  rightMouseButtonPressed = pressed;  break;
		}
	}

	bool EditorCamera::OnKeyPressEvent(KeyPressEvent& event)
	{
		OnKeyEvent(event.GetKeycode(), true);
		return false;
	}

	bool EditorCamera::OnKeyReleaseEvent(KeyReleaseEvent& event)
	{
		OnKeyEvent(event.GetKeycode(), false);
		return false;
	}

	bool EditorCamera::OnMouseButtonPressEvent(MouseButtonPressEvent& event)
	{
		OnMouseButtonEvent(event.GetButton(), true);
		return false;
	}

	bool EditorCamera::OnMouseButtonReleaseEvent(MouseButtonReleaseEvent& event)
	{
		OnMouseButtonEvent(event.GetButton(), false);
		return false;
	}

	bool EditorCamera::OnMouseMoveEvent(MouseMoveEvent& event)
	{
		mousePosition.x = event.GetX();
		mousePosition.y = event.GetY();
		return false;
	}

	bool EditorCamera::OnMouseScrollEvent(MouseScrollEvent& event)
	{
		if (IsUsing() && hovered)
		{
			MouseZoom(event.GetOffsetY() * 0.1f);
			RecalculateView();
		}
		return false;
	}

	void EditorCamera::OnViewportResize(int32_t width, int32_t height)
	{
		viewportSize.x = static_cast<float>(width);
		viewportSize.y = static_cast<float>(height);
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
		view = glm::inverse(glm::translate(glm::mat4(1.0f), position) * glm::toMat4(GetOrientation()));
	}

	void EditorCamera::MousePan(const glm::vec2& delta)
	{
		focalPoint += (GetUpDirection() * delta.y - GetRightDirection() * delta.x) * distance;
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

	//glm::vec2 EditorCamera::GetPanSpeed() const
	//{
	//	float x = std::min(viewportSize.x / 1000.0f, 2.4f);
	//	float xFactor = 0.0366f * x * x - 0.1778f * x + 0.3021f;

	//	float y = std::min(viewportSize.x / 1000.0f, 2.4f);
	//	float yFactor = 0.0366f * y * y - 0.1778f * y + 0.3021f;

	//	return {xFactor, yFactor};
	//}

	float EditorCamera::GetRotationSpeed() const
	{
		return 0.8f;
	}

	float EditorCamera::GetZoomSpeed() const
	{
		float dist = std::max(distance * 0.4f, 0.0f);
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
