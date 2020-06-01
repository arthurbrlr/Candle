#pragma once

#include "Candle/Core/Input.h"

namespace Candle {

	class CANDLE_API WindowsInput : public Input {

		protected:
			void PollEventsImpl() override;

			bool IsKeyPressedImpl(int keycode) override;

			bool IsMouseButtonPressedImpl(int button) override;
			double GetMouseXImpl() override;
			double GetMouseYImpl() override;
			std::pair<double, double> GetMousePosImpl() override;

	};

}