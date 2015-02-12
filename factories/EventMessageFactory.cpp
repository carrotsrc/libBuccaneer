#include "EventMessageFactory.h"

using namespace RackoonIO;

std::unique_ptr<EventMessage> EventMessageFactory::createMessage(EventType type) {
	EventMessage *msg = nullptr;
	switch(type) {
	case FramesFinalBuffer:
		msg = (EventMessage*) new EvFramesFinalBuffer; 
		break;
	case FlacLoadInit:
		msg = (EventMessage*) new EvFlacLoadInit;
		break;
	}

	msg->msgType = type;
	return std::unique_ptr<EventMessage>(msg);
}
