#pragma once

#include "Event.h"

namespace Candle {

	class WindowResizeEvent : public Event {

		public:
			WindowResizeEvent(uint32_t width, uint32_t height)
				: _width(width), _height(height) {}

			inline uint32_t GetWidth() const { return _width; }
			inline uint32_t GetHeight() const { return _height; }

			std::string ToString() const override
			{
				std::stringstream ss;
				ss << "WindowResizedEvent: " << _width << " / " << _height;
				return ss.str();
			}

			EVENT_CLASS_TYPE(WindowResize)
			EVENT_CLASS_CATEGORY(EventCategoryApplication)

		private:
			uint32_t _width, _height;
	};


	class WindowClosedEvent : public Event {
		
		public:
			WindowClosedEvent() {}
			
			EVENT_CLASS_TYPE(WindowClose)
			EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};


	class AppTickEvent : public Event {

		public:
			AppTickEvent() {}

			EVENT_CLASS_TYPE(AppTick)
			EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};


	class AppUpdateEvent : public Event {

		public:
			AppUpdateEvent() {}

			EVENT_CLASS_TYPE(AppUpdate)
			EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};


	class AppRenderEvent : public Event {

		public:
			AppRenderEvent() {}

			EVENT_CLASS_TYPE(AppRender)
			EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};


}