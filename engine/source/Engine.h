#pragma once
#include <memory>
#include <chrono>
#include "input/InputManager.h"
#include "graphics/GraphicsAPI.h"
#include "graphics/Texture.h"
#include "render/RenderQueue.h"
#include "io/FileSystem.h"

struct GLFWwindow;
namespace eng
{
    class Application;
    class Scene;
    
    class Engine
    {
        Engine() = default;
        Engine(const Engine&) = delete;
        Engine(Engine&&) = delete;
        Engine& operator=(const Engine&) = delete;
        Engine& operator=(Engine&&) = delete;
        
    public:
        static Engine& GetInstance();    
        bool Init(int width, int height);
        void Run();
        void Destroy();
        
        void SetApplication(Application* app);
        Application* GetApplication() const;
        InputManager& GetInputManager();
        GraphicsAPI& GetGraphicsAPI();
        RenderQueue& GetRenderQueue();
        FileSystem& GetFileSystem();
        TextureManager& GetTextureManager();
        
        Scene* GetScene() const;
        void SetScene(Scene* scene);
        
    private:
        std::unique_ptr<Application> m_application;
        std::chrono::high_resolution_clock::time_point m_lastTimeTime;
        GLFWwindow* m_window = nullptr;
        InputManager m_inputManager;
        GraphicsAPI m_graphicsAPI;
        RenderQueue m_renderQueue;
        FileSystem m_fileSystem;
        TextureManager m_textureManager;
        std::unique_ptr<Scene> m_currentScene;
    };   
}
