#include "Renderer.h"
#include "Window.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Texture.h"
#include "glm/glm/glm.hpp"
#include "glm/glm/gtc/matrix_transform.hpp"
#include "Camera/Camera.h"


static int width = 960, height = 720;
Camera camera(width, height);

void OnCursorPosChange(GLFWwindow* window, double xpos, double ypos)
{
    camera.OnCursorPosChange(xpos, ypos);
}

void OnKeyStateChange(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    camera.OnKeyStateChange(key, scancode, action, mods);
}


int main()
{
	Window& window = Window::GetWindow();
	window.Init(width, height, "OpenGL Window");


    //-----------------------cube---------------------------
    // 坐标 纹理 法线
    float vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f,  0.0f, -1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,  0.0f, -1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  0.0f, -1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f,  0.0f, -1.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f,  0.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, -1.0f, 0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, -1.0f, 0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, -1.0f, 0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, -1.0f, 0.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, -1.0f, 0.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f, 0.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f, -1.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 0.0f, -1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f, -1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f, -1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f, -1.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f, -1.0f, 0.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
    };
    VertexArray va;
    VertexBuffer vb(vertices, 36 * 8 * sizeof(float));
    VertexBufferLayout vbl(
        {
            {GL_FLOAT, 3, false, 0},
            {GL_FLOAT, 2, false, 0},
            {GL_FLOAT, 3, false, 0}
        }
    );
    va.AddBuffer(vb, vbl);
    unsigned int indices[] = {
        0, 1, 2,
        3, 4, 5,
        6, 7, 8,
        9, 10, 11,
        12, 13, 14,
        15, 16, 17,
        18, 19, 20,
        21, 22, 23,
        24, 25, 26,
        27, 28, 29,
        30, 31, 32,
        33, 34, 35
    };
    IndexBuffer ib(indices, 36);

    Shader shader("res/Shaders/cube.shader");
    shader.Bind();
    Texture texture("res/Textures/letter_p.png");
    texture.Bind();

    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 100.0f);
    glm::mat4 model = glm::mat4(1.0f);

    //-----------------------cube---------------------------

    //-----------------------light--------------------------
    glm::vec3 light_pos = { 0.0f, 30.0f, -80.f };
    glm::vec4 light_color = { 1.0f, 1.0f, 1.0f, 1.0f };
    VertexArray light_va;
    VertexBuffer light_vb(vertices, 36 * 5 * sizeof(float));
    VertexBufferLayout light_vbl(
        {
            {GL_FLOAT, 3, false, 0},
            {GL_FLOAT, 2, false, 0},
            {GL_FLOAT, 3, false, 0}
        }
    );
    light_va.AddBuffer(light_vb, light_vbl);
    IndexBuffer light_ib(indices, 36);
    Shader light_shader("res/Shaders/light.shader");
    //-----------------------light--------------------------

    glm::mat4 mvp;

    Renderer& renderer = Renderer::GetRenderer();

    glfwSetCursorPosCallback(window.GetGLWindow(), OnCursorPosChange);
    glfwSetKeyCallback(window.GetGLWindow(), OnKeyStateChange);

    glfwSetInputMode(window.GetGLWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    GLCALL(glEnable(GL_DEPTH_TEST));

	while (!window.ShouldCloseWindow())
	{
		renderer.Clear();
        // cube
        texture.Bind();
        shader.Bind();
        model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 1.0f, -10.0f)) * glm::rotate(glm::mat4(1.0f), (float)(glfwGetTime() * glm::radians(45.0)), glm::vec3(1.0f, 0.0f, 0.0f));
        mvp = projection * camera.GetViewMatrix() * model;
        glm::vec3 camera_pos = camera.GetPosition();
        shader.SetUniformMat4f("u_MVP", mvp);
        shader.SetUniformMat4f("u_Model", model);
        shader.SetUniform3f("u_LightColor", light_color.x, light_color.y, light_color.z);
        shader.SetUniform3f("u_LightPosition", light_pos.x, light_pos.y, light_pos.z);
        shader.SetUniform3f("u_CameraPos", camera_pos.x, camera_pos.y, camera_pos.z);
        shader.SetUniform1i("u_Texture", 0);

        renderer.Draw(va, ib, shader);
        //light
        light_shader.Bind();
        model = glm::translate(glm::mat4(1.0f), light_pos);
        mvp = projection * camera.GetViewMatrix() * model;
        light_shader.SetUniformMat4f("u_MVP", mvp);
        light_shader.SetUniform3f("u_LightColor", light_color.x, light_color.y, light_color.z);
        renderer.Draw(light_va, light_ib, light_shader);
		window.Update();
	}

	window.Close();

	return 0;
}