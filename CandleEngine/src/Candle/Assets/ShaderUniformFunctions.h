#pragma once

#include "Candle/CandleCore.h"
#include "Candle/Core/Time.h"

namespace Candle {

	enum ShaderUniformFunctions {
		TIME, DELTATIME
	};


	static std::map<ShaderUniformFunctions, &&std::function>



}