#pragma once

#include <Candle.h>
using namespace Candle;

class UUIDTest : public Script {

public:
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
			CDL_TRACE("{0}", NewUUID());
		}
	}


private:
	CANDLE_DECL_SCRIPT(UUIDTest)

};

CANDLE_SCRIPT(UUIDTest)