#pragma once
#include "PxPhysicsAPI.h"
#include "Vector3.h"
#include "Singleton.h"

namespace physx
{
	class PxPhysics;
	class PxScene;
	class PxDefaultCpuDispatcher;
	class PxMaterial;
	class PxRigidDynamic;
}

class CPhysicsEntity
{
private:
	physx::PxRigidDynamic* m_Actor;

public:
	CVector3 GetPosition () const;
	void SetVelocity(CVector3 const& vel);
	void SetAddVelocity (CVector3 const& vel);
	void SetPosition(CVector3 const& pos);
	
	CPhysicsEntity (physx::PxRigidDynamic* actor);
};

class CPhysics :
	public CSingleton<CPhysics>
{
	physx::PxPhysics* m_SDK;
	physx::PxCooking* m_Cooking;
	physx::PxScene* m_Scene;
	physx::PxDefaultCpuDispatcher* m_CpuDispatcher;
	physx::PxMaterial* m_Material;

public:
	void Init ();
	physx::PxPhysics* GetSDK() { return m_SDK; }
	physx::PxCooking* GetCooking() { return m_Cooking; }
	physx::PxMaterial* GetBaseMaterial() { return m_Material; }
	physx::PxScene* GetScene() { return m_Scene; }
	void CreateGround (int samplesX, int samplesY, float hScale, float wScale, const float* heightmap);
	void SimulateStep();

	CPhysicsEntity* CreateBall (float radius, CVector3 const& position);
	bool LoadTriangleMesh(std::string const& path, CVector3 const& pos);
};
