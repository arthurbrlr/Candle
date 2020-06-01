#pragma once

#include "Candle/CandleCore.h"
#include "Candle/Events/Event.h"

namespace Candle {
	class CANDLE_API Application;

	class CANDLE_API Layer {

		public:
			Layer(const std::string & name = "Layer");
			virtual ~Layer();

			virtual void OnAttach() {}
			virtual void OnDetach() {}
			virtual void OnUpdate() {}
			virtual void OnEvent(Event & e) {}

			virtual void OnEditor() {}

			void SetApp(Application* application) { _app = application; }

			inline const std::string & GetName() const { return _name; }

		protected:
			Application* _app;

		private:
			std::string _name;
	};

}