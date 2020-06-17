#pragma once

#include "Event.h"

namespace Candle {

	class CANDLE_API MouseMovedEvent : public Event {
		public:
			MouseMovedEvent(double x, double y, double px, double py)
				: _mouseX(x), _mouseY(y), _pMouseX(px), _pMouseY(py) {}

			inline double GetX() const { return _mouseX; }
			inline double GetY() const { return _mouseY; }
			inline double GetPX() const { return _pMouseX; }
			inline double GetPY() const { return _pMouseY; }

			std::string ToString() const override
			{
				std::stringstream ss;
				ss << "MouseMovedEvent: " << _mouseX << " / " << _mouseY;
				return ss.str();
			}

			EVENT_CLASS_TYPE(MouseMoved)
			EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)

		private:
			double _mouseX, _mouseY;
			double _pMouseX, _pMouseY;
	};


	class CANDLE_API MouseScrolledEvent : public Event {
		public:
			MouseScrolledEvent(double xOffset, double yOffset)
				: _xOffset(xOffset), _yOffset(yOffset) {}

			inline double GetXOffset() const { return _xOffset; }
			inline double GetYOffset() const { return _yOffset; }

			std::string ToString() const override
			{
				std::stringstream ss;
				ss << "MouseScrolledEvent: " << _xOffset << " / " << _yOffset;
				return ss.str();
			}

			EVENT_CLASS_TYPE(MouseScrolled)
			EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)

		private:
			double _xOffset, _yOffset;
	};


	class CANDLE_API MouseButtonEvent : public Event {
		public:
			inline int GetMouseButton() const { return _button; }

			EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)

		protected:
			MouseButtonEvent(int button)
				: _button(button) {}

			int _button;
	};


	class CANDLE_API MouseButtonPressedEvent : public MouseButtonEvent {
		public:
			MouseButtonPressedEvent(int button)
				: MouseButtonEvent(button) {}

			std::string ToString() const override
			{
				std::stringstream ss;
				ss << "MouseButtonPressedEvent : " << _button;
				return ss.str();
			}

			EVENT_CLASS_TYPE(MouseButtonPressed)
	};


	class CANDLE_API MouseButtonReleasedEvent : public MouseButtonEvent {
	public:
		MouseButtonReleasedEvent(int button)
			: MouseButtonEvent(button) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseButtonReleasedEvent : " << _button;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseButtonReleased)
	};

}
