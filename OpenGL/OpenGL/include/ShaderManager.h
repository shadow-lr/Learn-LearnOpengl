#pragma once
#ifndef SHADER_MANAGER_H
#define SHADER_MANAGER_H
#include <unordered_map>
#include <custom_shader.h>
class ShaderManager {
private:
	ShaderManager() {  }
	static ShaderManager* Instance;
protected:
	std::unordered_map<std::string, Shader*> shaderMap;
public:
	static ShaderManager* GetInstance()
	{
		if (ShaderManager::Instance == nullptr)
		{
			ShaderManager::Instance = new ShaderManager();
		}
		return ShaderManager::Instance;
	}

	// add shader by name
	void AddShader(const std::string& name, Shader& shaderObject)
	{
		if (shaderMap[name] == nullptr)
		{
			shaderMap[name] = &shaderObject;
		}
		else
		{
			std::cout << "Add Shader Failed:shader not added yet" << std::endl;
		}
	}

	// delete shader by name
	void DeleteShader(const std::string& name)
	{
		if (shaderMap[name] != nullptr)
		{
			shaderMap[name] = nullptr;
		}
		else
		{
			std::cout << "Delete Shader Failed:shader not added yet" << std::endl;
		}
	}

	// use shader by name
	static void UseShaderByName(const std::string& name)
	{
		if (ShaderManager::GetInstance()->shaderMap[name] != nullptr)
		{
			ShaderManager::GetInstance()->shaderMap[name]->Use();
		}
		else
		{
			std::cout << "Use Shader Failed:shader not added yet" << std::endl;
		}
	}

	// return shader by name
	static Shader* GetShaderByName(const std::string& name)
	{
		if (ShaderManager::GetInstance()->shaderMap[name] != nullptr)
		{
			return ShaderManager::GetInstance()->shaderMap[name];
		}
		else
		{
			std::cout << "Get Shader Failed:shader not added yet" << std::endl;
			return nullptr;
		}
	}
};
ShaderManager* ShaderManager::Instance = nullptr;
#endif