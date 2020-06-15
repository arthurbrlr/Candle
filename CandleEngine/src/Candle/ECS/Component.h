#pragma once

#include "Candle/CandleCore.h"
#include "Candle/Events/Event.h"

#include "imgui.h"

#include <type_traits>
#include <bitset>

namespace Candle {

	class Component;
	class Blueprint;
	typedef std::size_t ComponentID;

	inline ComponentID GetUniqueComponentID()
	{
		static ComponentID lastID{ 0u };
		return lastID++;
	}


	template<typename T>
	inline ComponentID GetComponentID()
	{
		bool isCompat = std::is_base_of<Component, T>::value;
		CASSERT(isCompat, "Template mush inherit from Component");

		static ComponentID typeID = GetUniqueComponentID();
		return typeID;
	}

	constexpr std::size_t MaxComponents{ 32 };
	typedef std::bitset<MaxComponents> ComponentBitSet;
	typedef std::array<Component*, MaxComponents> ComponentArray;

	class CANDLE_API Component {

		public:
			virtual ~Component() {}

			virtual void OnEditor() {};

			inline void AttachToBlueprint(Blueprint* parent) { _blueprint = parent; }
			inline Blueprint& GetParent() const { return *_blueprint; }

			inline const std::string & GetName() { return _name; }
			inline bool & IsActive() { return _isActive; }

			inline void SetActive(bool state) { _isActive = state; }

		protected:
			std::string _name = "candleComponent";
			Blueprint* _blueprint = nullptr;
			bool _isActive = true;
	};

}