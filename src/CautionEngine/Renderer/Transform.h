#pragma once

#include <glm/gtc/quaternion.hpp>

namespace CautionEngine::Rendering
{
	class Transform
	{
	private:
		glm::mat4x4 m_matrix;

		glm::vec3 m_position;
		glm::quat m_rotation;
		glm::vec3 m_scale;

		void UpdateMatrix();

	public:
		Transform();
		Transform(const glm::vec3& position, const glm::quat& rotation, const glm::vec3& scale);
		Transform(const glm::vec3& position, const glm::vec3& rotationEuler, const glm::vec3& scale);

		void SetPosition(const glm::vec3& position);
		glm::vec3 GetPosition() const { return m_position; }

		void SetScale(const glm::vec3& scale);
		glm::vec3 GetScale() const { return m_scale; }

		void SetRotation(const glm::quat& rotation);
		void SetRotationEuler(const glm::vec3& angles);
		glm::quat GetRotation() const { return m_rotation; }

		const glm::mat4x4& GetMatrix() const { return m_matrix; }

	};
}