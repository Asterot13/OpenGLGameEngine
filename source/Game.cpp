#include "Game.h"
#include <iostream>
#include "GLFW/glfw3.h"

bool Game::Init()
{
	std::string VertexShaderCode = R"(
        #version 330 core
        layout (location = 0) in vec3 position;
        layout (location = 1) in vec3 color;
        
        uniform vec2 uOffset;

        out vec3 vColor;

        void main()
        {
            vColor = color;
            gl_Position = vec4(position.x, position.y, position.z, 1.0);
            gl_Position = vec4(position.x + uOffset.x, position.y + uOffset.y, position.z, 1.0);
        }
    )";
	
	std::string FragmentShaderCode = R"(
        #version 330 core
        out vec4 FragColor;

        uniform vec4 uColor;

        in vec3 vColor;

        void main()
        {
            FragColor = vec4(vColor, 1.0f) * uColor;
        }
    )";
	
	auto& GraphicsAPI = eng::Engine::GetInstance().GetGraphicsAPI();
	auto ShaderProgram = GraphicsAPI.CreateShaderProgram(VertexShaderCode, FragmentShaderCode);
	
	return true;
}

void Game::Update(float DeltaTime)
{
	auto& input = eng::Engine::GetInstance().GetInputManager();
	if (input.IsKeyPressed(GLFW_KEY_A))
	{
		std::cout << "[A] button pressed" << std::endl;
	}
}

void Game::Destroy()
{
}
