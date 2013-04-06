#include <malloc.h>
#include "Physics.h"
#include <extensions/PxDefaultErrorCallback.h>
#include <extensions/PxDefaultAllocator.h>
#include <extensions/PxDefaultSimulationFilterShader.h>
#include <extensions/PxDefaultCpuDispatcher.h>
#include <extensions/PxExtensionsAPI.h>
#include <extensions/PxVisualDebuggerExt.h>
#include <PxTask.h>
//PVD
#include <PvdConnection.h>

/*#include <RepX.h>
#include <RepXUtility.h>
#include <RepXCoreExtensions.h>
#include <PxToolkit.h>*/

using physx::PxDefaultErrorCallback;
using physx::PxDefaultAllocator;
using physx::PxDefaultSimulationFilterShader;

using namespace physx;

class CCollisionCallback : public PxSimulationEventCallback
{
public:
    void onContact(PxContactPair& pair, PxU32 events)
    {
        if(events & PxPairFlag::eNOTIFY_TOUCH_FOUND)
        {
            _CrtDbgBreak();
                /*if((pair.actors[0] == mSubmarineActor) || (pair.actors[1] == mSubmarineActor))
                {
                        PxActor* otherActor = (mSubmarineActor == pair.actors[0]) ? pair.actors[1] : pair.actors[0];
                        Seamine* mine =  reinterpret_cast<Seamine*>(otherActor->userData);
                        // insert only once
                        if(std::find(mMinesToExplode.begin(), mMinesToExplode.end(), mine) == mMinesToExplode.end())
                                mMinesToExplode.push_back(mine);
                }*/
        }
    }
};

PxFilterFlags ButterflyFilterShader(
        PxFilterObjectAttributes attributes0, PxFilterData filterData0,
        PxFilterObjectAttributes attributes1, PxFilterData filterData1,
        PxPairFlags& pairFlags, const void* constantBlock, PxU32 constantBlockSize)
{
        // let triggers through
        if(PxFilterObjectIsTrigger(attributes0) || PxFilterObjectIsTrigger(attributes1))
        {
                pairFlags = PxPairFlag::eTRIGGER_DEFAULT | PxPairFlag::eNOTIFY_TOUCH_PERSISTS;
                return PxFilterFlag::eDEFAULT;
        }
        // generate contacts for all that were not filtered above
        pairFlags = PxPairFlag::eCONTACT_DEFAULT;

        // trigger the contact callback for pairs (A,B) where
        // the filtermask of A contains the ID of B and vice versa.
        if((filterData0.word0 & filterData1.word1) && (filterData1.word0 & filterData0.word1))
                pairFlags |= PxPairFlag::eNOTIFY_TOUCH_FOUND;

        return PxFilterFlag::eDEFAULT;
}


CVector3 CPhysicsEntity::GetPosition() const
{
    CVector3 Temp;
    auto Transform = m_Actor->getGlobalPose();
    Temp.X = Transform.p.x;
    Temp.Y = Transform.p.y;
    Temp.Z = Transform.p.z;
    return Temp;
}

void CPhysicsEntity::SetVelocity(CVector3 const& vel) 
{
    m_Actor->setLinearVelocity(PxVec3(vel.X, vel.Y, vel.Z));
}

void CPhysicsEntity::SetAddVelocity(CVector3 const& vel) 
{
    PxVec3 Curr = m_Actor->getLinearVelocity();
    Curr += PxVec3(vel.X, vel.Y, vel.Z);
    m_Actor->setLinearVelocity(Curr);
}

void CPhysicsEntity::SetPosition(CVector3 const& pos) 
{
    m_Actor->setGlobalPose(PxTransform(PxVec3(pos.X, pos.Y, pos.Z)));
}

CPhysicsEntity::CPhysicsEntity(PxRigidDynamic* actor)
{
    m_Actor = actor;
}

void CPhysics::Init()
{
    bool recordMemoryAllocations = true;

    static PxDefaultErrorCallback gDefaultErrorCallback;
    static PxDefaultAllocator gDefaultAllocatorCallback;

    
    m_SDK = PxCreatePhysics(PX_PHYSICS_VERSION, gDefaultAllocatorCallback, gDefaultErrorCallback, physx::PxTolerancesScale(), recordMemoryAllocations );
    if(!m_SDK)
        _CrtDbgBreak();

    if (!PxInitExtensions(*m_SDK))
        _CrtDbgBreak();

    m_Cooking = PxCreateCooking(PX_PHYSICS_VERSION, &m_SDK->getFoundation(), PxCookingParams());
    if (!m_Cooking)
        _CrtDbgBreak();


    PxSceneDesc sceneDesc(m_SDK->getTolerancesScale());
    sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f);
    //customizeSceneDesc(sceneDesc);

    if(!sceneDesc.cpuDispatcher)
    {
        unsigned int Threads = 1;
        m_CpuDispatcher = PxDefaultCpuDispatcherCreate(Threads);
        if(!m_CpuDispatcher)
            _CrtDbgBreak();
        sceneDesc.cpuDispatcher = static_cast<physx::pxtask::CpuDispatcher*>(m_CpuDispatcher);
    }
    if(!sceneDesc.filterShader)
        //sceneDesc.filterShader  = &PxDefaultSimulationFilterShader;
        sceneDesc.filterShader = &ButterflyFilterShader;

/*    #ifdef PX_WINDOWS
    if (true)
    {
        pxtask::CudaContextManagerDesc cudaContextManagerDesc;
        mCudaContextManager = pxtask::createCudaContextManager(cudaContextManagerDesc, &mSDK->getProfileZoneManager());
    }
    if(!sceneDesc.gpuDispatcher && mCudaContextManager)
    {
        sceneDesc.gpuDispatcher = mCudaContextManager->getGpuDispatcher();
    }
    #endif*/

    m_Scene = m_SDK->createScene(sceneDesc);
    if (!m_Scene)
        _CrtDbgBreak();

    // Basicowy material
    m_Material = m_SDK->createMaterial(0.5f, 0.5f, 0.1f);     //static friction, dynamic friction, restitution
    if(!m_Material)
        _CrtDbgBreak();

    m_Scene->setSimulationEventCallback(new CCollisionCallback);

    // PVD
    if (m_SDK->getPvdConnectionManager() == NULL)
        _CrtDbgBreak();
    //The connection flags state overall what data is to be sent to PVD.  Currently
    //the Debug connection flag requires support from the implementation (don't send
    //the data when debug isn't set) but the other two flags, profile and memory
    //are taken care of by the PVD SDK.
    //PVD::TConnectionFlagsType theConnectionFlags( PVD::PvdConnectionType::Debug | PVD::PvdConnectionType::Profile | PVD::PvdConnectionType::Memory );

    //Use these flags for a clean profile trace with minimal overhead
    PVD::TConnectionFlagsType theConnectionFlags( PVD::PvdConnectionType::Profile | PVD::PvdConnectionType::Debug);

    //Create a pvd connection that writes data straight to the filesystem.  This is
    //the fastest connection on windows for various reasons.  First, the transport is quite fast as
    //pvd writes data in blocks and filesystems work well with that abstraction.
    //Second, you don't have the PVD application parsing data and using CPU and memory bandwidth
    //while your application is running.
    //return mSDK->getPvdConnectionManager()->connect("c:\\temp.pxd2", true, theConnectionFlags);
    
    //The normal way to connect to pvd.  PVD needs to be running at the time this function is called.
    //We don't worry about the return value because we are already registered as a listener for connections
    //and thus our onPvdConnected call will take care of setting up our basic connection state.
    PVD::PvdConnection* Conn = PxExtensionVisualDebugger::connect(m_SDK->getPvdConnectionManager(), "127.0.0.1", 5425, 10, true, PxDebuggerConnectionFlags( (PxU32)theConnectionFlags) );

    // Test plane
    PxReal d = 0.0f;
    PxU32 axis = 1;
    PxTransform pose;
    
    #pragma region Testowe obiekty
    if(axis == 0)
            pose = PxTransform(PxVec3(d, 0.0f, 0.0f));
    else if(axis == 1)
            pose = PxTransform(PxVec3(0.0f, d, 0.0f),PxQuat(PxHalfPi, PxVec3(0.0f, 0.0f, 1.0f)));
    else if(axis == 2)
            pose = PxTransform(PxVec3(0.0f, 0.0f, d), PxQuat(-PxHalfPi, PxVec3(0.0f, 1.0f, 0.0f)));

    PxRigidStatic* plane = m_SDK->createRigidStatic(pose);
    if (!plane)
        _CrtDbgBreak();
    PxShape* shape = plane->createShape(PxPlaneGeometry(), *m_Material);
    if (!shape)
        _CrtDbgBreak();
    m_Scene->addActor(*plane);

    PxReal density = 1.0f;
    PxTransform transform(PxVec3(50.0f, 50.0f, 50.0f), PxQuat::createIdentity());
    PxVec3 dimensions(1.0f, 1.0f, 1.0f);
    PxBoxGeometry geometry(dimensions);

    PxRigidDynamic *actor = PxCreateDynamic(*m_SDK, transform, geometry, *m_Material, density);
    if (!actor)
        _CrtDbgBreak();
    m_Scene->addActor(*actor);

    //physx::repx::RepXCollection* collection = physx::repx::createCollection("file.xml");
    //physx::repx::addObjectsToScene(collection, m_SDK, m_Cooking, m_Scene, 0);

    #pragma endregion
}

void CPhysics::CreateGround (int samplesX, int samplesY, float hScale, float wScale, const float* heightmap)
{
    PxHeightFieldDesc hfDesc;

    PxHeightFieldSample* Samples = new PxHeightFieldSample[sizeof(PxHeightFieldSample)*(samplesX*samplesY)];
    for (int x = 0; x < samplesX; ++x)
    {
        for (int y = 0; y < samplesY; ++ y)
        {
            Samples[x*samplesY + y].materialIndex0 = 0;
            Samples[x*samplesY + y].materialIndex1 = 0;

            short Height = heightmap[x*samplesY + y] * 32000;
            Samples[x*samplesY + y].height = Height;
        }
    }

    hfDesc.format           = PxHeightFieldFormat::eS16_TM;
    hfDesc.nbColumns        = samplesX;
    hfDesc.nbRows           = samplesY;
    hfDesc.samples.data     = Samples;
    hfDesc.samples.stride   = sizeof(PxHeightFieldSample);

    PxHeightField* aHeightField = m_SDK->createHeightField(hfDesc);
    if (!aHeightField)
        _CrtDbgBreak();

    PxHeightFieldGeometry hfGeom(aHeightField, PxMeshGeometryFlags(), hScale / 32000.f, wScale, wScale);

    PxTransform Pose = PxTransform(PxVec3(0.f, 0.0f, 0.0f));

    PxRigidStatic* aHeightFieldActor = m_SDK->createRigidStatic(Pose);
    if (!aHeightFieldActor)
        _CrtDbgBreak();

    PxShape* aHeightFieldShape = aHeightFieldActor->createShape(hfGeom, *m_Material);
    m_Scene->addActor(*aHeightFieldActor);

    delete Samples;
}

void CPhysics::SimulateStep()
{
    m_Scene->simulate(1.0 / 60.0);
    m_Scene->fetchResults(true);
}

CPhysicsEntity* CPhysics::CreateBall(float radius, CVector3 const& position)
{
    PxTransform Transform (PxVec3 (position.X, position.Y, position.Z));
    PxRigidDynamic* aSphereActor = m_SDK->createRigidDynamic(Transform);
    PxShape* aSphereShape = aSphereActor->createShape(PxSphereGeometry(radius), *m_Material);
    float SphereDensity = 0.0f;
    PxRigidBodyExt::updateMassAndInertia(*aSphereActor, SphereDensity);
    m_Scene->addActor(*aSphereActor);
    return new CPhysicsEntity(aSphereActor);
}