#include "Engine.h"
#include "Application.h"

namespace eng
{
    bool Engine::Init()
    {
        if (!m_application)
        {
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
        while (!m_application->NeedsToBeClosed())
        {
            auto currentTime = std::chrono::high_resolution_clock::now();
            float DeltaTime = std::chrono::duration<float>(currentTime - m_lastTimeTime).count();
            m_lastTimeTime = currentTime;
            
            m_application->Update(DeltaTime);
        }
    }

    void Engine::Destroy()
    {
        if (m_application)
        {
            m_application->Destroy();
            m_application.reset();
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
