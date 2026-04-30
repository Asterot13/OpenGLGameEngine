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

        in vec3 vColor;

        void main()
        {
            FragColor = vec4(vColor, 1.0f);
        }
    )";
	
	auto& GraphicsAPI = eng::Engine::GetInstance().GetGraphicsAPI();
	auto ShaderProgram = GraphicsAPI.CreateShaderProgram(VertexShaderCode, FragmentShaderCode);
	m_material.SetShaderProgram(ShaderProgram);
	
	std::vector<float> vertices = 
	{
		0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
		-0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 
		0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f
	};

	std::vector<unsigned int> indices = 
	{
		0, 1, 3,
		1, 2, 3
	};
	
	eng::VertexLayout vertexLayout;
	
	// Position
	vertexLayout.elements.push_back
	({
		0,
		3,
		GL_FLOAT,
		0
	});
	
	// Color
	vertexLayout.elements.push_back
	({
		1,
		3,
		GL_FLOAT,
		sizeof(float) * 3
	});
	
	vertexLayout.stride = sizeof(float) * 6;
	
	m_mesh = std::make_unique<eng::Mesh>(vertexLayout, vertices, indices);
	
	return true;
}

void Game::Update(float DeltaTime)
{
	auto& input = eng::Engine::GetInstance().GetInputManager();
	
	// Horizontal movement
	if (input.IsKeyPressed(GLFW_KEY_A))
	{
		offsetX -= 0.01f;
	}
	else if (input.IsKeyPressed(GLFW_KEY_D))
	{
		offsetX += 0.01f;
	}
	// Vertical movement
	if (input.IsKeyPressed(GLFW_KEY_W))
	{
		offsetY += 0.01f;
	}
	else if (input.IsKeyPressed(GLFW_KEY_S))
	{
		offsetY -= 0.01f;
	}
	
	m_material.SetParam("uOffset", offsetX, offsetY);
	
	eng::RenderCommand command;
	command.material = &m_material;
	command.mesh = m_mesh.get();
	eng::Engine::GetInstance().GetRenderQueue().Submit(command);
}

void Game::Destroy()
{
}
