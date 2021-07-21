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
		EditorCamera(float fov, float nearClip, float farClip);

		void OnUpdate(Timestep timestep);
		void OnEvent(Event& event);

		inline float GetDistance() const noexcept { return distance; }
		inline void SetDistance(float distance) noexcept { this->distance = distance; }

		virtual void OnViewportResize(int width, int height) override;

		inline const glm::mat4& GetView() const noexcept { return view; }
		inline glm::mat4 GetViewProjection() const noexcept { return projection * view; }

		glm::vec3 GetRightDirection() const;
		glm::vec3 GetUpDirection() const;
		glm::vec3 GetForwardDirection() const;
		inline const glm::vec3& GetPosition() const noexcept { return position; }
		glm::quat GetOrientation() const;

		inline float GetPitch() const noexcept { return pitch; }
		inline float GetYaw() const noexcept { return yaw; }

		bool IsUsing() const;
		inline void SetBlocked(bool blocked) noexcept { this->blocked = blocked; }
	private:
		void RecalculateProjection();
		void RecalculateView();

		bool OnMouseScrollEvent(MouseScrollEvent& event);

		void MousePan(const glm::vec2& delta);
		void MouseRotate(const glm::vec2& delta);
		void MouseZoom(float delta);

		//glm::vec2 GetPanSpeed() const;
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

		bool blocked = false;
	};
}
