#pragma once

#include "Candle/CandleCore.h"
#include "Candle/Core/Input.h"

#include "Candle/Core/Controllers.h"

#include "imgui.h"
#include "GuiNode_Implementation.h"

namespace Candle {

	enum GuiElementState {
		NONE, HOVERED, L_CLICK, R_CLICK
	};

	class Gui {

		public:
			static GuiElementState GuiNode(const ImVec2& mousePosition, const ImVec2& entryPoint, const ImVec2& outputPoint, int width, const std::string& text, ImU32 color = IM_COL32(50, 70, 120, 255))
			{
				ImVec2 min = ImVec2(entryPoint.x, entryPoint.y - width / 2.f);
				ImVec2 max = ImVec2(outputPoint.x, entryPoint.y + width / 2.f);

				bool valid_x = mousePosition.x > min.x && mousePosition.x < max.x;
				bool valid_y = mousePosition.y > min.y && mousePosition.y < max.y;

				if ( valid_x && valid_y ) {
					if ( Input::OnMouseButtonDown(CDL_MOUSE_BUTTON_1) ) {
						GuiNode_Implementation(entryPoint, outputPoint, width, text, color);
						return GuiElementState::L_CLICK;
					} else if ( Input::OnMouseButtonDown(CDL_MOUSE_BUTTON_2) ) {
						GuiNode_Implementation(entryPoint, outputPoint, width, text, color);
						return GuiElementState::R_CLICK;
					} else {
						GuiNode_Implementation(entryPoint, outputPoint, width, text, color + IM_COL32(20, 20, 20, 0.));
						return GuiElementState::HOVERED;
					}
				}

				GuiNode_Implementation(entryPoint, outputPoint, width, text, color);
				return GuiElementState::NONE;
			}

	};

}