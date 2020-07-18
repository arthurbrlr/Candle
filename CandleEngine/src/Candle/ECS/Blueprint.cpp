#include "cdlpch.h"
#include "Blueprint.h"
#include "BlueprintManager.h"

namespace Candle {

	size_t Blueprint::blueprintCount = 0L;

	Blueprint::Blueprint()
	{
		_blueprintID = blueprintCount++;
	}
	
	const void Blueprint::Awake() 
	{
		_isAwake = true;
	}

	const void Blueprint::Sleep()
	{
		_isAwake = false; 
	}

	const void Blueprint::Destroy()
	{
		_isAlive = false;
		BlueprintManager::Remove(_blueprintID);
		//ECS::ClearBlueprints();
		//if(HasParent()) {
		//	_parent->RemoveChild(_blueprintID);
		//}
	}
	
	bool Blueprint::HasComponentOfID(size_t id) const 
	{
		return _compBitset[id];
	}

	Component* Blueprint::GetComponentOfID(size_t id) const
	{
		CASSERT(HasComponentOfID(id), "Blueprint has no component of type T");
		auto compPointer = _compArray[id];
		return static_cast<Component*>( compPointer );
	}

	void Blueprint::AddChild(Blueprint* child)
	{
		_childs[child->GetID()] = child;
		_hasChildren = true;
		child->SetParent(this);
	}

	void Blueprint::RemoveChild(size_t childID)
	{
		if ( _childs.empty() ) return;
		try {
			_childs.erase(childID);
		} catch ( std::exception e ) {
			CDL_ERROR(e.what());
		}
		if ( _childs.size() == 0 ) _hasChildren = false;
	}

	void Blueprint::SetParent(Blueprint* parent)
	{
		_parent = parent;
	}
}