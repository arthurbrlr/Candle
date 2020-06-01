#include "cdlpch.h"
#include "Time.h"

#include "platform/Windows/WindowsTime.h"

namespace Candle {


	TimeAPI* Time::_API = new WindowsTime();

}