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
#include "framework/threads/RackQueue.h"
#include "framework/rack/Rack.h"
#include "framework/memory/BitfieldCache.h"
#include "factories/RackUnitFactory.h"
#include "factories/EventMessageFactory.h"

#include <chrono>

int main(int argc, char *argv[])
{
	std::string userConfig = "";
	if(argc > 0) {
		for(int i = 0; i < argc; i++) {
			if(strcmp(argv[i], "-c") == 0 && argc > 1)
				userConfig = std::string(argv[i+1]);
		}
	}
	RackoonIO::Rack rack;
	if(userConfig != "")
		rack.setConfigPath(userConfig);

	std::unique_ptr<RackoonIO::RackUnitGenericFactory> factory(new RackUnitFactory());
	
	factory->setMessageFactory(new EventMessageFactory);
	RackoonIO::CacheHandler *cache = new RackoonIO::BitfieldCache();
	cache->init(512, 150);
	factory->setCacheHandler(cache);

	rack.setRackUnitFactory(std::move(factory));
	rack.init();
	rack.initEvents(NUM_EVENTS);
	rack.start();
	    std::map<std::string, RackoonIO::RackUnit*> units;
	units = rack.getUnits();
	std::map<std::string, RackoonIO::RackUnit*>::iterator it;

	int x;
	cin >> x;
}




