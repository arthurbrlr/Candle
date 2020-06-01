#pragma once

#include <cdlpch.h>
#include "Candle/CandleCore.h"

namespace Candle {

	enum class EventType
	{
		None = 0,
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
		AppTick, AppUpdate, AppRender,
		KeyPressed, KeyReleased, KeyTyped,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
	};

	enum EventCategory
	{
		None = 0,
		EventCategoryApplication = BIT(0),
		EventCategoryInput		 = BIT(1),
		EventCategoryKeyboard	 = BIT(2),
		EventCategoryMouse		 = BIT(3),
		EventCategoryMouseButton = BIT(4)
	};

	#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::##type; }\
								   virtual EventType GetEventType() const override { return GetStaticType(); }\
								   virtual const char* GetName() const override { return #type; }

	#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override { return category; }

	#define BIND_EVENT_FUNCTION(x) std::bind(&x, this, std::placeholders::_1)

	class CANDLE_API Event {
		friend class EventDispatcher;

		public:
			virtual EventType GetEventType() const = 0;
			virtual const char* GetName() const = 0;
			virtual int GetCategoryFlags() const = 0;
			virtual std::string ToString() const { return GetName(); };

			inline bool IsInCategory(EventCategory category)
			{
				return GetCategoryFlags() & category;
			}

			inline bool IsHandled() const { return _handled; }

		protected:
			bool _handled = false;
	};


	class EventDispatcher {
		template<typename T>
		using EventFn = std::function<bool(T&)>;

		public:
			EventDispatcher(Event & event)
				: _event(event) {}

			template<typename T>
			bool Dispatch(EventFn<T> func)
			{
				if (_event.GetEventType() == T::GetStaticType()) {
					_event._handled = func(*(T*)&_event);
					return true;
				}
				return false;
			}

		private:
			Event& _event;
	};

	inline std::ostream& operator<<(std::ostream& os, const Event& e)
	{
		return os << e.ToString();
	}


}