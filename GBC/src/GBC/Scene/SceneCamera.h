#pragma once

#include "GBC/Rendering/Camera.h"

namespace gbc
{
	class SceneCamera : public Camera
	{
	public:
		enum class ProjectionType { Perspective, Orthographic };

		void SetPerspective(float fov, float nearClip, float farClip);
		inline float GetPerspectiveFOV() const { return perspectiveFOV; }
		inline void SetPerspectiveFOV(float fov) { perspectiveFOV = fov; Recalculate(); }
		inline float GetPerspectiveNearClip() const { return perspectiveNear; }
		inline void SetPerspectiveNearClip(float nearClip) { perspectiveNear = nearClip; Recalculate(); }
		inline float GetPerspectiveFarClip() const { return perspectiveFar; }
		inline void SetPerspectiveFarClip(float farClip) { perspectiveFar = farClip; Recalculate(); }

		void SetOrthographic(float size, float nearClip, float farClip);
		inline float GetOrthographicSize() const { return orthographicSize; }
		inline void SetOrthographicSize(float size) { orthographicSize = size; Recalculate(); }
		inline float GetOrthographicNearClip() const { return orthographicNear; }
		inline void SetOrthographicNearClip(float nearClip) { orthographicNear = nearClip; Recalculate(); }
		inline float GetOrthographicFarClip() const { return orthographicFar; }
		inline void SetOrthographicFarClip(float farClip) { orthographicFar = farClip; Recalculate(); }

		inline ProjectionType GetProjectionType() const { return projectionType; }
		inline void SetProjectionType(ProjectionType type) { projectionType = type; Recalculate(); }

		void OnViewportResize(int width, int height);
	private:
		void Recalculate();

		ProjectionType projectionType = ProjectionType::Perspective;

		// Perspective stuff
		float perspectiveFOV = glm::radians(90.0f);
		float perspectiveNear = 0.001f;
		float perspectiveFar = 1000.0f;

		// Orthographic stuff
		float orthographicSize = 10.0f;
		float orthographicNear = -1.0f;
		float orthographicFar = 1.0f;

		// Other stuff
		float aspectRatio = 1.0f;
	};
}
