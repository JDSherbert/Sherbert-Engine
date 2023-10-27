//©2021 JDSherbert. All Rights Reserved.

#include "Inspector.h"

#include "../../ECS/Entity.h"
#include "../../ECS/Component/Component.h"
#include "../../ECS/Component/TransformComponent.h"
#include "../../ECS/Component/MeshComponent.h"
#include "../../ECS/Component/CameraComponent.h"
#include "../../ECS/Component/TextMeshComponent.h"
#include "../../ECS/Component/RigidbodyComponent.h"
#include "../../System/ScriptingSystem.h"
#include "../../Editor/Window/Assets.h"
#include "../../System/PhysicsSystem.h"

static InspectorWindow inspectorWindow;

InspectorWindow& InspectorClass()
{
	return inspectorWindow;
}

///////////////////////////////////////////////////////////////

static Entity* ecs = &EntityClass();
static AssetsWindow* assetsWindow = &AssetsClass();

void InspectorWindow::Render()
{
	if (!renderInspector)
		return;

	ImGui::Begin("Entity");
	{
		if (ecs->selected != entt::null)
		{
			RenderComponent<Component>(ecs->selected);
			RenderComponent<TransformComponent>(ecs->selected);
			RenderComponent<MeshComponent>(ecs->selected);
			RenderComponent<CameraComponent>(ecs->selected);
			RenderComponent<TextMeshComponent>(ecs->selected);
			RenderComponent<RigidBodyComponent>(ecs->selected);
			RenderComponent<ScriptingComponent>(ecs->selected);
			RenderComponent<PhysicsComponent>(ecs->selected);
			RenderAdd();
		}
	}
	ImGui::End();
}

void InspectorWindow::RenderAdd()
{
	ImGui::Separator();

	if (ImGui::Button("Add"))
		ImGui::OpenPopup("addComp");

	if (ImGui::BeginPopup("addComp", ImGuiWindowFlags_NoMove))
	{
		if (ImGui::Selectable("MeshComponent"))
		{
			if (!ecs->HasComponent<MeshComponent>(ecs->selected))
				ecs->AddComponent<MeshComponent>(ecs->selected);
		}
		if (ImGui::Selectable("TextMeshComponent"))
		{
			if (!ecs->HasComponent<TextMeshComponent>(ecs->selected))
				ecs->AddComponent<TextMeshComponent>(ecs->selected);
		}
		ImGui::Separator();
		if (ImGui::Selectable("CameraComponent"))
		{
			if (!ecs->HasComponent<CameraComponent>(ecs->selected))
				ecs->AddComponent<CameraComponent>(ecs->selected);
		}
		ImGui::Separator();
		if (ImGui::Selectable("RigidBodyComponent"))
		{
			if (!ecs->HasComponent<RigidBodyComponent>(ecs->selected))
			{
				ecs->AddComponent<RigidBodyComponent>(ecs->selected);
				ecs->GetComponent<RigidBodyComponent>(ecs->selected).CreateActor();
			}
		}

		if (ImGui::BeginMenu("Collider"))
		{
			if (ImGui::MenuItem("Box"))
			{
				ecs->registry.get<PhysicsComponent>(ecs->selected).AddBoxCollider();
			}

			ImGui::EndMenu();
		}

		ImGui::Separator();

		static std::vector<std::pair<std::string, std::string>> data;

		if (ImGui::BeginMenu("Script"))
		{
			if (ImGui::IsWindowAppearing())
				assetsWindow->GetFileNameFromProjectDir(assetsWindow->GetProjectDirPath(), LUA, data);
			for (size_t i = 0; i < data.size(); i++)
				if (ImGui::MenuItem(data[i].second.c_str()))
					ecs->registry.get<ScriptingComponent>(ecs->selected).AddScript(data[i].first.c_str());
			ImGui::EndMenu();
		}
		else
		{
			data.clear();
		}

		/****/
		ImGui::EndPopup();
	}
}

template <typename T>
void InspectorWindow::RenderComponent(entt::entity entity)
{
	if (ecs->registry.any_of<T>(entity))
		ecs->registry.get<T>(entity).Render();
}