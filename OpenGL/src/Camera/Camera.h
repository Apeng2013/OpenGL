#pragma once
#include "glm/glm.hpp"


class Camera
{
public:
	void InitParam(int width, int height);

	static Camera& GetCamera()
	{
		static Camera camera;
		return camera;
	}

	const glm::mat4& GetViewMatrix() const { return m_ViewMat; };
	const glm::vec3& GetPosition() const { return m_Position; };

	void SetPosition(float x, float y, float z);

	void OnCursorPosChange(double xpos, double ypos);
	void OnKeyStateChange(int key, int scancode, int action, int mods);

private:
	float m_Lastx, m_Lasty;
	float m_MouseSensitivity;
	float m_Pitch;
	float m_Yaw;
	bool m_FirstMouseEnter;
	glm::vec3 m_Position;
	glm::vec3 m_ForwardDir;
	glm::vec3 m_UpDir;
	glm::mat4 m_ViewMat;

private:
	Camera();
	~Camera();
	Camera(Camera& other);
	Camera& operator=(Camera& other);

};