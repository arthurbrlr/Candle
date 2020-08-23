#include "cdlpch.h"
#include "HierarchyComponent.h"

#include "Candle/ECS/ECS.h"
#include "Candle/ECS/Entity.h"


namespace Candle {

	HierarchyComponent::HierarchyComponent(Burst::Entity parent)
		: _parent(parent)
	{
	}


	void HierarchyComponent::AddChild(Entity childEntity)
	{
		childEntity.AddComponent<HierarchyComponent>(_entity);
		_childs.push_back(childEntity.GetNativeEntity());
	}

	
	void HierarchyComponent::RemoveChild(Entity childEntity)
	{
		auto it = _childs.begin();
		for ( it = _childs.begin(); it != _childs.end(); it++ ) {
			if ( *it == childEntity.GetNativeEntity() ) {
				break;
			}
		}
		_childs.erase(it);
	}


	void HierarchyComponent::RemoveAllChilds()
	{
		auto childsCopy = _childs;
		for ( auto child : childsCopy ) {
			ECS::Remove(child);
		}
		_childs.clear();
	}


	void HierarchyComponent::Clear()
	{
		if ( HasChilds() ) {
			for ( auto child : _childs ) {
				ECS::Remove(child);
			}
		}

		if ( HasParent() ) {
			Entity parent{ SceneManagement::CurrentScene().get(), _parent };
			parent.GetComponent<HierarchyComponent>().RemoveChild(Entity{ SceneManagement::CurrentScene().get(), _entity });
		}
	}


	Entity HierarchyComponent::GetParent()
	{
		return Entity{ SceneManagement::CurrentScene().get(), _parent };
	}


	const std::vector<Burst::Entity>& HierarchyComponent::GetChilds()
	{
		return _childs;
	}


	bool HierarchyComponent::HasChilds()
	{
		return _childs.size() != 0;
	}


	bool HierarchyComponent::HasParent()
	{
		return _parent != -1;
	}

	void HierarchyComponent::OnEditor()
	{
		ImGui::Text("Childs:");
		for ( auto& child : _childs ) {
			Entity currentChild = { SceneManagement::CurrentScene().get(), child };
			ImGui::TreeNodeEx(currentChild.GetUUID().c_str(), ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_SpanAvailWidth);
		}
	}


}