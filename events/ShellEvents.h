/* Copyright 2015 Charlie Fyvie-Gauld
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published 
 *  by the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef ShellEvents_h
#define ShellEvents_h
#include "framework/events/EventMessage.h"

#define FlacLoadInit 0
#define FramesFinalBuffer 1


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
