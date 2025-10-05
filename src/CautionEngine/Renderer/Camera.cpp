
#include "stdafx.h"
#include "Camera.h"
#include "Transform.h"

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/quaternion_common.hpp>

namespace CautionEngine::Rendering
{
	Camera::Camera()
		: m_fovY(90.0f)
		, m_aspect(1.0f)
		, m_nearPlane(0.1f)
		, m_farPlane(100.0f)
		, m_projection(glm::perspective(90.0f, 1.0f, 0.1f, 100.0f))
	{
	}

	Camera::Camera(float fovY, float aspect, float nearPlane, float farPlane)
		: m_fovY(fovY)
		, m_aspect(aspect)
		, m_nearPlane(nearPlane)
		, m_farPlane(farPlane)
		, m_projection(glm::perspective(fovY, aspect, nearPlane, farPlane))
	{
		
	}

	void Camera::SetFOV(float fov)
	{
		m_fovY = fov;
		UpdateProjection();
	}

	void Camera::UpdateAspectRatio(float aspect)
	{
		m_aspect = aspect;
		UpdateProjection();
	}

	void Camera::SetNearPlane(float nearPlane)
	{
		m_nearPlane = nearPlane;
		UpdateProjection();
	}

	void Camera::SetFarPlane(float farPlane)
	{
		m_farPlane = farPlane;
		UpdateProjection();
	}

	void Camera::SetAspectRatio(float ratio)
	{
		m_aspect = ratio;
		UpdateProjection();
	}

	void Camera::SetProjectionParams(float fov, float aspect, float nearPlane, float farPlane)
	{
		m_fovY = fov;
		m_aspect = aspect;
		m_nearPlane = nearPlane;
		m_farPlane = farPlane;
		UpdateProjection();
	}

	glm::mat4x4 Camera::CreateViewProjection(const Transform& cameraTransform) const
	{
		return m_projection * CreateViewMatrix(cameraTransform);
	}

	glm::mat4x4 Camera::CreateViewMatrix(const Transform& cameraTransform) const
	{
		glm::vec3 inversePosition = -cameraTransform.GetPosition();
		glm::mat4x4 inverseRotation = glm::mat4_cast(glm::inverse(cameraTransform.GetRotation()));
		return inverseRotation * glm::translate(glm::identity<glm::mat4x4>(), inversePosition);
	}

	void Camera::UpdateProjection()
	{
		m_projection = glm::perspective(m_fovY, m_aspect, m_nearPlane, m_farPlane);
	}
}