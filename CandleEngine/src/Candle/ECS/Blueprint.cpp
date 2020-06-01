#include "cdlpch.h"
#include "Blueprint.h"

namespace Candle {

	long Blueprint::blueprintCount = 0L;

	Blueprint::Blueprint()
	{
		_blueprintID = (long)blueprintCount++;
	}
	
}