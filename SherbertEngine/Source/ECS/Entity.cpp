//©2021 JDSherbert. All Rights Reserved.

#include "Entity.h"

#include <vector>
#include "../Core/Utils.h"
#include "Component/MeshComponent.h"
#include "../XTK/WICTextureLoader11.h"
#include "Component/Component.h"
#include "Component/TransformComponent.h"
#include "Component/RigidbodyComponent.h"
#include "Component/CameraComponent.h"
#include "Component/TextMeshComponent.h"
#include "../System/ProjectSceneSystem.h"
#include "../System/ScriptingSystem.h"
#include "../System/PhysicsSystem.h"
#include "../Model/AssimpLoader.h"

#define CUBE_MODEL      "Resources\\Models\\Cube.obj"
#define SPHERE_MODEL    "Resources\\Models\\Sphere.obj"
#define CAPSULE_MODEL   "Resources\\Models\\Capsule.obj"
#define PLANE_MODEL     "Resources\\Models\\Plane.obj"

Entity entity;

Entity& EntityClass()
{
	return entity;
}

static AssimpLoader* assimpLoader = &AssimpLoaderClass();
static ProjectSceneSystem* projectSceneSystem = &ProjectSceneSystemClass();

static std::vector<Mesh> cube;
static std::vector<Mesh> sphere;
static std::vector<Mesh> capsule;
static std::vector<Mesh> plane;

bool Entity::Init()
{
    root = CreateEntity();
	GetComponent<Component>(root).SetName("Scene");
    projectSceneSystem->NewScene();
    return true;
}

void Entity::CreateEmptyEntity(entt::entity entity)
{
    GetComponent<Component>(entity).SetName("Empty");
    GetComponent<Component>(root).AddChild(entity);
}
void Entity::CreateCubeEntity(entt::entity entity)
{
    GetComponent<Component>(entity).SetName("Cube");
	AddComponent<MeshComponent>(entity);
	auto& meshComponent = GetComponent<MeshComponent>(entity);

    for (size_t i = 0; i < cube[0].vertices.size(); i++)
    {
        Vertex vertex;
        vertex.position.x = cube[0].vertices[i].position.x;
        vertex.position.y = cube[0].vertices[i].position.y;
        vertex.position.z = cube[0].vertices[i].position.z;
        vertex.normal.x = cube[0].vertices[i].normal.x;
        vertex.normal.y = cube[0].vertices[i].normal.y;
        vertex.normal.z = cube[0].vertices[i].normal.z;
        vertex.texCoords.x = cube[0].vertices[i].texCoords.x;
        vertex.texCoords.y = cube[0].vertices[i].texCoords.y;
        meshComponent.AddVertices(vertex);
    }

    for (size_t i = 0; i < cube[0].indices.size(); i++)
        meshComponent.AddIndices(cube[0].indices[i]);

    meshComponent.SetupMesh();
    meshComponent.SetFileName(CUBE_MODEL);
    meshComponent.SetMeshName("Cube");
    GetComponent<Component>(root).AddChild(entity);
}
void Entity::CreateSphereEntity(entt::entity entity)
{
    GetComponent<Component>(entity).SetName("Sphere");
    AddComponent<MeshComponent>(entity);
    auto& meshComponent = GetComponent<MeshComponent>(entity);

    for (size_t i = 0; i < sphere[0].vertices.size(); i++)
    {
        Vertex vertex;
        vertex.position.x = sphere[0].vertices[i].position.x;
        vertex.position.y = sphere[0].vertices[i].position.y;
        vertex.position.z = sphere[0].vertices[i].position.z;
        vertex.normal.x = sphere[0].vertices[i].normal.x;
        vertex.normal.y = sphere[0].vertices[i].normal.y;
        vertex.normal.z = sphere[0].vertices[i].normal.z;
        vertex.texCoords.x = sphere[0].vertices[i].texCoords.x;
        vertex.texCoords.y = sphere[0].vertices[i].texCoords.y;
        meshComponent.AddVertices(vertex);
    }

    for (size_t i = 0; i < sphere[0].indices.size(); i++)
        meshComponent.AddIndices(sphere[0].indices[i]);

    meshComponent.SetupMesh();
    meshComponent.SetFileName(SPHERE_MODEL);
    meshComponent.SetMeshName("Sphere");
    GetComponent<Component>(root).AddChild(entity);
}
void Entity::CreateCapsuleEntity(entt::entity entity)
{
    GetComponent<Component>(entity).SetName("Capsule");
    AddComponent<MeshComponent>(entity);
    auto& meshComponent = GetComponent<MeshComponent>(entity);

    for (size_t i = 0; i < capsule[0].vertices.size(); i++)
    {
        Vertex vertex;
        vertex.position.x = capsule[0].vertices[i].position.x;
        vertex.position.y = capsule[0].vertices[i].position.y;
        vertex.position.z = capsule[0].vertices[i].position.z;
        vertex.normal.x = capsule[0].vertices[i].normal.x;
        vertex.normal.y = capsule[0].vertices[i].normal.y;
        vertex.normal.z = capsule[0].vertices[i].normal.z;
        vertex.texCoords.x = capsule[0].vertices[i].texCoords.x;
        vertex.texCoords.y = capsule[0].vertices[i].texCoords.y;
        meshComponent.AddVertices(vertex);
    }

    for (size_t i = 0; i < capsule[0].indices.size(); i++)
        meshComponent.AddIndices(capsule[0].indices[i]);

    meshComponent.SetupMesh();
    meshComponent.SetFileName(CAPSULE_MODEL);
    meshComponent.SetMeshName("Capsule");
    GetComponent<Component>(root).AddChild(entity);
}
void Entity::CreatePlaneEntity(entt::entity entity)
{
    GetComponent<Component>(entity).SetName("Plane");
    AddComponent<MeshComponent>(entity);
    auto& meshComponent = GetComponent<MeshComponent>(entity);

    for (size_t i = 0; i < plane[0].vertices.size(); i++)
    {
        Vertex vertex;
        vertex.position.x = plane[0].vertices[i].position.x;
        vertex.position.y = plane[0].vertices[i].position.y;
        vertex.position.z = plane[0].vertices[i].position.z;
        vertex.normal.x = plane[0].vertices[i].normal.x;
        vertex.normal.y = plane[0].vertices[i].normal.y;
        vertex.normal.z = plane[0].vertices[i].normal.z;
        vertex.texCoords.x = plane[0].vertices[i].texCoords.x;
        vertex.texCoords.y = plane[0].vertices[i].texCoords.y;
        meshComponent.AddVertices(vertex);
    }

    for (size_t i = 0; i < plane[0].indices.size(); i++)
        meshComponent.AddIndices(plane[0].indices[i]);

    meshComponent.SetupMesh();
    meshComponent.SetFileName(PLANE_MODEL);
    meshComponent.SetMeshName("Plane");
    GetComponent<Component>(root).AddChild(entity);
}
void Entity::CreateCameraEntity(entt::entity entity)
{
    GetComponent<Component>(entity).SetName("Camera");
    AddComponent<CameraComponent>(entity);
    GetComponent<Component>(root).AddChild(entity);
}
void Entity::CreateTextMeshEntity(entt::entity entity)
{
    GetComponent<Component>(entity).SetName("TextMesh");
    AddComponent<TextMeshComponent>(entity);
    GetComponent<TextMeshComponent>(entity).SetText("Hello World!");
    GetComponent<Component>(root).AddChild(entity);
}

void Entity::LoadCoreModels()
{
    cube    = assimpLoader->LoadRawModel(CUBE_MODEL);
    sphere  = assimpLoader->LoadRawModel(SPHERE_MODEL);
    capsule = assimpLoader->LoadRawModel(CAPSULE_MODEL);
    plane   = assimpLoader->LoadRawModel(PLANE_MODEL);
}

entt::entity Entity::CreateEntity()
{
    entt::entity entity = entt::null;
    entity = registry.create();
    AddComponent<Component>(entity);
    AddComponent<TransformComponent>(entity);
    AddComponent<ScriptingComponent>(entity);
    AddComponent<PhysicsComponent>(entity);
    return entity;
}
void Entity::SetSelectedEntity(entt::entity entity)
{
    selected = entity;
}
entt::entity Entity::GetSelectedEntity()
{
    return selected;
}
void Entity::SetRootEntity(entt::entity entity)
{
    root = entity;
}
entt::entity Entity::GetRootEntity()
{
    return root;
}
entt::registry& Entity::GetRegistry()
{
    return registry;
}