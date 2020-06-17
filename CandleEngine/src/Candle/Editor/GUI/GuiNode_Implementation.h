#pragma once

#include "Candle/CandleCore.h"
#include "imgui.h"

namespace Candle {

	class CANDLE_API GuiNode_Implementation {

		public:	
			GuiNode_Implementation(const ImVec2 & entryPoint, const ImVec2 & outputPoint, int width, const std::string & text, ImU32 color = IM_COL32(50, 70, 120, 255))
			{
				ImVec2 min = ImVec2(entryPoint.x, entryPoint.y - width / 2.f);
				ImVec2 max = ImVec2(outputPoint.x, entryPoint.y + width / 2.f);
				ImGui::GetWindowDrawList()->AddRectFilled(min, max, color, 5.f);

				ImGui::GetWindowDrawList()->AddCircleFilled(entryPoint, 1, IM_COL32(255, 255, 255, 255));
				ImGui::GetWindowDrawList()->AddCircleFilled(outputPoint, 1, IM_COL32(255, 255, 255, 255));

				ImGui::GetWindowDrawList()->AddText(ImGui::GetFont(), ImGui::GetFontSize(), ImVec2(entryPoint.x + 5, entryPoint.y - ImGui::GetFontSize() / 2.f), IM_COL32_WHITE, text.c_str());
			}
	};

}