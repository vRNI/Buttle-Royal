#pragma once

#include <memory>

class IEvent
{
public:
	IEvent() = default;
	virtual ~IEvent() = default;

	typedef unsigned long EventType;
	typedef std::shared_ptr<IEvent> Ptr;

	virtual EventType GetEventType() = 0;
};

template<typename T>
class BasicDataEvent : public IEvent
{
public:

	BasicDataEvent(T data)
		: m_data(data)
	{}
	virtual ~BasicDataEvent() = default;

	T GetData() { return m_data; }

private:
	T m_data;
};