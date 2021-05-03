#include <iostream>
#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

using std::cout;
using std::endl;
using std::string;

GLfloat points[] = {
   0.0f,  0.5f,  0.0f,
   0.5f, -0.5f,  0.0f,
  -0.5f, -0.5f,  0.0f
};

GLfloat colours[] = {
  1.0f, 0.0f,  0.0f,
  0.0f, 1.0f,  0.0f,
  0.0f, 0.0f,  1.0f
};

const GLchar* vertexShaderSource =
	"#version 330 core\n"
	"layout (location = 0) in vec3 vertex_position;"
	"layout (location = 1) in vec3 vertex_colour;\n"
	"out vec3 colour; "
	"void main()\n"
	"{\n"
	"colour = vertex_colour;"
	"gl_Position = vec4(vertex_position, 1.0);\n"
	"}\0";

const GLchar* fragmentShaderSource =
	"#version 330 core\n"
	"in vec3 colour;\n"
	"out vec4 color;\n"
	"void main()\n"
	"{\n"
	"color = vec4(colour, 1.0f);\n"
	"}\n\0";

GLFWwindow* window;

const GLuint WIDTH = 800, HEIGHT = 600;
GLuint shaderProgram;
GLuint VBO, VAO, EBO;
GLuint points_vbo = 0, colours_vbo = 0;

void InitGLSetting();
void InitShaderProgram();
void InitObjectVertexFragmentData();
void GameLoop();
void DeleteBufferAndArrays();
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

int main()
{
	InitGLSetting();
	InitShaderProgram();
	InitObjectVertexFragmentData();
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

	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);
}
void InitShaderProgram()
{
	// 创建着色器
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	shaderProgram = glCreateProgram();

	// 检查信息
	GLint successvShader, successfShader, successpLinking;
	GLchar vShaderInfoLog[512], fShaderInfoLog[512], pLinkingInfoLog[512];

	// 源码附加到着色器上
	glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);

	// 编译shader
	glCompileShader(vertexShader);
	glCompileShader(fragmentShader);

	// 检查shader语法
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &successvShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &successfShader);
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &successpLinking);

	if (!successvShader)
	{
		glGetShaderInfoLog(vertexShader, 512, nullptr, vShaderInfoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << vShaderInfoLog << std::endl;
	}
	if (!successfShader)
	{
		glGetShaderInfoLog(fragmentShader, 512, nullptr, fShaderInfoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << fShaderInfoLog << std::endl;
	}
	if (!successpLinking)
	{
		glGetProgramInfoLog(shaderProgram, 512, nullptr, pLinkingInfoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << pLinkingInfoLog << std::endl;
	}
	// 链接shader
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);

	// insert location binding code here
	glBindAttribLocation(shaderProgram, 0, "vertex_position");
	glBindAttribLocation(shaderProgram, 1, "vertex_colour");

	glLinkProgram(shaderProgram);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void InitObjectVertexFragmentData()
{
	// VBO vertex buffer Object
	// VAO vertex Array Object
	// EBO Element buffer Object	Index buffer Object IBO
	points_vbo = 0, colours_vbo = 0, VAO = 0;

	glGenBuffers(1, &points_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

	glGenBuffers(1, &colours_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, colours_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colours), colours, GL_STATIC_DRAW);

	glGenVertexArrays(1, &VAO);
	// 绑定VAO
	glBindVertexArray(VAO);
	// 任何（在GL_ARRAY_BUFFER目标上的）缓冲调用都会用来配置当前绑定的缓冲(VBO)
	glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
	// 链接顶点属性 如何解析数据 设置顶点属性指针
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, colours_vbo);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	// 解绑BindBuffer
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// 解绑VAO
	glBindVertexArray(0);
}
void GameLoop()
{
	glfwPollEvents();

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(shaderProgram);
	glBindVertexArray(VAO);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glDrawArrays(GL_TRIANGLES, 0, 3);
	glBindVertexArray(0);

	glEnable(GL_CULL_FACE); // cull face
	glCullFace(GL_BACK); // cull back face
	glFrontFace(GL_CW); // GL_CCW for counter clock-wise

	glfwSwapBuffers(window);
}

void DeleteBufferAndArrays()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &points_vbo);
	glDeleteBuffers(1, &colours_vbo);
	glDeleteBuffers(1, &EBO);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
}