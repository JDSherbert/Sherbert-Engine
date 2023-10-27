//©2021 JDSherbert. All Rights Reserved.

#include "Component.h"

#include "../../ECS/Component/MeshComponent.h"

static Entity* ecs = &EntityClass();

void Component::Render()
{
	if (ImGui::CollapsingHeader("COMPONENT", ImGuiTreeNodeFlags_DefaultOpen))
	{
		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Copy")) {}
			if (ImGui::MenuItem("Paste")) {}
			ImGui::Separator();
			ImGui::MenuItem("Remove", "", false, false);
			ImGui::EndPopup();
		}

		if (ImGui::BeginTable("ComponentTable", 2, ImGuiTableFlags_Resizable))
		{
			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			{
				ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 2);
				ImGui::Text("Active");
				ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 2 + 4);
				ImGui::Text("Name");
				ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 2 + 4);
				ImGui::Text("Static");
				ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 2 + 4);
				ImGui::Text("Tag");
			}
			ImGui::TableNextColumn();
			ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
			{
				bool _Active = IsActive();
				if (ImGui::Checkbox("##ActiveComponent", &_Active))
					if (_Active) SetActive(true); else SetActive(false);

				//ImVec2 size = ImGui::GetItemRectSize();
				//StarHelpers::AddLog("%f", size.y);

				std::string _Name = GetName();
				if (ImGui::InputText("##NameComponent", &_Name, ImGuiInputTextFlags_EnterReturnsTrue))
					SetName(_Name);

				bool _Static = IsStatic();
				if (ImGui::Checkbox("##StaticComponent", &_Static))
					if (_Static) SetStatic(true); else SetStatic(false);

				std::string _Tag = GetTag();
				if (ImGui::InputText("##TagComponent", &_Tag, ImGuiInputTextFlags_EnterReturnsTrue))
					SetTag(_Tag);
			}
			ImGui::PopItemWidth();
			ImGui::EndTable();
		}
	}
}

void Component::SetName(std::string name)
{
	if (!name.compare(""))
		return;

	nameEntity = name;
}
void Component::SetTag(std::string tag)
{
	if (!tag.compare(""))
		return;

	tagEntity = tag;
}
std::vector<entt::entity> Component::GetChildren()
{
	return childrenEntity;
}
entt::entity Component::GetParent()
{
	return parentEntity;
}
bool Component::IsStatic()
{
	return staticEntity;
}
bool Component::IsActive()
{
	return activeEntity;
}
std::string Component::GetTag()
{
	return tagEntity;
}
std::string Component::GetName()
{
	return nameEntity;
}
bool Component::HasChildren()
{
	if (childrenEntity.empty()) return true; else return false;
}

void Component::SetActiveAll(entt::entity entity, bool arg)
{
	auto& generalComp = ecs->registry.get<Component>(entity);
	generalComp.activeEntity = arg;

	for (int i = 0; i < generalComp.GetChildren().size(); i++)
		SetActiveAll(generalComp.GetChildren()[i], arg);
}

void Component::SetActive(bool arg)
{
	entt::entity entity = entt::to_entity(ecs->registry, *this);
	SetActiveAll(entity, arg);
}

void Component::SetStaticAll(entt::entity entity, bool arg)
{
	auto& generalComp = ecs->registry.get<Component>(entity);
	generalComp.staticEntity = arg;

	for (int i = 0; i < generalComp.GetChildren().size(); i++)
		SetStaticAll(generalComp.GetChildren()[i], arg);
}

void Component::SetStatic(bool arg)
{
	entt::entity entity = entt::to_entity(ecs->registry, *this);
	SetStaticAll(entity, arg);
}

void Component::EntitySkip(entt::entity entity, int& size)
{
	auto& generalComp = ecs->registry.get<Component>(entity);
	entt::entity thisEntity = entt::to_entity(ecs->registry, *this);

	std::vector<entt::entity>::iterator find = std::find(generalComp.childrenEntity.begin(), generalComp.childrenEntity.end(), thisEntity);
	if (find != generalComp.childrenEntity.end())
		size++;

	for (int i = 0; i < generalComp.childrenEntity.size(); i++)
		EntitySkip(generalComp.childrenEntity[i], size);
}

void Component::AddChild(entt::entity entity)
{
	auto& genComp = ecs->registry.get<Component>(entity);

	//// if move root so return code ////
	if (entity == ecs->root)
		return;

	//// 2. issue fix ////
	if (genComp.parentEntity == entt::null)
	{
		genComp.parentEntity = entt::to_entity(ecs->registry, *this);
		childrenEntity.push_back(entity);
	}
	else
	{
		//// 3. issue fix ////
		int size = 0;
		EntitySkip(entity, size);
		if (size != 0)
			return;

		auto& parent = ecs->registry.get<Component>(genComp.parentEntity);

		//// 4. issue fix ////
		std::vector<entt::entity>::iterator old = std::find(parent.childrenEntity.begin(), parent.childrenEntity.end(), entity);
		if (old != parent.childrenEntity.end())
		{
			auto index = std::distance(parent.childrenEntity.begin(), old);
			parent.childrenEntity.erase(parent.childrenEntity.begin() + index);
		}

		//// 5. issue fix ////
		std::vector<entt::entity>::iterator back = std::find(childrenEntity.begin(), childrenEntity.end(), entity);
		if (back == childrenEntity.end())
		{
			genComp.parentEntity = entt::to_entity(ecs->registry, *this);
			childrenEntity.push_back(entity);
		}
	}
}

static std::vector<entt::entity> toDestroy;

void Component::Destroy()
{
	ecs->selected = entt::null;
	entt::entity entity = entt::to_entity(ecs->registry, *this);

	///////////////////////////////////////////////////////////////

	auto& parent = ecs->registry.get<Component>(parentEntity);
	std::vector<entt::entity>::iterator old = std::find(parent.childrenEntity.begin(), parent.childrenEntity.end(), entity);

	///////////////////////////////////////////////////////////////

	DestroyAll(entity);

	for (size_t i = 0; i < toDestroy.size(); i++)
		ecs->registry.destroy(toDestroy[i]);

	toDestroy.clear();

	///////////////////////////////////////////////////////////////

	if (old != parent.childrenEntity.end())
	{
		auto index = std::distance(parent.childrenEntity.begin(), old);
		parent.childrenEntity.erase(parent.childrenEntity.begin() + index);
	}
}

void Component::DestroyAll(entt::entity entity)
{
	auto& generalComp = ecs->registry.get<Component>(entity);

	toDestroy.push_back(entity);

	for (size_t i = 0; i < generalComp.childrenEntity.size(); i++)
		DestroyAll(generalComp.childrenEntity[i]);
}

void Component::DestroyChildren()
{
	entt::entity entity = entt::to_entity(ecs->registry, *this);
	DestroyAll(entity);

	toDestroy.erase(toDestroy.begin());

	for (size_t i = 0; i < toDestroy.size(); i++)
		ecs->registry.destroy(toDestroy[i]);

	toDestroy.clear();
	childrenEntity.clear();
}

void Component::MoveUp()
{
	entt::entity entity = entt::to_entity(ecs->registry, *this); /* get entity from component */
	auto& parent = ecs->registry.get<Component>(GetParent()); /* get parent from this entity */
	std::vector<entt::entity>::iterator iterator = std::find(parent.childrenEntity.begin(), parent.childrenEntity.end(), entity);
	if (iterator != parent.childrenEntity.end())
	{
		size_t index = std::distance(parent.childrenEntity.begin(), iterator);
		if (index == NULL) /* completely up so return */
			return;

		std::swap(parent.childrenEntity[index], parent.childrenEntity[index - 1]);
	}
}

void Component::MoveDown()
{
	entt::entity entity = entt::to_entity(ecs->registry, *this); /* get entity from component */
	auto& parent = ecs->registry.get<Component>(GetParent()); /* get parent from this entity */
	std::vector<entt::entity>::iterator iterator = std::find(parent.childrenEntity.begin(), parent.childrenEntity.end(), entity);
	if (iterator != parent.childrenEntity.end())
	{
		size_t index = std::distance(parent.childrenEntity.begin(), iterator);
		if (index == parent.childrenEntity.size() - 1) /* completely down so return */
			return;

		std::swap(parent.childrenEntity[index], parent.childrenEntity[index + 1]);
	}
}

void Component::SerializeComponent(YAML::Emitter& out)
{
	out << YAML::Key << "Component";
	out << YAML::BeginMap;
	{
		out << YAML::Key << "Name" << YAML::Value << nameEntity;
		out << YAML::Key << "Tag" << YAML::Value << tagEntity;
		out << YAML::Key << "Active" << YAML::Value << activeEntity;
		out << YAML::Key << "Static" << YAML::Value << staticEntity;
		out << YAML::Key << "Children" << YAML::Value << childrenEntity.size();
	}
	out << YAML::EndMap;
}

void Component::DeserializeComponent(YAML::Node& in)
{
	nameEntity = in["Name"].as<std::string>();
	tagEntity = in["Tag"].as<std::string>();
	activeEntity = in["Active"].as<bool>();
	staticEntity = in["Static"].as<bool>();
}