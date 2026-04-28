#include "Engine.h"
#include "Application.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

namespace eng
{
    bool Engine::Init(int width, int height)
    {
        if (!m_application)
        {
            return false;
        }
        
        if (!glfwInit())
        {
            std::cout << "Failed to initialize GLFW" << std::endl;
            return false;
        }
        
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        
        m_window = glfwCreateWindow(width, height, "Engine", NULL, NULL);
        
        if (!m_window)
        {
            std::cout << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
            return false;
        }
        
        glfwMakeContextCurrent(m_window);
        
        if (glewInit() != GLEW_OK)
        {
            std::cout << "Failed to initialize GLEW" << std::endl;
            glfwTerminate();
            return false;
        }
        
        return m_application->Init();
    }

    void Engine::Run()
    {
        if (!m_application)
        {
            return;
        }
        
        m_lastTimeTime = std::chrono::high_resolution_clock::now();
        while (!glfwWindowShouldClose(m_window) && !m_application->NeedsToBeClosed())
        {
            glfwPollEvents();
            
            auto currentTime = std::chrono::high_resolution_clock::now();
            float DeltaTime = std::chrono::duration<float>(currentTime - m_lastTimeTime).count();
            m_lastTimeTime = currentTime;
            
            m_application->Update(DeltaTime);
            
            glfwSwapBuffers(m_window);
        }
    }

    void Engine::Destroy()
    {
        if (m_application)
        {
            m_application->Destroy();
            m_application.reset();
            glfwTerminate();
            m_window = nullptr;
        }
    }

    void Engine::SetApplication(Application* app)
    {
        m_application.reset(app);
    }

    Application* Engine::GetApplication()
    {
        return m_application.get();
    }
}
