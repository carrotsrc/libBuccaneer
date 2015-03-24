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
#include "factories/EventMessageFactory.h"
#include "events/ShellEvents.h"
#include "framework/events/EventLoop.h"

#include <chrono>
static bool running;

void clientEventCallback(std::shared_ptr<RackoonIO::EventMessage> msg) {
	std::cout << "Callback: ";
	std::cout << msg->msgType << std::endl;
	running = false;
}
int main(int argc, char *argv[])
{
	EventMessageFactory factory;
	RackoonIO::EventLoop loop;

	loop.initEvents(4);
	loop.addEventListener(FlacLoadInit, std::bind(&clientEventCallback, std::placeholders::_1));
	loop.start();
	running = true;
	auto msg = factory.createMessage(FlacLoadInit);
	loop.addEvent(std::move(msg));
	while(running) continue;

	loop.stop();
	return 0;
}




