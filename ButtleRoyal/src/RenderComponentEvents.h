#pragma once

#include "IEvent.h"

#include "SpriteRenderComponent.h"

// RigidBodyComponent is a reference, because components are managed by
// game objects
class RenderableCreateEvent : public BasicDataEvent<IRenderComponent&>
{
public:
	RenderableCreateEvent(const std::string &layer, IRenderComponent& comp)
		: BasicDataEvent(comp)
		, m_layer(layer)
	{}

	// EventType defined using GUID functionality of VS 2015. 
	// Tools->Create GUID -> DEFINE_GUID -> take longest value in the beginning.
	// output:
	//
	// {DEC71435-8D92-4CA7-A3D5-FD17F9D5CC34}
	// DEFINE_GUID(<< name >> ,
	//	0xdec71435, 0x8d92, 0x4ca7, 0xa3, 0xd5, 0xfd, 0x17, 0xf9, 0xd5, 0xcc, 0x34);
	//
	// sk_EventType identifies event, when we want to add it to the event bus
	static const EventType sk_EventType;
	virtual EventType GetEventType() override { return sk_EventType; }

	std::string GetLayer() const { return m_layer;  }

private:
	std::string m_layer;
};

