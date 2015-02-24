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

	midiExportMethod(string("pause"), std::bind(&RuFlacLoad::midiPause, this, std::placeholders::_1));

	buffer = nullptr;

	workState = IDLE;
	psize = 512;
}

RackoonIO::FeedState RuFlacLoad::feed(RackoonIO::Jack*jack) {
}

void RuFlacLoad::setConfig(string config, string value) {
	if(config == "filename") {
		filename = value;
	}
}

void RuFlacLoad::actionNextChunk() {
	period = cacheAlloc(1);

//	while(period == NULL) period = (short*)calloc(psize, sizeof(short));
	if(count < psize) psize = count;
	memcpy(period, position, psize<<1);
	count -= psize;
	position += psize;
	workState = STREAMING;
}

void RuFlacLoad::actionLoadFile() {
	CONSOLE_MSG("RuFlacLoad", "Loading file " << filename);
	file = new SndfileHandle(filename.c_str());

	if(file->error() > 0) {
		CONSOLE_MSG("RuFlacLoad", "Error occured when loading file `" << filename << "` with error " << file->error());
		workState = ERROR;
		return;
	}


	bufSize = file->frames()<<1;
	count = bufSize;
	if(buffer != nullptr)
		free(buffer);

	buffer = (short*)calloc(bufSize, sizeof(short));
	position = buffer;

	while(file->read(position, CHUNK_SIZE) == CHUNK_SIZE) {
		position += CHUNK_SIZE;
	}
	position = buffer;

	std::unique_ptr<EventMessage> msg = createMessage(FlacLoadInit);
	_FlacLoadInit(msg)->numFrames = count;
	addEvent(std::move(msg));

	if(onStateChange)
		onStateChange(PRESTREAM);
	workState = PRESTREAM;
	CONSOLE_MSG("RuFlacLoad", "Initialised");
}

RackoonIO::RackState RuFlacLoad::init() {
	workState = LOADING;
	if(onStateChange)
		onStateChange(workState);
	outsource(std::bind(&RuFlacLoad::actionLoadFile, this));
	addEventListener(FramesFinalBuffer, std::bind(&RuFlacLoad::eventFinalBuffer, this, std::placeholders::_1));
	return RACK_UNIT_OK;
}

RackoonIO::RackState RuFlacLoad::cycle() {
	if(workState < READY)
		return RACK_UNIT_OK;

	switch(workState) {
	case ERROR:
		return RACK_UNIT_FAILURE;

	case READY:
		workState = LOADING_CHUNK;
		if(onStateChange)
			onStateChange(STREAMING);
		outsource(std::bind(&RuFlacLoad::actionNextChunk, this));
		break;

	case STREAMING:
		Jack *jack = getPlug("audio_out")->jack;
		jack->frames = psize;
		if(jack->feed(period) == FEED_OK) {
			workState = LOADING_CHUNK;
			outsource(std::bind(&RuFlacLoad::actionNextChunk, this));
		}
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
