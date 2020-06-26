#pragma once

#include "Candle/CandleCore.h"

#include "Candle/Events/KeyEvent.h"
#include "Candle/Events/MouseEvent.h"

namespace Candle {

	class CANDLE_API Input {

		public:
			static void Init();
			static void PollEvents();
			
			static bool IsKeyPressed(int keycode);
			static bool OnKeyDown(int keycode);
			static bool OnKeyUp(int keycode);

			static bool IsMouseButtonPressed(int button);
			static bool OnMouseButtonDown(int button);
			static bool OnMouseButtonUp(int button);

			static double GetMouseX();
			static double GetMouseY();
			static std::pair<double, double> GetMousePos();
			static glm::vec4 GetMouseInEyeSpace();
			static glm::vec3 GetMouseInWorldSpace();

			static void RegisterKeyEvent(KeyPressedEvent& event);
			static void RegisterKeyEvent(KeyReleasedEvent& event);
			static void RegisterMouseButtonEvent(MouseButtonPressedEvent& event);
			static void RegisterMouseButtonEvent(MouseButtonReleasedEvent& event);

		private:
			struct KeyState {
				bool currrent = false;
				bool previous = false;
			};

			static Input* _instance;

			bool _inputRegistered = false;
			std::unordered_map<uint32_t, KeyState> _inputKeyStates;
			std::unordered_map<uint32_t, KeyState> _inputMouseStates;

			static glm::vec2 GetNormalizedDeviceCoords(double mx, double my);
			static glm::vec4 ClipToEyeSpace(glm::vec4& clipCoords);
			static glm::vec3 EyeToWorldSpace(glm::vec4& eyeCoords);

	};


}