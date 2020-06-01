#pragma once

#include "Candle/CandleCore.h"

#include "Candle/Events/KeyEvent.h"
#include "Candle/Events/MouseEvent.h"

#include "Keycodes.h"
#include "Controllers.h"

namespace Candle {

	class CANDLE_API Input {

		public:
			static void Init();
			static void PollEvents();
			
			inline static bool IsKeyPressed(int keycode) { return _instance->IsKeyPressedImpl(keycode); }
			inline static bool OnKeyDown(int keycode) { return _instance->_inputKeyStates[keycode].currrent && !_instance->_inputKeyStates[keycode].previous; }
			inline static bool OnKeyUp(int keycode) { return !_instance->_inputKeyStates[keycode].currrent && _instance->_inputKeyStates[keycode].previous; }

			inline static bool IsMouseButtonPressed(int button) { return _instance->IsMouseButtonPressedImpl(button); }
			inline static bool OnMouseButtonDown(int button) { return _instance->_inputMouseStates[button].currrent && !_instance->_inputMouseStates[button].previous; }
			inline static bool OnMouseButtonUp(int button) { return !_instance->_inputMouseStates[button].currrent && _instance->_inputMouseStates[button].previous; }

			inline static double GetMouseX() { return _instance->GetMouseXImpl(); }
			inline static double GetMouseY() { return _instance->GetMouseYImpl(); }
			inline static std::pair<double, double> GetMousePos() { return _instance->GetMousePosImpl(); }

			static void RegisterKeyEvent(KeyPressedEvent& event);
			static void RegisterKeyEvent(KeyReleasedEvent& event);
			static void RegisterMouseButtonEvent(MouseButtonPressedEvent& event);
			static void RegisterMouseButtonEvent(MouseButtonReleasedEvent& event);

		protected:
			virtual void PollEventsImpl() = 0;

			virtual bool IsKeyPressedImpl(int keycode) = 0;

			virtual bool IsMouseButtonPressedImpl(int button) = 0;
			virtual double GetMouseXImpl() = 0;
			virtual double GetMouseYImpl() = 0;
			virtual std::pair<double, double> GetMousePosImpl() = 0;

		private:
			struct KeyState {
				bool currrent = false;
				bool previous = false;
			};

			static Input* _instance;

			bool _inputRegistered = false;
			std::unordered_map<unsigned int, KeyState> _inputKeyStates;
			std::unordered_map<unsigned int, KeyState> _inputMouseStates;

	};


}