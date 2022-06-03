#include "Camera.h"
#include "glm/gtc/matrix_transform.hpp"
#include "GLFW/glfw3.h"

Camera::Camera(int width, int height)
	:m_Lastx(width / 2), m_Lasty(height / 2), m_MouseSensitivity(0.1f), m_Pitch(0.0f), m_Yaw(90.0f), m_FirstMouseEnter(true),
	m_Position{ 0.0f, 0.0f, 0.0f }, m_ForwardDir{ 0.0f, 0.0f, 1.0f }, m_UpDir{ 0.0f, 1.0f, 0.0f }, m_ViewMat(glm::mat4(1.0f))
{
	m_ViewMat = glm::lookAt(m_Position, m_Position - m_ForwardDir, m_UpDir);
}

Camera::~Camera()
{
}

void Camera::SetPosition(float x, float y, float z)
{
    m_Position = { x, y, z };
    m_ViewMat = glm::lookAt(m_Position, m_Position - m_ForwardDir, m_UpDir);
}

void Camera::OnCursorPosChange(double xpos, double ypos)
{
    if (m_FirstMouseEnter)
    {
        m_Lastx = xpos;
        m_Lasty = ypos;
        m_FirstMouseEnter = false;
    }
    float offsetx = (xpos - m_Lastx) * m_MouseSensitivity;
    float offsety = (ypos - m_Lasty) * m_MouseSensitivity;
    m_Lastx = xpos;
    m_Lasty = ypos;

    m_Pitch += offsety;
    if (m_Pitch > 89.0f)
        m_Pitch = 89.0f;
    if (m_Pitch < -89.0f)
        m_Pitch = -89.0f;
    m_Yaw += offsetx;

    glm::vec3 direction;
    direction.x = cos(glm::radians(m_Pitch)) * cos(glm::radians(m_Yaw));
    direction.y = sin(glm::radians(m_Pitch));
    direction.z = cos(glm::radians(m_Pitch)) * sin(glm::radians(m_Yaw));

    m_ForwardDir = glm::normalize(direction);
    glm::vec3 right_dir = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), m_ForwardDir));
    m_UpDir = glm::normalize(glm::cross(m_ForwardDir, right_dir));
    m_ViewMat = glm::lookAt(m_Position, m_Position - m_ForwardDir, m_UpDir);
}

void Camera::OnKeyStateChange(int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS || action == GLFW_REPEAT)
    {
        if (key == GLFW_KEY_W)
        {
            m_Position = m_Position - m_MouseSensitivity * m_ForwardDir;
        }
        else if (key == GLFW_KEY_S)
        {
            m_Position = m_Position + m_MouseSensitivity * m_ForwardDir;
        }
        else if (key == GLFW_KEY_A)
        {
            glm::vec3 right_dir = glm::normalize(glm::cross(m_UpDir, m_ForwardDir));
            m_Position = m_Position - m_MouseSensitivity * right_dir;
        }
        else if (key == GLFW_KEY_D)
        {
            glm::vec3 right_dir = glm::normalize(glm::cross(m_UpDir, m_ForwardDir));
            m_Position = m_Position + m_MouseSensitivity * right_dir;
        }
        m_ViewMat = glm::lookAt(m_Position, m_Position - m_ForwardDir, m_UpDir);
    }
}
