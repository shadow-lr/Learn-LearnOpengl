#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 LightPos;
uniform vec3 lightPos;

// 顶点着色器
out vec3 FragPos;  
out vec3 Normal;

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0);
    FragPos = vec3(model * vec4(position, 1.0));
    Normal = mat3(transpose(inverse(model))) * normal;
    // 将light的世界坐标转换为观察空间下的观察坐标
//    LightPos = vec3(view * vec4(lightPos, 1.0));
}