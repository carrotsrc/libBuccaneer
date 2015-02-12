#include "framework/threads/RackQueue.h"
#include "framework/rack/Rack.h"
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
	rack.setRackUnitFactory(std::move(factory));
	rack.init();
	rack.initEvents(NUM_EVENTS);
	rack.start();
	    std::map<std::string, RackoonIO::RackUnit*> units;
	units = rack.getUnits();
	std::map<std::string, RackoonIO::RackUnit*>::iterator it;
	std::cout << "Loading this" << std::endl;
	for(it = units.begin(); it != units.end(); ++it)
	std::cout << (*it).first << std::endl;

	int x;
	cin >> x;
}




