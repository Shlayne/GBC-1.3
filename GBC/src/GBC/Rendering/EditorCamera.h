#pragma once

#include "Camera.h"
#include "GBC/Core/Timestep.h"
#include "GBC/Events/KeyEvents.h"
#include "GBC/Events/MouseEvents.h"

namespace gbc
{
	class EditorCamera : public Camera
	{
	public:
		EditorCamera();
		EditorCamera(float size, float nearClip, float farClip);

		void OnUpdate(Timestep timestep);
		void OnEvent(Event& event);

		void OnViewportMove(int32_t x, int32_t y);
		virtual void OnViewportResize(int32_t width, int32_t height) override;

		inline const glm::mat4& GetView() const noexcept { return view; }
		inline glm::mat4 GetViewProjection() const noexcept { return projection * view; }

		inline const glm::vec2& GetPosition() const noexcept { return position; }

		bool IsUsing() const;
		inline void SetBlocked(bool blocked) noexcept { this->blocked = blocked; }
	private:
		void RecalculateProjection();
		void RecalculateView();
		bool recalculateProjection = false;
		bool recalculateView = false;

		bool OnKeyPressEvent(KeyPressEvent& event);
		bool OnKeyReleaseEvent(KeyReleaseEvent& event);
		bool OnMouseButtonPressEvent(MouseButtonPressEvent& event);
		bool OnMouseButtonReleaseEvent(MouseButtonReleaseEvent& event);
		bool OnMouseMoveEvent(MouseMoveEvent& event);
		bool OnMouseScrollEvent(MouseScrollEvent& event);

		void MousePan();
		void MouseZoom();

		glm::vec2 PixelSpaceToWorldSpace(const glm::vec2& pixelSpace) const;

		float zoomSpeed = 10.0f;
		glm::vec2 startPanMousePosition{ 0.0f };
		glm::vec2 startPanPosition{ 0.0f };

		float size = 10.0f;
		float nearClip = -1.0f;
		float farClip = 1.0f;

		float aspectRatio = 1.0f;
		glm::vec2 viewportSize{ 1.0f };
		glm::vec2 viewportPosition{ 0.0f };

		glm::mat4 view{ 1.0f };
		glm::vec2 position{ 0.0f };
		
		glm::vec2 mousePosition{ 0.0f };
		float mouseScrollOffset = 0.0f;

		Keycode activatorKey = Keycode::LeftShift;
		bool activatorKeyPressed = false;
		MouseButton panButton = MouseButton::ButtonMiddle;
		bool panButtonPressed = false;

		bool blocked = false;
	};
}
