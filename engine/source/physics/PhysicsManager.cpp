#include "PhysicsManager.h"
#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>
#include "RigidBody.h"

namespace eng
{
    PhysicsManager::PhysicsManager()
    {
        
    }

    PhysicsManager::~PhysicsManager()
    {
    }

    void PhysicsManager::Init()
    {
        m_broadphase = std::make_unique<btDbvtBroadphase>();
        m_collisionConfiguration = std::make_unique<btDefaultCollisionConfiguration>();
        m_dispatcher = std::make_unique<btCollisionDispatcher>(m_collisionConfiguration.get());
        m_solver = std::make_unique<btSequentialImpulseConstraintSolver>();
        m_world = std::make_unique<btDiscreteDynamicsWorld>(m_dispatcher.get(), m_broadphase.get(), m_solver.get(), m_collisionConfiguration.get());
        m_world->setGravity(btVector3(0, -9.81f, 0));
    }

    void PhysicsManager::Update(float DeltaTime)
    {
        const btScalar fixedTimeStep = 1.f / 60.f;
        const int maxSubSteps = 4;
        m_world->stepSimulation(DeltaTime, maxSubSteps, fixedTimeStep);
    }

    void PhysicsManager::AddRigidBody(RigidBody* body)
    {
        if (!body || !m_world)
        {
            return;
        }
        
        if (auto rigidBody = body->GetRigidBody())
        {
            m_world->addRigidBody(rigidBody, btBroadphaseProxy::StaticFilter, btBroadphaseProxy::AllFilter);
            body->SetAddedToWorld(true);
        }
    }

    void PhysicsManager::RemoveRigidBody(RigidBody* body)
    {
        if (!body || !m_world)
        {
            return;
        }
        
        if (auto rigidBody = body->GetRigidBody())
        {
            m_world->removeRigidBody(body->GetRigidBody());
            body->SetAddedToWorld(false);
        }
    }

    btDiscreteDynamicsWorld* PhysicsManager::GetWorld() const
    {
        return m_world.get();
    }
}
