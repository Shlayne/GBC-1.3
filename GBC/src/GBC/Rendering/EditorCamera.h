#pragma once

#include "Camera.h"
#include "GBC/Core/Timestep.h"
#include "GBC/Events/MouseEvents.h"

namespace gbc
{
	class EditorCamera : public Camera
	{
	public:
		EditorCamera() = default;
		EditorCamera(float fov, float aspectRatio, float nearClip, float farClip);

		void OnUpdate(Timestep timestep);
		void OnEvent(Event& event);

		inline float GetDistance() const { return distance; }
		inline void SetDistance(float distance) { this->distance = distance; }

		void OnViewportResize(int width, int height);

		const glm::mat4& GetView() const { return view; }
		glm::mat4 GetViewProjection() const { return projection * view; }

		glm::vec3 GetRightDirection() const;
		glm::vec3 GetUpDirection() const;
		glm::vec3 GetForwardDirection() const;
		const glm::vec3& GetPosition() const { return position; }
		glm::quat GetOrientation() const;

		float GetPitch() const { return pitch; }
		float GetYaw() const { return yaw; }
	private:
		void RecalculateProjection();
		void RecalculateView();

		bool OnMouseScrollEvent(MouseScrollEvent& event);

		void MousePan(const glm::vec2& delta);
		void MouseRotate(const glm::vec2& delta);
		void MouseZoom(float delta);

		glm::vec2 GetPanSpeed() const;
		float GetRotationSpeed() const;
		float GetZoomSpeed() const;

		float fov = 90.0f;
		float aspectRatio = 1.0f;
		float nearClip = 0.001f;
		float farClip = 1000.0f;

		glm::mat4 view{1.0f};
		glm::vec3 position{0.0f};
		glm::vec3 focalPoint{0.0f};
		
		glm::vec2 prevMousePosition{0.0f};
		glm::vec2 viewportSize{1.0f};

		float distance = 5.0f;
		float pitch = 0.0f;
		float yaw = 0.0f;
	};
}
