#pragma once

#include <CautionEngineDefinitions.h>

namespace CautionEngine::Rendering
{
	class Transform;

	class Camera
	{
	private:
		glm::mat4x4 m_projection;

		float m_fovY;
		float m_aspect;
		float m_nearPlane;
		float m_farPlane;

		void UpdateProjection();

	public:
		Camera();
		Camera(float fovY, float aspect, float nearPlane, float farPlane);

		const glm::mat4x4& GetMatrix() const { return m_projection; }

		float GetFOV() const { return m_fovY; }
		void SetFOV(float fov);

		void UpdateAspectRatio(float aspect);

		float GetNearPlane() const { return m_nearPlane; }
		void SetNearPlane(float nearPlane);

		float GetFarPlane() const { return m_farPlane; }
		void SetFarPlane(float farPlane);

		float GetAspectRatio() const { return m_aspect; }
		void SetAspectRatio(float ratio);

		void SetProjectionParams(float fov, float aspect, float nearPlane, float farPlane);

		glm::mat4x4 CreateViewProjection(const Transform& cameraTransform) const;
		glm::mat4x4 CreateViewMatrix(const Transform& cameraTransform) const;
	};
}