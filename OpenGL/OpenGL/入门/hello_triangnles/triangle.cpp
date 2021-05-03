#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

using std::cout;
using std::endl;

GLfloat vertices[] = {
	// ��һ��������
	-0.7f, -0.5f, 0.0f,
	 0.3f, -0.5f, 0.0f,
	 -0.2f,  0.5f, 0.0f
	 // �ڶ���������
	,0.3f,-0.5f,0.0f,
	0.8f,-0.5f,0.0f,
	0.55f,0.5f,0.0f
};

GLuint indices[] = { // ע��������0��ʼ! 
	0, 1, 3, // ��һ��������
	1, 2, 3  // �ڶ���������
};

const GLchar* vertexShaderSource =
	"#version 330 core\n"
	"layout (location = 0) in vec3 position;\n"
	"void main()\n"
	"{\n"
	"gl_Position = vec4(position.x, position.y, position.z, 1.0);\n"
	"}\0";
const GLchar* fragmentShaderSource =
	"#version 330 core\n"
	"out vec4 color;\n"
	"void main()\n"
	"{\n"
	"color = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
	"}\n\0";

GLFWwindow* window;

const GLuint WIDTH = 800, HEIGHT = 600;
GLuint shaderProgram;
GLuint VBO, VAO, EBO;

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
	// ������ɫ��
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	// Դ�븽�ӵ���ɫ����
	glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
	glCompileShader(vertexShader);

	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// ����shader
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void InitObjectVertexFragmentData()
{
	// VBO vertex buffer Object
	// VAO vertex Array Object
	// EBO Element buffer Object	Index buffer Object IBO
	//GLuint VBO, VAO, EBO;

	glGenVertexArrays(1, &VAO);
	// ���㻺�������OpenGL����
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	// ��VAO
	glBindVertexArray(VAO);
	// �κΣ���GL_ARRAY_BUFFERĿ���ϵģ�������ö����������õ�ǰ�󶨵Ļ���(VBO)
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// �������ݸ��Ƶ�������ڴ���
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// usage ʹ������
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// ���Ӷ������� ��ν������� ���ö�������ָ��
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// ���BindBuffer
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// ���VAO
	glBindVertexArray(0);
}
void GameLoop()
{
	glfwPollEvents();

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(shaderProgram);
	glBindVertexArray(VAO);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glDrawArrays(GL_TRIANGLES, 0, 3);
	glDrawArrays(GL_TRIANGLES, 3, 3);
	glBindVertexArray(0);

	glfwSwapBuffers(window);
}

void DeleteBufferAndArrays()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
}