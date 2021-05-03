#include <iostream>
#include <unordered_map>
#include <algorithm>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <custom_shader.h>
#include <SOIL/SOIL.h>
#include <ShaderManager.h>
#include <Camera.h>

#ifndef GLM_H
#define GLM_H
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#endif // !GLM_H

GLfloat vertices[] = {
	-0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f, -0.5f,
	 0.5f,  0.5f, -0.5f,
	 0.5f,  0.5f, -0.5f,
	-0.5f,  0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f,

	-0.5f, -0.5f,  0.5f,
	 0.5f, -0.5f,  0.5f,
	 0.5f,  0.5f,  0.5f,
	 0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,
	-0.5f, -0.5f,  0.5f,

	-0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f,
	-0.5f, -0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,

	 0.5f,  0.5f,  0.5f,
	 0.5f,  0.5f, -0.5f,
	 0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f,  0.5f,
	 0.5f,  0.5f,  0.5f,

	-0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f,  0.5f,
	 0.5f, -0.5f,  0.5f,
	-0.5f, -0.5f,  0.5f,
	-0.5f, -0.5f, -0.5f,

	-0.5f,  0.5f, -0.5f,
	 0.5f,  0.5f, -0.5f,
	 0.5f,  0.5f,  0.5f,
	 0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f, -0.5f
};

// Init camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

GLFWwindow* window;
const GLuint WIDTH = 800, HEIGHT = 600;

GLuint lightVAO, containerVAO;
GLuint VBO;

// 当前帧遇上一帧的时间差   上一帧的时间
GLfloat deltaTime = 0.0f, lastFrame = 0.0f;

// 记录上一次鼠标位置
GLfloat lastX = 400, lastY = 300;

GLboolean firstMouse = true;
bool keys[1024];

void InitGLSetting();
void BindVertexAndDrawArrays();
void DeleteBufferAndArrays();
void ProcessInputUpdateCameraPos();
void do_movement();
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

int main()
{
	InitGLSetting();
	std::string objectVertexShaderPath = "shaders/primary_shaders/shaderLightColor.vs";
	std::string objectFragShaderPath = "shaders/primary_shaders/shaderLightObject.frag";
	std::string lightFragShaderPath = "shaders/primary_shaders/shaderLightColor.frag";
	Shader objectShader((const GLchar*)objectVertexShaderPath.c_str(), (const GLchar*)objectFragShaderPath.c_str());
	Shader lightShader((const GLchar*)objectVertexShaderPath.c_str(), (const GLchar*)lightFragShaderPath.c_str());
	ShaderManager::GetInstance()->AddShader("objectShader", objectShader);
	ShaderManager::GetInstance()->AddShader("lightShader", lightShader);

	GLuint VBO, containerVAO;
	glGenVertexArrays(1, &containerVAO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(containerVAO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);

	GLuint lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);

	// Game loop
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		do_movement();

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		objectShader.Use();
		objectShader.SetColor("objectColor", { 1.0f, 0.5f, 0.31f });
		objectShader.SetColor("lightColor", { 1.0f, 0.5f, 0.31f });

		glm::mat4 view(1.0f);
		view = camera.GetViewMatrix();
		glm::mat4 projection = glm::perspective(glm::radians(camera.zoom), (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);

		objectShader.SetMat4("view", 1, GL_FALSE, view);
		objectShader.SetMat4("projection", 1, GL_FALSE, projection);

		glBindVertexArray(containerVAO);
		glm::mat4 model(1.0f);
		objectShader.SetMat4("model", 1, GL_FALSE, model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		lightShader.Use();

		lightShader.SetMat4("view", 1, GL_FALSE, view);
		lightShader.SetMat4("projection", 1, GL_FALSE, projection);

		glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
		model = glm::mat4(1.0f);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f));
		lightShader.SetMat4("model", 1, GL_FALSE, model);

		glBindVertexArray(lightVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		glfwSwapBuffers(window);
	}

	DeleteBufferAndArrays();

	glfwTerminate();
	return 0;
}

void InitGLSetting()
{
	std::cout << "Starting GLFW context, OpenGL 3.3" << std::endl;

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;
	glewInit();

	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// 隐藏鼠标光标
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);

	std::cout << "width = " << width << "," << "height = " << height << std::endl;

	glEnable(GL_DEPTH_TEST);

	GLint nrAttributes;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
	std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;
}

void DeleteBufferAndArrays()
{
	glDeleteVertexArrays(1, &containerVAO);
	glDeleteVertexArrays(1, &lightVAO);
	glDeleteBuffers(1, &VBO);
}

void do_movement()
{
	GLfloat currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

	if (keys[GLFW_KEY_W])
		camera.KeyBoardProcess(CameraMoveType::FORWARD, deltaTime);
	if (keys[GLFW_KEY_S])
		camera.KeyBoardProcess(CameraMoveType::BACKWARD, deltaTime);
	if (keys[GLFW_KEY_A])
		camera.KeyBoardProcess(CameraMoveType::LEFT, deltaTime);
	if (keys[GLFW_KEY_D])
		camera.KeyBoardProcess(CameraMoveType::RIGHT, deltaTime);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (action == GLFW_PRESS)
	{
		if (key == GLFW_KEY_ESCAPE)
		{
			glfwSetWindowShouldClose(window, GL_TRUE);
		}
		keys[key] = true;
	}
	else if (action == GLFW_RELEASE)
	{
		keys[key] = false;
	}
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse) // 这个bool变量一开始是设定为true的
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	GLfloat xoffset = xpos - lastX;

	// reversed since y-coordinates go from bottom to top
	GLfloat yoffset = lastY - ypos;

	lastX = xpos;
	lastY = ypos;

	camera.MouseProcess(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	// Mouse scroll process
	camera.ScrollProcess(yoffset);
}
