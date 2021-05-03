#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <custom_shader.h>

GLfloat first_vertices[] = {
	// Positions			// Colors
	-0.7f, -0.5f, 0.0f,		1.0f, 0.0f, 0.0f,
	 0.3f, -0.5f, 0.0f,		0.0f, 1.0f, 0.0f,
	 -0.2f,  0.5f, 0.0f,	0.0f, 0.0f, 1.0f
};
GLfloat second_vertices[] = {
	0.5f,-0.5f,0.0f,
	0.8f,-0.5f,0.0f,
	0.75f,0.5f,0.0f
};

GLfloat color[] = {
	1.0f,0.5f,3.0f
};

GLfloat vertices[] = {
	// 位置              // 颜色
	 0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // 右下
	-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // 左下
	 0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // 顶部
};

GLFWwindow* window;
const GLuint WIDTH = 800, HEIGHT = 600;
GLuint shaderProgramYellow, shaderProgramRed;
GLuint VBO[2], VAO[2];
GLfloat horizontalOffset = 0.0f;

void InitGLSetting();
void InitObjectVertexFragmentData();
void GameLoop();
void DeleteBufferAndArrays();
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

int main()
{
	InitGLSetting();
	InitObjectVertexFragmentData();
	
	Shader ourRedShader("shaders/primary_shaders/shader.vs", "shaders/primary_shaders/shaderRed.frag");
	Shader ourYellowShader("shaders/primary_shaders/shader.vs", "shaders/primary_shaders/shaderYellow.frag");

	// Game loop
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		ourRedShader.Use();
		// 改变颜色
		GLfloat timeValue = glfwGetTime();
		float greenValue = (sin(timeValue) / 2) + 0.5;

		ourRedShader.SetColor("ourColor", { 0.0f, greenValue, 1.0f, 1.0f });
		ourRedShader.SetFloat("offset", horizontalOffset);

		glBindVertexArray(VAO[0]);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		ourYellowShader.Use();

		ourYellowShader.SetColor("ourColor", { greenValue,0.0f,1.0f,1.0f });
		glBindVertexArray(VAO[1]);
		glDrawArrays(GL_TRIANGLES, 0, 3);

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

void InitObjectVertexFragmentData()
{
	glGenVertexArrays(2, VAO);
	glGenBuffers(2, VBO);

	// 第一个三角形的VAO和VBO
	glBindVertexArray(VAO[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GL_FLOAT), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// 第二个三角形的VAO和VBO
	glBindVertexArray(VAO[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(second_vertices), second_vertices, GL_STATIC_DRAW);
	// new VAO, index start from 1
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void DeleteBufferAndArrays()
{
	glDeleteVertexArrays(2, VAO);
	glDeleteBuffers(2, VBO);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (action == GLFW_PRESS)
	{
		if (key == GLFW_KEY_ESCAPE)
		{
			glfwSetWindowShouldClose(window, GL_TRUE);
		}
		if (key == GLFW_KEY_D)
		{
			horizontalOffset += 0.1f;
		}
		if (key == GLFW_KEY_A)
		{
			horizontalOffset -= 0.1f;
		}
	}
}