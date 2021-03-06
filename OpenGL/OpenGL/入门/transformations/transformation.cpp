#include <iostream>
#include <unordered_map>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <custom_shader.h>
#include <SOIL/SOIL.h>
#include <ShaderManager.h>

#ifndef GLM_H
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#endif // !GLM_H

GLfloat texCoords[] = {
	0.0f, 0.0f,	// 左下角
	1.0f, 0.0f,	// 右下角
	0.5f, 1.0f	// 上中
};

GLfloat vertices[] = {
	//     ---- 位置 ----       ---- 颜色 ----     - 纹理坐标 -
		 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // 右上
		 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // 右下
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // 左下
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // 左上
};

GLuint indices[] = {  // Note that we start from 0!
		0, 1, 3, // First Triangle
		1, 2, 3, // Second Triangle
};

GLFWwindow* window;
const GLuint WIDTH = 800, HEIGHT = 600;
GLuint shaderProgramYellow, shaderProgramRed;
GLuint VAO[2], VBO[2], EBO;
GLuint texture[2];

void InitGLSetting();
void InitTextureData();
void InitObjectVertexFragmentData();
void CaclulateVector();
void GameLoop();
void DeleteBufferAndArrays();
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

int main()
{
	InitGLSetting();
	InitTextureData();
	InitObjectVertexFragmentData();

	Shader ourRedShader("shaders/primary_shaders/shaderTexture.vs", "shaders/primary_shaders/shaderTextureFrag.frag");
	CaclulateVector();

	// Game loop
	while (!glfwWindowShouldClose(window))
	{
		//GameLoop();
		glfwPollEvents();

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture[0]);
		ourRedShader.SetInt("ourTexture", 0);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture[1]);
		ourRedShader.SetInt("ourTexture2", 1);

		GLfloat timeValue = glfwGetTime();
		float alphaValue = (sin(timeValue) / 2) + 0.5;
		ourRedShader.SetFloat("mixDegree", alphaValue);

		ourRedShader.Use();

		// Calculate rotate 90 angle and scale in half
		glm::mat4 trans(1.0f);
		// 实际的变换顺序应该与阅读顺序相反
		trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
		trans = glm::rotate(trans, (GLfloat)glfwGetTime() * 2.0f, glm::vec3(0.0f, 0.0f, 1.0f));

		ourRedShader.SetMat4("transform", 1, GL_FALSE, trans);
		
		glBindVertexArray(VAO[0]);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		glm::mat4 trans2(1.0f);
		trans2 = glm::translate(trans2, glm::vec3(-0.5f, 0.5f, 0.0f));
		GLfloat scaleValue = sin(glfwGetTime()) / 2 + 0.5f;
		trans2 = glm::scale(trans2, glm::vec3(scaleValue, scaleValue, 0.0f));

		ourRedShader.SetMat4("transform", 1, GL_FALSE, trans2);

		glBindVertexArray(VAO[0]);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
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

	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);

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

	// 生成纹理过程
	glGenTextures(2, texture);
	// 绑定纹理
	glBindTexture(GL_TEXTURE_2D, texture[0]);

	// Set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Target Level InternalFormat Width Height Border Format types Pixels
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width[0], height[0], 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	// 为当前绑定的纹理自动生成所有需要的多级渐远纹理
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
	// 取消绑定 释放纹理
	glBindTexture(GL_TEXTURE_2D, 0);
}

void InitObjectVertexFragmentData()
{
	glGenVertexArrays(2, VAO);
	glGenBuffers(2, VBO);
	glGenBuffers(1, &EBO);
	glBindVertexArray(VAO[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// vertices
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// texcoord
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void GameLoop()
{
}

void DeleteBufferAndArrays()
{
	glDeleteVertexArrays(2, VAO);
	glDeleteVertexArrays(2, VBO);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (action == GLFW_PRESS)
	{
		if (key == GLFW_KEY_ESCAPE)
		{
			glfwSetWindowShouldClose(window, GL_TRUE);
		}
	}
}

void CaclulateVector()
{
	//glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);
	//// 单位矩阵
	//glm::mat4 identityMat(1.0f);
	//glm::mat4 trans = glm::translate(identityMat, glm::vec3(1.0f, 1.0f, 0.0f));
	//vec = trans * vec;
	//std::cout << vec.x << vec.y << vec.z << std::endl;

	//trans = glm::rotate(identityMat, 90.0f, glm::vec3(0.0f, 0.0f, 1.0f));
	//trans = glm::scale(trans, glm::vec3(0.5f, 0.5f, 0.5f));
}
