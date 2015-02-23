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
