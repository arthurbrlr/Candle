#include "cdlpch.h"
#include "Blueprint.h"

namespace Candle {

	size_t Blueprint::blueprintCount = 0L;

	Blueprint::Blueprint()
	{
		_blueprintID = blueprintCount++;
	}
	
}