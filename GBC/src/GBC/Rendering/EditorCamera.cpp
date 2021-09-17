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

	EditorCamera::EditorCamera(float size, float nearClip, float farClip)
		: size(size), nearClip(nearClip), farClip(farClip)
	{
		RecalculateView();
	}

	bool EditorCamera::IsUsing() const
	{
		return !blocked && activatorKeyPressed;
	}

	void EditorCamera::OnUpdate(Timestep timestep)
	{
		if (timestep && IsUsing())
		{
			if (Input::IsMouseButtonPressed(panButton))
				MousePan();
			if (mouseScrollOffset != 0.0f)
				MouseZoom();

			if (recalculateView)
				RecalculateView();
			if (recalculateProjection)
				RecalculateProjection();
		}

		mouseScrollOffset = 0.0f;
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

	bool EditorCamera::OnKeyPressEvent(KeyPressEvent& event)
	{
		if (!blocked && event.GetKeycode() == activatorKey)
		{
			activatorKeyPressed = true;
			if (panButtonPressed)
			{
				startPanMousePosition = Input::GetAbsoluteMousePosition() - viewportPosition - viewportSize / 2.0f;
				startPanPosition = position;
			}
		}
		return false;
	}

	bool EditorCamera::OnKeyReleaseEvent(KeyReleaseEvent& event)
	{
		if (!blocked && event.GetKeycode() == activatorKey)
			activatorKeyPressed = false;
		return false;
	}

	bool EditorCamera::OnMouseButtonPressEvent(MouseButtonPressEvent& event)
	{
		if (!blocked && event.GetButton() == panButton)
		{
			panButtonPressed = true;
			if (activatorKeyPressed)
			{
				startPanMousePosition = Input::GetAbsoluteMousePosition() - viewportPosition - viewportSize / 2.0f;
				startPanPosition = position;
			}
		}
		return false;
	}

	bool EditorCamera::OnMouseButtonReleaseEvent(MouseButtonReleaseEvent& event)
	{
		if (!blocked && event.GetButton() == panButton)
			panButtonPressed = false;
		return false;
	}

	bool EditorCamera::OnMouseMoveEvent(MouseMoveEvent& event)
	{
		mousePosition = Input::GetAbsoluteMousePosition() - viewportPosition - viewportSize / 2.0f;
		return false;
	}

	bool EditorCamera::OnMouseScrollEvent(MouseScrollEvent& event)
	{
		mouseScrollOffset = event.GetOffsetY();
		return false;
	}

	void EditorCamera::OnViewportMove(int32_t x, int32_t y)
	{
		viewportPosition = { static_cast<float>(x), static_cast<float>(y) };
	}

	void EditorCamera::OnViewportResize(int32_t width, int32_t height)
	{
		viewportSize = { static_cast<float>(width), static_cast<float>(height) };
		aspectRatio = viewportSize.x / viewportSize.y;
		RecalculateProjection();
	}

	void EditorCamera::RecalculateProjection()
	{
		float orthoTop = size * 0.5f;
		float orthoRight = orthoTop * aspectRatio;
		projection = glm::ortho(-orthoRight, orthoRight, -orthoTop, orthoTop, nearClip, farClip);
		recalculateProjection = false;
	}

	void EditorCamera::RecalculateView()
	{
		view = glm::inverse(glm::translate(glm::mat4(1.0f), glm::vec3(position, 0.0f)));
		recalculateView = false;
	}

	void EditorCamera::MousePan()
	{
		position = startPanPosition - PixelSpaceToWorldSpace(mousePosition - startPanMousePosition);
		recalculateView = true;
	}

	void EditorCamera::MouseZoom()
	{
		glm::vec2 preZoomMousePositionWorldSpace = PixelSpaceToWorldSpace(mousePosition);
		size *= std::pow(1.2f, -mouseScrollOffset);
		position += preZoomMousePositionWorldSpace - PixelSpaceToWorldSpace(mousePosition);

		recalculateProjection = true;
		recalculateView = true;
	}

	// Input  Range: -viewportSize * 0.5f -> viewportSize * 0.5f
	// Output Range: x = -aspectRatio * size -> aspectRatio * size, y = -size -> size
	glm::vec2 EditorCamera::PixelSpaceToWorldSpace(const glm::vec2& pixelSpace) const
	{
		return pixelSpace / viewportSize * glm::vec2(aspectRatio, -1.0f) * size;
	}
}
