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
#include "RuLoop.h"

using namespace RackoonIO;
#include "events/ShellEvents.h"

RuLoop::RuLoop()
: RackUnit(std::string("RuLoop")) {
	addJack("audio", JACK_SEQ);
	addPlug("audio_out");
	workState = IDLE;
	sampleRate = 44100;
	loopBuffer = nullptr;
	MidiExport("toggleRecord", RuLoop::midiToggleRecord);
	MidiExport("toggleLoop", RuLoop::midiToggleLoop);
}

void RuLoop::bufferRealloc() {
	// add another second
	PcmSample *tmp = NULL;
	int offset = writePos - loopBuffer;
	while(tmp == NULL)
		tmp = (PcmSample*) realloc(loopBuffer, sizeof(PcmSample) * (loopCapacity + (sampleRate<<1)));

	loopBuffer = tmp;
	writePos = loopBuffer + offset;
	loopLength = writePos;
	loopCapacity = loopCapacity + (sampleRate<<1);
	UnitMsg("Reallocated buffer to " << loopCapacity);
}

void RuLoop::feedLoop() {
	int bufSize = frames;
	readPos = loopBuffer;

	if(readPos + frames > loopLength)
		bufSize = (loopLength - readPos);

	PcmSample *period = cacheAlloc(1);
	memcpy(period, readPos, bufSize*sizeof(PcmSample));

	if(bufSize < frames) {
		readPos = loopBuffer;
		memcpy(period+bufSize, readPos, (frames-bufSize)*sizeof(PcmSample));
		readPos += (frames-bufSize);
	}
	Jack *out = getPlug("audio_out")->jack;
	UnitMsg("Length: " << loopLength-loopBuffer);
	while(workState == LOOPING) {

		if(out->feed(period) == FEED_OK) {
			readPos += bufSize;
			bufSize = frames;
			if(readPos + frames > loopLength)
				bufSize = (loopLength - readPos);

			//period = (short*)malloc(frames*sizeof(short));
			period = cacheAlloc(1);
			memcpy(period, readPos, bufSize*sizeof(PcmSample));

			if(bufSize < frames) {
				readPos = loopBuffer;
				memcpy(period+bufSize, readPos, (frames-bufSize)*sizeof(PcmSample));
				readPos += (frames-bufSize);
			}
		}
	}
}

FeedState RuLoop::feed(Jack *jack) {
	if(workState == LOOPING)
		return FEED_WAIT;

	PcmSample *period = NULL;
	Jack *out = getPlug("audio_out")->jack;
	jack->flush(&period);
	out->frames = jack->frames;
	frames = jack->frames;

	return out->feed(period);
	return FEED_OK;
}

RackState RuLoop::init() {
	workState = PASSTHROUGH;
	EventListener(FramesFinalBuffer, RuLoop::eventFinalBuffer);
	UnitMsg("Initialised");
	return RACK_UNIT_OK;
}

RackState RuLoop::cycle() {
	return RACK_UNIT_OK;
}

void RuLoop::setConfig(string config, string value) {

}

void RuLoop::block(Jack *jack) {
	Jack *out = getPlug("audio_out")->jack;
	out->block();
}

void RuLoop::midiToggleRecord(int value) {
	if(value < 127)
		return;

	if(workState == READY || workState == PASSTHROUGH) {
		if(loopBuffer == nullptr) {
			// start with a two second loop (sampleRate * 2 channels * 2 seconds)
			loopBuffer = (PcmSample*) malloc( (sampleRate<<2) * sizeof(PcmSample));
			loopCapacity = sampleRate<<2;
			loopLength = readPos = writePos = loopBuffer;
		} else {
			loopLength = readPos = writePos = loopBuffer;
		}
		workState = PRIMING;
		UnitMsg("Priming");
	}
	else
	if(workState == PRIMING) {
		workState = READY;
		UnitMsg("Primed and Ready");
	}
}

void RuLoop::midiToggleLoop(int value) {
	if(value < 127)
		return;

	if(workState == READY) {
		workState = LOOPING;
		UnitMsg("Looping");
	}
	else if(workState == LOOPING) {
		workState = READY;
		UnitMsg("Ready");
	} else
	if(workState == PRIMING) {
		workState = LOOPING;
		UnitMsg("Straight into READY");
		ConcurrentTask(RuLoop::feedLoop);
	}
}

void RuLoop::eventFinalBuffer(std::shared_ptr<EventMessage> msg) {
	if(workState == PRIMING) {
		if( ((loopLength-loopBuffer) + FFB(msg)->numFrames) > loopCapacity)
			bufferRealloc();


		memcpy(writePos, FFB(msg)->frames, sizeof(PcmSample)*FFB(msg)->numFrames);
		loopLength += FFB(msg)->numFrames;
		writePos += FFB(msg)->numFrames;
	}
}
