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
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};

glm::vec3 cubePositions[] = {
  glm::vec3(0.0f,  0.0f,  0.0f),
  glm::vec3(2.0f,  5.0f, -15.0f),
  glm::vec3(-1.5f, -2.2f, -2.5f),
  glm::vec3(-3.8f, -2.0f, -12.3f),
  glm::vec3(2.4f, -0.4f, -3.5f),
  glm::vec3(-1.7f,  3.0f, -7.5f),
  glm::vec3(1.3f, -2.0f, -2.5f),
  glm::vec3(1.5f,  2.0f, -2.5f),
  glm::vec3(1.5f,  0.2f, -1.5f),
  glm::vec3(-1.3f,  1.0f, -1.5f)
};

// Init camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

glm::vec3 cameraPos = glm::vec3(0.0f, 1.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

GLFWwindow* window;
const GLuint WIDTH = 800, HEIGHT = 600;
GLuint shaderProgramYellow, shaderProgramRed;
GLuint VAO[2], VBO[2];
GLuint texture[2];
glm::vec3 relativePos(0, 0, -3.0f);

// ????
GLfloat aspect = 45.0f;

GLfloat deltaTime = 0.0f;   // ??????????????????????
GLfloat lastFrame = 0.0f;   // ????????????

// ??????????????????
GLfloat lastX = 400, lastY = 300;
GLfloat pitch = 0, yaw = -90.0f, roll = 0;

GLboolean firstMouse = true;

bool keys[1024];

void InitGLSetting();
void InitTextureData();
void InitObjectVertexFragmentData();
void CalculteVector();
void GameLoop();
void Draw();
void SetObjectTexture();
void SetVertexObjectMVP();
void BindVertexAndDrawArrays();
void DeleteBufferAndArrays();
void UpdateCameraPos();
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

int main()
{
	InitGLSetting();
	InitTextureData();
	InitObjectVertexFragmentData();
	
	CalculteVector();

	Shader ourRedShader("shaders/primary_shaders/shaderCoordinateSystem.vs", "shaders/primary_shaders/shaderCoordinateSystem.frag");
	ShaderManager::GetInstance()->AddShader("ourRedShader", ourRedShader);

	// Game loop
	while (!glfwWindowShouldClose(window))
	{
		GameLoop();
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

	// ????????????
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

void InitTextureData()
{
	int width[2], height[2];
	// third Parameter is Channel numbers
	unsigned char* image = SOIL_load_image("textures/container.jpg", &width[0], &height[0], 0, SOIL_LOAD_RGB);
	unsigned char* image2 = SOIL_load_image("textures/awesomeface.png", &width[1], &height[1], 0, SOIL_LOAD_RGB);

	glGenTextures(2, texture);
	glBindTexture(GL_TEXTURE_2D, texture[0]);

	// Set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Target Level InternalFormat Width Height Border Format types Pixels
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width[0], height[0], 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	// ??????????????????????????????????????????????
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, texture[1]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width[1], height[1], 0, GL_RGB, GL_UNSIGNED_BYTE, image2);
	glGenerateMipmap(GL_TEXTURE_2D);

	SOIL_free_image_data(image);
	SOIL_free_image_data(image2);
	// ???????? ????????
	glBindTexture(GL_TEXTURE_2D, 0);
}

void InitObjectVertexFragmentData()
{
	glGenVertexArrays(2, VAO);
	glGenBuffers(2, VBO);
	glBindVertexArray(VAO[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// vertices
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// color
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	//glEnableVertexAttribArray(1);

	// texcoord
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void CalculteVector()
{
	/*
	
	glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);

	// ??????????
	glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget);

	// ????up????????cameraDirection????????????????????????
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));

	// ????????????????????????????y??
	glm::vec3 cameraUp = glm::normalize(glm::cross(cameraDirection, cameraRight));

	// ????LookAt????????
	glm::mat4 view(1.0f);
	// ??????????????????
	// cameraPos cameraTarget UpVector
	view = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	*/

	// ????direction????????????????????????????????????????????????????
	// pitch ?????? yaw ?????? roll ??????
	glm::vec3 direction(0.0f, 0.0f, 0.0f);

	direction.x = std::cos(glm::radians(pitch)) * std::cos(glm::radians(yaw));
	direction.y = std::sin(glm::radians(pitch));
	direction.z = std::cos(glm::radians(pitch)) * std::sin(glm::radians(yaw));
}


void GameLoop()
{
	glfwPollEvents();

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	Draw();

	glfwSwapBuffers(window);
}

void DeleteBufferAndArrays()
{
	glDeleteVertexArrays(2, VAO);
	glDeleteVertexArrays(2, VBO);
}

void Draw()
{
	SetObjectTexture();
	SetVertexObjectMVP();
	UpdateCameraPos();
	BindVertexAndDrawArrays();
}

void SetObjectTexture()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	ShaderManager::GetInstance()->GetShaderByName("ourRedShader")->SetInt("ourTexture", 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture[1]);
	ShaderManager::GetInstance()->GetShaderByName("ourRedShader")->SetInt("ourTexture2", 1);

	GLfloat timeValue = glfwGetTime();
	float alphaValue = (sin(timeValue) / 2) + 0.5;

	ShaderManager::GetInstance()->GetShaderByName("ourRedShader")->SetFloat("mixDegree", alphaValue);
	ShaderManager::GetInstance()->UseShaderByName("ourRedShader");
}

void SetVertexObjectMVP()
{
	glm::mat4 model(1.0f);
	glm::mat4 view(1.0f);
	glm::mat4 projection(1.0f);

	glm::ortho(0.0f, 800.0f, 0.0f, 600.0f, 0.1f, 100.0f);

	//model = glm::rotate(model, (GLfloat)glfwGetTime() * 2.0f, glm::vec3(0.5f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(0.5f, 1.0f, 0.0f));

	// ??????????????????????????????????????????????
	//view = glm::translate(view, glm::vec3(relativePos));

	//GLfloat radius = 10.0f;
	//GLfloat camX = std::sin(glfwGetTime()) * radius;
	//GLfloat camZ = std::cos(glfwGetTime()) * radius;

	// ????LookAt????
	//view = glm::lookAt(glm::vec3(camX, 0.0f, camZ), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	view = glm::lookAt(cameraPos, cameraFront + cameraPos, cameraUp);

	// ????????????
	projection = glm::perspective(glm::radians(aspect), (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);

	ShaderManager::GetInstance()->GetShaderByName("ourRedShader")->SetMat4("model", 1, GL_FALSE, model);
	ShaderManager::GetInstance()->GetShaderByName("ourRedShader")->SetMat4("view", 1, GL_FALSE, view);
	ShaderManager::GetInstance()->GetShaderByName("ourRedShader")->SetMat4("projection", 1, GL_FALSE, projection);
}

void BindVertexAndDrawArrays()
{
	glBindVertexArray(VAO[0]);
	for (GLuint i = 0; i < 10; i++)
	{
		glm::mat4 model(1.0f);
		model = glm::translate(model, cubePositions[i]);
		GLfloat angle = glm::radians(55.0f);
		if (i % 3 == 0)
		{
			angle = glfwGetTime() * std::min(5.0f * i, 10.0f);
		}
		model = glm::rotate(model, angle, glm::vec3(1.0f, 0.3f, 0.5f));
		ShaderManager::GetInstance()->GetShaderByName("ourRedShader")->SetMat4("model", 1, GL_FALSE, model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
	glBindVertexArray(0);
}
void UpdateCameraPos()
{
	GLfloat currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

	GLfloat cameraSpeed = 5.0f * deltaTime;
	if (keys[GLFW_KEY_W])
	{
		cameraPos += cameraSpeed * cameraFront;
	}
	if (keys[GLFW_KEY_S])
	{
		cameraPos -= cameraSpeed * cameraFront;
	}
	if (keys[GLFW_KEY_A])
	{
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	}
	if (keys[GLFW_KEY_D])
	{
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	}

	cameraPos.y = 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	GLfloat cameraSpeed = 0.05f;
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
	if (firstMouse) // ????bool??????????????????true??
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	GLfloat xoffset = xpos - lastX;
	// y??????????????????????
	GLfloat yoffset = lastY - ypos;

	lastX = xpos;
	lastY = ypos;

	GLfloat sensitivity = 0.05f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	// ????????????????????90??
	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	// ??????????????????????????????????
	glm::vec3 front(0.0f, 0.0f, 0.0f);
	front.x = std::cos(glm::radians(pitch)) * std::cos(glm::radians(yaw));
	front.y = std::sin(glm::radians(pitch));
	front.z = std::cos(glm::radians(pitch)) * std::sin(glm::radians(yaw));

	cameraFront = glm::normalize(front);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	// ?????? yoffset = 1 , ???? yoffset = -1
	//std::cout << xoffset << "," << yoffset << "\n";

	if (aspect >= 1.0f && aspect <= 45.0f)
		aspect -= yoffset;
	if (aspect <= 1.0f)
		aspect = 1.0f;
	if (aspect >= 45.0f)
		aspect = 45.0f;
}
