#include "Engine.h"
#include "Application.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "scene/Scene.h"
#include "scene/components/CameraComponent.h"

namespace eng
{
    void keyCallBack(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        auto& inputManager = Engine::GetInstance().GetInputManager();
        if (action == GLFW_PRESS)
        {
            inputManager.SetKeyPressed(key, true);
        }
        else if (action == GLFW_RELEASE)
        {
            inputManager.SetKeyPressed(key, false);
        }
    }
    
    void mouseButtonCallBack(GLFWwindow* window, int button, int action, int mods)
    {
        auto& inputManager = Engine::GetInstance().GetInputManager();
        if (action == GLFW_PRESS)
        {
            inputManager.SetMouseButtonPressed(button, true);
        }
        else if (action == GLFW_RELEASE)
        {
            inputManager.SetMouseButtonPressed(button, false);
        }
    }
    
    void cursorPositionCallBack(GLFWwindow* window, double xpos, double ypos)
    {
        auto& inputManager = Engine::GetInstance().GetInputManager();
        inputManager.SetMousePositionPrev(inputManager.GetMousePositionCurrent());
        inputManager.SetMousePositionCurrent(glm::vec2(static_cast<float>(xpos), static_cast<float>(ypos)));
    }
    
    Engine& Engine::GetInstance()
    {
        static Engine instance;
        return instance;
    }

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
        
        glfwSetKeyCallback(m_window, keyCallBack);
        glfwSetMouseButtonCallback(m_window, mouseButtonCallBack);
        glfwSetCursorPosCallback(m_window, cursorPositionCallBack);
        
        glfwMakeContextCurrent(m_window);
        
        if (glewInit() != GLEW_OK)
        {
            std::cout << "Failed to initialize GLEW" << std::endl;
            glfwTerminate();
            return false;
        }
        
        m_graphicsAPI.Init();
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
            
            m_graphicsAPI.SetClearColor(1.f, 1.f, 1.f, 1.f);
            m_graphicsAPI.ClearBuffers();
            
            CameraData cameraData;
            
            int width, height;
            glfwGetWindowSize(m_window, &width, &height);
            float aspectRatio = static_cast<float>(width) / static_cast<float>(height);
            
            if (m_currentScene)
            {
                if (auto cameraObject = m_currentScene->GetMainCamera())
                {
                    // logic for matrices
                    auto CameraComponent = cameraObject->GetComponent<class CameraComponent>();
                    if (CameraComponent)
                    {
                        cameraData.viewMatrix = CameraComponent->GetViewMatrix();
                        cameraData.projectionMatrix = CameraComponent->GetProjectionMatrix(aspectRatio);
                    }
                }
            }
            
            m_renderQueue.Draw(m_graphicsAPI, cameraData);
            
            glfwSwapBuffers(m_window);
            
            m_inputManager.SetMousePositionPrev(m_inputManager.GetMousePositionCurrent());
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

    Application* Engine::GetApplication() const
    {
        return m_application.get();
    }

    InputManager& Engine::GetInputManager()
    {
        return m_inputManager;
    }

    GraphicsAPI& Engine::GetGraphicsAPI()
    {
        return m_graphicsAPI;   
    }

    RenderQueue& Engine::GetRenderQueue()
    {
        return m_renderQueue;
    }

    FileSystem& Engine::GetFileSystem()
    {
        return m_fileSystem;
    }

    Scene* Engine::GetScene() const
    {
        return m_currentScene.get();
    }

    void Engine::SetScene(Scene* scene)
    {
        m_currentScene.reset(scene);
    }
}
