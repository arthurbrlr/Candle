#pragma once

#include "Candle/CandleCore.h"
#include "Candle/Events/Event.h"

namespace Candle {
	class Application;

	class Layer {

		public:
			Layer(const std::string & name = "Layer");
			virtual ~Layer();

			virtual void OnAttach() {}
			virtual void OnDetach() {}
			virtual void OnUpdate() {}
			virtual void OnDraw() {}
			virtual void OnEvent(Event & e) {}


			void SetApp(Application* application) { _app = application; }

			inline const std::string & GetName() const { return _name; }

		protected:
			Application* _app;

		private:
			std::string _name;
	};

}