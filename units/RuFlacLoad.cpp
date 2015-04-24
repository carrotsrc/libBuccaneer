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
#include "RuFlacLoad.h"
#define CHUNK_SIZE 0x100000
#include "events/ShellEvents.h"
using namespace RackoonIO;

RuFlacLoad::RuFlacLoad()
: RackUnit(std::string("RuFlacLoad")) {
	addJack("power", JACK_AC);
	addPlug("audio_out");

	MidiExport("pause", RuFlacLoad::midiPause);

	buffer = nullptr;
	workState = IDLE;
	psize = 512;

}

RackoonIO::FeedState RuFlacLoad::feed(RackoonIO::Jack*jack) {
	return FEED_OK;
}

void RuFlacLoad::setConfig(string config, string value) {
	if(config == "filename") {
		filename = value;
	}
}

void RuFlacLoad::actionNextChunk() {
	period = nullptr;

	while(period == nullptr)
		period = cacheAlloc(1);

	if(count < psize) psize = count;
	memcpy(period, position, psize*sizeof(PcmSample));
	count -= psize;
	position += psize;
	workState = STREAMING;
	notifyProcComplete();
}

void RuFlacLoad::actionLoadFile() {
	UnitMsg("Loading file " << filename);
	file = new SndfileHandle(filename.c_str());

	if(file->error() > 0) {
		UnitMsg("Error occured when loading file `" << filename << "` with error " << file->error());
		workState = ERROR;
		return;
	}


	bufSize = file->frames()<<1;
	count = bufSize;
	if(buffer != nullptr)
		free(buffer);

	buffer = (PcmSample*)calloc(bufSize, sizeof(PcmSample));
	position = buffer;

	while(file->read((float*)position, CHUNK_SIZE) == CHUNK_SIZE) {
		position += CHUNK_SIZE;
	}

	position = buffer;

	auto msg = createMessage(FlacLoadInit);
	if(msg == nullptr) {
		std::cerr << "Problem creating msg" << std::endl;
		exit(EXIT_FAILURE);
	}
	_FlacLoadInit(msg)->numFrames = count;
	addEvent(std::move(msg));

	if(onStateChange)
		onStateChange(PRESTREAM);
	workState = PRESTREAM;
	UnitMsg("Initialised");

	notifyProcComplete();
}

RackoonIO::RackState RuFlacLoad::init() {
	workState = LOADING;
	if(onStateChange)
		onStateChange(workState);
	ConcurrentTask(RuFlacLoad::actionLoadFile);
	EventListener(FramesFinalBuffer, RuFlacLoad::eventFinalBuffer);
	return RACK_UNIT_OK;
}

RackoonIO::RackState RuFlacLoad::cycle() {
	if(workState < READY)
		return RACK_UNIT_OK;
	
	Jack *jack = nullptr;

	switch(workState) {
	case ERROR:
		return RACK_UNIT_FAILURE;

	case READY:
		workState = LOADING_CHUNK;
		if(onStateChange)
			onStateChange(STREAMING);
		ConcurrentTask(RuFlacLoad::actionNextChunk);
		break;

	case STREAMING:
		jack = getPlug("audio_out")->jack;
		jack->frames = psize;
		if(jack->feed(period) == FEED_OK) {
			workState = LOADING_CHUNK;
			ConcurrentTask(RuFlacLoad::actionNextChunk);
		}
		break;

	case LOADING:
	case PAUSED:
	case LOADING_CHUNK:
	case PRESTREAM:
	case IDLE:
		break;
	}

	return RACK_UNIT_OK;
}

void RuFlacLoad::block(Jack *jack) {
	Jack *out = getPlug("audio_out")->jack;
	workState = PAUSED;
	if(onStateChange)
		onStateChange(workState);
	out->block();
}

void RuFlacLoad::midiPause(int code) {
	if(code == 127) {
		if(workState == STREAMING) {
			block(NULL);
		} else
		if(workState == PRESTREAM) {
			workState = READY;
			notifyProcComplete();
			if(onStateChange)
				onStateChange(workState);
		} else {
			workState = STREAMING;
			if(onStateChange)
				onStateChange(workState);
		}
	}
}

void RuFlacLoad::eventFinalBuffer(std::shared_ptr<EventMessage> msg) {
}


/* Exposed Unit */

void RuFlacLoad::setFilename(std::string fname) {
	filename = fname;
}

std::string RuFlacLoad::getFilename() {
	return filename;
}

void RuFlacLoad::cbStateChange(std::function<void(RuFlacLoad::WorkState)> cb) {
	onStateChange = cb;
}
