#pragma once

#include "Event.h"

namespace Candle {

	class CANDLE_API WindowResizeEvent : public Event {

		public:
			WindowResizeEvent(unsigned int width, unsigned int height)
				: _width(width), _height(height) {}

			inline unsigned int GetWidth() const { return _width; }
			inline unsigned int GetHeight() const { return _height; }

			std::string ToString() const override
			{
				std::stringstream ss;
				ss << "WindowResizedEvent: " << _width << " / " << _height;
				return ss.str();
			}

			EVENT_CLASS_TYPE(WindowResize)
			EVENT_CLASS_CATEGORY(EventCategoryApplication)

		private:
			unsigned int _width, _height;
	};


	class CANDLE_API WindowClosedEvent : public Event {
		
		public:
			WindowClosedEvent() {}
			
			EVENT_CLASS_TYPE(WindowClose)
			EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};


	class CANDLE_API AppTickEvent : public Event {

		public:
			AppTickEvent() {}

			EVENT_CLASS_TYPE(AppTick)
			EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};


	class CANDLE_API AppUpdateEvent : public Event {

		public:
			AppUpdateEvent() {}

			EVENT_CLASS_TYPE(AppUpdate)
			EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};


	class CANDLE_API AppRenderEvent : public Event {

		public:
			AppRenderEvent() {}

			EVENT_CLASS_TYPE(AppRender)
			EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};


}