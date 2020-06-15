#pragma once

#include "Candle/CandleCore.h"
#include "Candle/Events/Event.h"

namespace Candle {

	class Blueprint;
	
	class CANDLE_API Script {

		public:
			virtual ~Script() {}

			virtual void OnAwake() {}
			virtual void OnUpdate() {}
			virtual void OnEvent(Event & event) {}
			virtual void OnEditor() {}
			virtual void OnDestroy() {}

			inline void AttachToBlueprint(Blueprint* parent) { _blueprint = parent; }
			inline Blueprint& GetParent() const { return *_blueprint; }

			inline const std::string& GetName() { return _name; }

		protected:
			std::string _name = "candleScript";
			Blueprint* _blueprint = nullptr;
			bool isAlive = true;

			/*
			template<typename T> 
			T& GetComponent() const
			{
				T& comp = _parent->GetComponent<T>();
				return comp;
			}
			*/
	};

}