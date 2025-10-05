
#include "stdafx.h"
#include "Transform.h"

#include <glm/common.hpp>


namespace CautionEngine::Rendering
{
	Transform::Transform()
		: m_matrix(glm::identity<glm::mat4x4>())
		, m_position(0.0f)
		, m_rotation()
		, m_scale(1.0f)
	{
		
	}

	Transform::Transform(const glm::vec3& position, const glm::quat& rotation, const glm::vec3& scale)
		: m_position(position)
		, m_rotation(rotation)
		, m_scale(scale)
		, m_matrix(1.0f)
	{
		UpdateMatrix();
	}

	Transform::Transform(const glm::vec3& position, const glm::vec3& rotationEuler, const glm::vec3& scale)
		: m_position(position)
		, m_rotation(rotationEuler)
		, m_scale(scale)
		, m_matrix(1.0f)
	{
		UpdateMatrix();
	}

	void Transform::SetPosition(const glm::vec3& position)
	{
		m_position = position;
		UpdateMatrix();
	}

	void Transform::SetScale(const glm::vec3& scale)
	{
		m_scale = scale;
		UpdateMatrix();
	}

	void Transform::SetRotation(const glm::quat& rotation)
	{
		m_rotation = rotation;
		UpdateMatrix();
	}

	void Transform::SetRotationEuler(const glm::vec3& angles)
	{
		m_rotation = glm::quat(angles);
		UpdateMatrix();
	}

	void Transform::UpdateMatrix()
	{
		m_matrix = glm::scale(glm::identity<glm::mat4x4>(), m_scale);
		m_matrix = glm::mat4_cast(m_rotation) * m_matrix;
		m_matrix = glm::translate(m_matrix, m_position);
	}
}