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
#include "RuLevels.h"

using namespace RackoonIO;
RuLevels::RuLevels()
: RackUnit(std::string("RuLevels")) {
	addJack("audio", JACK_SEQ);
	addPlug("audio_out");
	masterGain = 0.5;
	fadeGain = 1.0;
	processed = false;

	MIDI_BIND("masterGain", RuLevels::midiMasterGain);
	MIDI_BIND("fadeGain", RuLevels::midiFadeGain);
}

void RuLevels::writeDebugPCM(short value) {
	short *d = (short*)calloc(256, sizeof(short));
	for(int i = 0; i < 256; i++)
		d[i] = value;
	free(d);
}

FeedState RuLevels::feed(Jack *jack) {
	short *period = NULL;
	Jack *out = getPlug("audio_out")->jack;
	out->frames = jack->frames;
	if(jack->flush(&period) == FEED_BLOCKED) {

	}
	FeedState fState;

	if( !processed ) {
		for(int i = 0; i < jack->frames; i++)
			period[i] = period[i] * masterGain * fadeGain;

		processed = true;
	}

	if((fState = out->feed(period)) == FEED_OK)
		processed = false;

	return fState;
}
RackState RuLevels::init() {
	workState = READY;
	cout << "RuLevels: Initialised" << endl;
	return RACK_UNIT_OK;
}

RackState RuLevels::cycle() {
	return RACK_UNIT_OK;
}

void RuLevels::setConfig(string config, string value) {

}

void RuLevels::block(Jack *jack) {
	Jack *out = getPlug("audio_out")->jack;
	out->block();
}

void RuLevels::midiMasterGain(int value) {
	masterGain = ((0.78*value)/100);
	if(onGainChange)
		onGainChange(value);
}

void RuLevels::midiFadeGain(int value) {
	fadeGain = ((0.78*value)/100);
	if(onFadeChange)
		onFadeChange(value);
}

void RuLevels::cbFadeChange(std::function<void(int)> cb) {
	onFadeChange = cb;
}

void RuLevels::cbGainChange(std::function<void(int)> cb) {
	onGainChange = cb;
}
