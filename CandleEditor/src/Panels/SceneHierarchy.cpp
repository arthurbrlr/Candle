#include "../EditorLayer.h"

#include "Candle/Core/Input.h"
#include "Candle/Scenes/SceneManagement.h"

using namespace Candle;

	/*
		--- ShowSceneHierarchy
		Display the scene tree given by the ECS
		Only Blueprints and their childrens are displayed
	*/

void EditorLayer::ShowSceneHierarchy()
{
	ImGui::Begin("Scene Hierarchy");

	static Burst::Entity draggedEntity = -1;
	static Burst::Entity targetEntity = -1;
	static bool issueDragAndDrop = false;

	static Burst::Entity hoveredEntity = -1;
	static Burst::Entity selectedEntity;
	static int selected = -1;
	selectedEntity = -1;

	int index = 0;

	static bool openHierarchyPopup = false;
	static bool openEntityPopup = false;

	bool windowFocusAndHovered = ImGui::IsWindowHovered();


	if ( ImGui::Button("Reload Scene") ) {
		SceneManagement::Reload();
		_eb.wasSceneModified = false;
	}

	if ( ImGui::Button("Clear Selection") ) {
		selected = -1;
		selectedEntity = -1;
	}


	std::function<void(Entity&)> BuildEntityHierarchy = [&](Entity& currentEntity) -> void {

		index++;
		ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;

		std::string entityName = currentEntity.GetComponent<EntityTagComponent>().GetName();
		bool hasHierarchy = currentEntity.HasComponent<HierarchyComponent>();

		if ( ( hasHierarchy && !currentEntity.GetComponent<HierarchyComponent>().HasChilds() ) || !hasHierarchy )
			node_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
		if ( selected == index ) {
			node_flags |= ImGuiTreeNodeFlags_Selected;
			selectedEntity = currentEntity.GetNativeEntity();
		}

		bool nodeSelected = ImGui::TreeNodeEx(entityName.c_str(), node_flags);

		if ( ImGui::IsItemHovered() ) hoveredEntity = currentEntity.GetNativeEntity();
		if ( ImGui::IsItemClicked() ) selected = index;

		if ( ImGui::BeginDragDropSource() ) {
			draggedEntity = currentEntity.GetNativeEntity();
			ImGui::SetDragDropPayload("MOVE_ENTITY", &draggedEntity, sizeof(Burst::Entity));
			ImGui::EndDragDropSource();
		}

		if ( ImGui::BeginDragDropTarget() ) {
			if ( const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("MOVE_ENTITY") ) {
				//IM_ASSERT(payload->DataSize == sizeof(Burst::Entity));
				//Burst::Entity payloadData = *(const Burst::Entity*)payload->Data;
				issueDragAndDrop = true;
				targetEntity = currentEntity.GetNativeEntity();
			}
			ImGui::EndDragDropTarget();
		}

		if ( nodeSelected && hasHierarchy && currentEntity.GetComponent<HierarchyComponent>().HasChilds() ) {
			for ( auto& child : currentEntity.GetComponent<HierarchyComponent>().GetChilds() ) {
				Entity childEntity = Entity{ SceneManagement::CurrentScene(), child };
				BuildEntityHierarchy(childEntity);
			}
			ImGui::TreePop();
		}
	};


	for ( auto& entity : ECS::ViewAllEntities() ) {
		Entity currentEntity = Entity{ SceneManagement::CurrentScene(), entity.second };
		bool hasHierarchy = currentEntity.HasComponent<HierarchyComponent>();
		if ( !hasHierarchy || ( hasHierarchy && !currentEntity.GetComponent<HierarchyComponent>().HasParent() ) )
			BuildEntityHierarchy(currentEntity);
	}

	if ( selectedEntity != -1 ) {
		Entity entityToShow = Entity{ SceneManagement::CurrentScene(), selectedEntity };
		ShowComponentsOf(entityToShow);
	} else {
		ImGui::Begin("Entity Components");
		ImGui::End();
	}


	if ( Input::IsKeyPressed(CDL_KEY_LEFT_CONTROL) && Input::OnKeyDown(CDL_KEY_D) ) {
		CTRACE("Control D -> Copy Entity");
	}


	if ( ( Input::IsMouseButtonPressed(1) || openHierarchyPopup ) && windowFocusAndHovered ) {
		openHierarchyPopup = true;
		ImGui::OpenPopup("popup_hierarchy");
	}

	if ( ImGui::BeginPopup("popup_hierarchy") ) {

		if ( ImGui::MenuItem("New Entity") ) {
			_eb.wasSceneModified = true;
			ECS::New("new_entity");
		}

		if ( selected != -1 && selectedEntity != -1 && ImGui::MenuItem("Delete Entity") ) {
			_eb.wasSceneModified = true;
			ECS::Remove(selectedEntity);
			selected = -1;
			selectedEntity = -1;
		}
		ImGui::EndPopup();
	}

	if ( ( Input::IsMouseButtonPressed(0) || Input::IsMouseButtonPressed(2) ) && openHierarchyPopup ) {
		openHierarchyPopup = false;
		ImGui::CloseCurrentPopup();
	}

	ImGui::End();

	if ( issueDragAndDrop ) {
		Entity newParent = Entity{ SceneManagement::CurrentScene(), targetEntity };
		Entity newChild = Entity{ SceneManagement::CurrentScene(), draggedEntity };
		newParent.AddChild(newChild);

		targetEntity = -1;
		draggedEntity = -1;
		issueDragAndDrop = false;
	}

}