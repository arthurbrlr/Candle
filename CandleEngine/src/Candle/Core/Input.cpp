#include "cdlpch.h"
#include "Input.h"

namespace Candle {


	void Input::Init()
	{

			/* Alphanumerical keys */
		_instance->_inputKeyStates[CDL_KEY_SPACE] = { false, false };
		_instance->_inputKeyStates[CDL_KEY_APOSTROPHE] = { false, false };
		_instance->_inputKeyStates[CDL_KEY_COMMA] = { false, false };
		_instance->_inputKeyStates[CDL_KEY_MINUS] = { false, false };
		_instance->_inputKeyStates[CDL_KEY_PERIOD] = { false, false };
		_instance->_inputKeyStates[CDL_KEY_SLASH] = { false, false };
		_instance->_inputKeyStates[CDL_KEY_0] = { false, false };
		_instance->_inputKeyStates[CDL_KEY_1] = { false, false };
		_instance->_inputKeyStates[CDL_KEY_2] = { false, false };
		_instance->_inputKeyStates[CDL_KEY_3] = { false, false };
		_instance->_inputKeyStates[CDL_KEY_4] = { false, false };
		_instance->_inputKeyStates[CDL_KEY_5] = { false, false };
		_instance->_inputKeyStates[CDL_KEY_6] = { false, false };
		_instance->_inputKeyStates[CDL_KEY_7] = { false, false };
		_instance->_inputKeyStates[CDL_KEY_8] = { false, false };
		_instance->_inputKeyStates[CDL_KEY_9] = { false, false };
		_instance->_inputKeyStates[CDL_KEY_SEMICOLON] = { false, false };
		_instance->_inputKeyStates[CDL_KEY_EQUAL] = { false, false };
		_instance->_inputKeyStates[CDL_KEY_A] = { false, false };
		_instance->_inputKeyStates[CDL_KEY_B] = { false, false };
		_instance->_inputKeyStates[CDL_KEY_C] = { false, false };
		_instance->_inputKeyStates[CDL_KEY_D] = { false, false };
		_instance->_inputKeyStates[CDL_KEY_E] = { false, false };
		_instance->_inputKeyStates[CDL_KEY_F] = { false, false };
		_instance->_inputKeyStates[CDL_KEY_G] = { false, false };
		_instance->_inputKeyStates[CDL_KEY_H] = { false, false };
		_instance->_inputKeyStates[CDL_KEY_I] = { false, false };
		_instance->_inputKeyStates[CDL_KEY_J] = { false, false };
		_instance->_inputKeyStates[CDL_KEY_K] = { false, false };
		_instance->_inputKeyStates[CDL_KEY_L] = { false, false };
		_instance->_inputKeyStates[CDL_KEY_M] = { false, false };
		_instance->_inputKeyStates[CDL_KEY_N] = { false, false };
		_instance->_inputKeyStates[CDL_KEY_O] = { false, false };
		_instance->_inputKeyStates[CDL_KEY_P] = { false, false };
		_instance->_inputKeyStates[CDL_KEY_Q] = { false, false };
		_instance->_inputKeyStates[CDL_KEY_R] = { false, false };
		_instance->_inputKeyStates[CDL_KEY_S] = { false, false };
		_instance->_inputKeyStates[CDL_KEY_T] = { false, false };
		_instance->_inputKeyStates[CDL_KEY_U] = { false, false };
		_instance->_inputKeyStates[CDL_KEY_V] = { false, false };
		_instance->_inputKeyStates[CDL_KEY_W] = { false, false };
		_instance->_inputKeyStates[CDL_KEY_X] = { false, false };
		_instance->_inputKeyStates[CDL_KEY_Y] = { false, false };
		_instance->_inputKeyStates[CDL_KEY_Z] = { false, false };
		_instance->_inputKeyStates[CDL_KEY_LEFT_BRACKET] = { false, false };
		_instance->_inputKeyStates[CDL_KEY_BACKSLASH] = { false, false };
		_instance->_inputKeyStates[CDL_KEY_RIGHT_BRACKET] = { false, false };
		_instance->_inputKeyStates[CDL_KEY_GRAVE_ACCENT] = { false, false };
		_instance->_inputKeyStates[CDL_KEY_WORLD_1] = { false, false };
		_instance->_inputKeyStates[CDL_KEY_WORLD_2] = { false, false };

			/* Special Keyboard keys */
		_instance->_inputKeyStates[CDL_KEY_ESCAPE] = { false, false };
		_instance->_inputKeyStates[CDL_KEY_ENTER] = { false, false };
		_instance->_inputKeyStates[CDL_KEY_TAB] = { false, false };
		_instance->_inputKeyStates[CDL_KEY_BACKSPACE] = { false, false };
		_instance->_inputKeyStates[CDL_KEY_INSERT] = { false, false };
		_instance->_inputKeyStates[CDL_KEY_DELETE] = { false, false };
		_instance->_inputKeyStates[CDL_KEY_RIGHT] = { false, false };
		_instance->_inputKeyStates[CDL_KEY_LEFT] = { false, false };
		_instance->_inputKeyStates[CDL_KEY_DOWN] = { false, false };
		_instance->_inputKeyStates[CDL_KEY_UP] = { false, false };
		_instance->_inputKeyStates[CDL_KEY_PAGE_UP] = { false, false };
		_instance->_inputKeyStates[CDL_KEY_PAGE_DOWN] = { false, false };
		_instance->_inputKeyStates[CDL_KEY_HOME] = { false, false };
		_instance->_inputKeyStates[CDL_KEY_END] = { false, false };
		_instance->_inputKeyStates[CDL_KEY_CAPS_LOCK] = { false, false };
		_instance->_inputKeyStates[CDL_KEY_SCROLL_LOCK] = { false, false };
		_instance->_inputKeyStates[CDL_KEY_NUM_LOCK] = { false, false };
		_instance->_inputKeyStates[CDL_KEY_PRINT_SCREEN] = { false, false };
		_instance->_inputKeyStates[CDL_KEY_PAUSE] = { false, false };
		_instance->_inputKeyStates[CDL_KEY_F1] = { false, false };
		_instance->_inputKeyStates[CDL_KEY_F2] = { false, false };
		_instance->_inputKeyStates[CDL_KEY_F3] = { false, false };
		_instance->_inputKeyStates[CDL_KEY_F4] = { false, false };
		_instance->_inputKeyStates[CDL_KEY_F5] = { false, false };
		_instance->_inputKeyStates[CDL_KEY_F6] = { false, false };
		_instance->_inputKeyStates[CDL_KEY_F7] = { false, false };
		_instance->_inputKeyStates[CDL_KEY_F8] = { false, false };
		_instance->_inputKeyStates[CDL_KEY_F9] = { false, false };
		_instance->_inputKeyStates[CDL_KEY_F10] = { false, false };
		_instance->_inputKeyStates[CDL_KEY_F11] = { false, false };
		_instance->_inputKeyStates[CDL_KEY_F12] = { false, false };
		_instance->_inputKeyStates[CDL_KEY_F13] = { false, false };
		_instance->_inputKeyStates[CDL_KEY_F14] = { false, false };
		_instance->_inputKeyStates[CDL_KEY_F15] = { false, false };
		_instance->_inputKeyStates[CDL_KEY_F16] = { false, false };
		_instance->_inputKeyStates[CDL_KEY_F17] = { false, false };
		_instance->_inputKeyStates[CDL_KEY_F18] = { false, false };
		_instance->_inputKeyStates[CDL_KEY_F19] = { false, false };
		_instance->_inputKeyStates[CDL_KEY_F20] = { false, false };
		_instance->_inputKeyStates[CDL_KEY_F21] = { false, false };
		_instance->_inputKeyStates[CDL_KEY_F22] = { false, false };
		_instance->_inputKeyStates[CDL_KEY_F23] = { false, false };
		_instance->_inputKeyStates[CDL_KEY_F24] = { false, false };
		_instance->_inputKeyStates[CDL_KEY_F25] = { false, false };
		_instance->_inputKeyStates[CDL_KEY_KP_0] = { false, false };
		_instance->_inputKeyStates[CDL_KEY_KP_1] = { false, false };
		_instance->_inputKeyStates[CDL_KEY_KP_2] = { false, false };
		_instance->_inputKeyStates[CDL_KEY_KP_3] = { false, false };
		_instance->_inputKeyStates[CDL_KEY_KP_4] = { false, false };
		_instance->_inputKeyStates[CDL_KEY_KP_5] = { false, false };
		_instance->_inputKeyStates[CDL_KEY_KP_6] = { false, false };
		_instance->_inputKeyStates[CDL_KEY_KP_7] = { false, false };
		_instance->_inputKeyStates[CDL_KEY_KP_8] = { false, false };
		_instance->_inputKeyStates[CDL_KEY_KP_9] = { false, false };
		_instance->_inputKeyStates[CDL_KEY_KP_DECIMAL] = { false, false };
		_instance->_inputKeyStates[CDL_KEY_KP_DIVIDE] = { false, false };
		_instance->_inputKeyStates[CDL_KEY_KP_MULTIPLY] = { false, false };
		_instance->_inputKeyStates[CDL_KEY_KP_SUBTRACT] = { false, false };
		_instance->_inputKeyStates[CDL_KEY_KP_ADD] = { false, false };
		_instance->_inputKeyStates[CDL_KEY_KP_ENTER] = { false, false };
		_instance->_inputKeyStates[CDL_KEY_KP_EQUAL] = { false, false };
		_instance->_inputKeyStates[CDL_KEY_LEFT_SHIFT] = { false, false };
		_instance->_inputKeyStates[CDL_KEY_LEFT_CONTROL] = { false, false };
		_instance->_inputKeyStates[CDL_KEY_LEFT_ALT] = { false, false };
		_instance->_inputKeyStates[CDL_KEY_LEFT_SUPER] = { false, false };
		_instance->_inputKeyStates[CDL_KEY_RIGHT_SHIFT] = { false, false };
		_instance->_inputKeyStates[CDL_KEY_RIGHT_CONTROL] = { false, false };
		_instance->_inputKeyStates[CDL_KEY_RIGHT_ALT] = { false, false };
		_instance->_inputKeyStates[CDL_KEY_RIGHT_SUPER] = { false, false };
		_instance->_inputKeyStates[CDL_KEY_MENU] = { false, false };

			/* Mouse Buttons */
		_instance->_inputMouseStates[CDL_MOUSE_BUTTON_1] = { false, false };
		_instance->_inputMouseStates[CDL_MOUSE_BUTTON_2] = { false, false };
		_instance->_inputMouseStates[CDL_MOUSE_BUTTON_3] = { false, false };
		_instance->_inputMouseStates[CDL_MOUSE_BUTTON_4] = { false, false };
		_instance->_inputMouseStates[CDL_MOUSE_BUTTON_5] = { false, false };
		_instance->_inputMouseStates[CDL_MOUSE_BUTTON_6] = { false, false };
		_instance->_inputMouseStates[CDL_MOUSE_BUTTON_7] = { false, false };
		_instance->_inputMouseStates[CDL_MOUSE_BUTTON_8] = { false, false };
	}


	void Input::PollEvents()
	{
		if ( _instance->_inputRegistered ) {

			for ( auto& key : _instance->_inputKeyStates ) {

				key.second.previous = key.second.currrent;

			}

			for ( auto& button : _instance->_inputMouseStates ) {

				button.second.previous = button.second.currrent;

			}

		}

		_instance->_inputRegistered = false;
		_instance->PollEventsImpl();
	}


	void Input::RegisterKeyEvent(KeyPressedEvent& event)
	{
		_instance->_inputKeyStates[event.GetKeyCode()].currrent = true;
		
		_instance->_inputRegistered = true;
	}


	void Input::RegisterKeyEvent(KeyReleasedEvent& event)
	{
		_instance->_inputKeyStates[event.GetKeyCode()].currrent = false;

		_instance->_inputRegistered = true;
	}
	
	
	void Input::RegisterMouseButtonEvent(MouseButtonPressedEvent& event)
	{
		_instance->_inputMouseStates[event.GetMouseButton()].currrent = true;
		_instance->_inputMouseStates[event.GetMouseButton()].previous = false;

		_instance->_inputRegistered = true;
	}
	
	
	void Input::RegisterMouseButtonEvent(MouseButtonReleasedEvent& event)
	{
		_instance->_inputMouseStates[event.GetMouseButton()].currrent = false;
		_instance->_inputMouseStates[event.GetMouseButton()].previous = true;

		_instance->_inputRegistered = true;
	}


}