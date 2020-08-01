#pragma once

#include <Candle.h>
using namespace Candle;

class UUIDTest : public Script {

public:
	UUIDTest()
	{
		_name = __FILE__;
	}


	void OnAwake() override
	{
	}

	void OnUpdate() override
	{
	}


	void OnEvent(Event& event) override
	{
	}


	void OnEditor() override
	{
		if ( ImGui::Button("New UUID") ) {
			CDL_TRACE("{0}", Noise::UUID());
		}
	}


private:

};

AUTO_REGISTER_SCRIPT(UUIDTest)