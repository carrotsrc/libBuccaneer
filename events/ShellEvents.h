#ifndef ShellEvents_h
#define ShellEvents_h
#include "framework/events/EventMessage.h"

#define FlacLoadInit 1
#define FramesFinalBuffer 2


#define NUM_EVENTS 4

class EvFramesFinalBuffer : public RackoonIO::EventMessage {
public:
	~EvFramesFinalBuffer(){ free(frames); };

	short *frames;
	int numFrames;
};

class EvFlacLoadInit : public RackoonIO::EventMessage {
public:
	int numFrames;
};

#define FFB(s_ptr) (((EvFramesFinalBuffer*)(s_ptr.get())))
#define _FlacLoadInit(s_ptr) (((EvFlacLoadInit*)(s_ptr.get())))
#endif
