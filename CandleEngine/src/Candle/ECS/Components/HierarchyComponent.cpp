#include "cdlpch.h"
#include "HierarchyComponent.h"

#include "Candle/ECS/ECS.h"
#include "Candle/ECS/Entity.h"

#include "Utility/System/SerializeUtility.h"


namespace Candle {

	HierarchyComponent::HierarchyComponent(Burst::Entity parent)
		: _parent(parent)
	{
	}


	void HierarchyComponent::AddChild(Entity childEntity)
	{
		if ( !childEntity.HasComponent<HierarchyComponent>() ) {
			childEntity.AddComponent<HierarchyComponent>(_entity);
		} else {
			Entity thisEntity = Entity{ SceneManagement::CurrentScene(), _entity };

			if ( childEntity.GetComponent<HierarchyComponent>().HasParent() )
				childEntity.GetComponent<HierarchyComponent>().GetParent().GetComponent<HierarchyComponent>().RemoveChild(childEntity);

			childEntity.GetComponent<HierarchyComponent>()._parent = thisEntity.GetNativeEntity();

		}
		_childs.push_back(childEntity.GetNativeEntity());
	}

	
	void HierarchyComponent::RemoveChild(Entity childEntity)
	{
		if ( _childs.size() == 0 ) return;

		auto it = _childs.begin();
		for ( it = _childs.begin(); it != _childs.end(); it++ ) {
			if ( *it == childEntity.GetNativeEntity() ) {
				_childs.erase(it);
				break;
			}
		}
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
			Entity parent{ SceneManagement::CurrentScene(), _parent };
			parent.GetComponent<HierarchyComponent>().RemoveChild(Entity{ SceneManagement::CurrentScene(), _entity });
		}
	}


	Entity HierarchyComponent::GetParent()
	{
		return Entity{ SceneManagement::CurrentScene(), _parent };
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


	void HierarchyComponent::Serialize(std::fstream& sceneFile)
	{
		if ( HasParent() ) {
			Entity parent = { SceneManagement::CurrentScene(), _parent };
			SerializeString(sceneFile, "parent", parent.GetUUID());
		}

		for ( auto& child : _childs ) {
			Entity currentChild = { SceneManagement::CurrentScene(), child };
			SerializeString(sceneFile, "child", currentChild.GetUUID());
		}
	}


	void HierarchyComponent::Deserialize(std::fstream& sceneFile)
	{
		std::string line;
		std::getline(sceneFile, line);

		if ( line.find("parent:") != std::string::npos ) {
			UUID parentUUID = line.substr( line.find_first_of(':') + 1, line.size() );
			Entity parentEntity = Entity{ SceneManagement::CurrentScene(), ECS::EntityFromUUID(parentUUID).GetName(), parentUUID };
			_parent = parentEntity.GetNativeEntity();
			parentEntity.AddChild(Entity{ SceneManagement::CurrentScene(), _entity });
		}
	}


	void HierarchyComponent::OnEditor()
	{
		ImGui::Text("Childs:");
		for ( auto& child : _childs ) {
			Entity currentChild = { SceneManagement::CurrentScene(), child };
			ImGui::TreeNodeEx(currentChild.GetUUID().c_str(), ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_SpanAvailWidth);
		}

		if ( HasParent() ) {
			Entity parentEntity = { SceneManagement::CurrentScene(), _parent };
			ImGui::Text("Parent: %s", parentEntity.GetUUID().c_str());
		}
	}

	bool HierarchyComponent::IsSerializable()
	{
		return HasChilds() || HasParent();
	}


}