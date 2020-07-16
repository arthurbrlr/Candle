#pragma once

#include "imgui.h"

#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "Candle/Events/AppEvent.h"
#include "Candle/Events/KeyEvent.h"
#include "Candle/Events/MouseEvent.h"

#include "Candle/Layers/Layer.h"

namespace Candle {

	class ImGuiLayer : public Layer	{

		public:
			ImGuiLayer();
			~ImGuiLayer() {};

			void OnAttach() override;
			void OnDetach() override;
			void OnUpdate() override {};
			void OnEvent(Event & event) override;

			void Begin();
			void End();

		private:
			double _time = 0;

	};

}