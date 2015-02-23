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
#ifndef RUFLACLOAD_H
#define RUFLACLOAD_H
#include "framework/rack/RackUnit.h"
#include <sndfile.hh>
class RuFlacLoad : public RackoonIO::RackUnit
{
public:
	enum WorkState {
		IDLE,
		LOADING,
		READY,
		STREAMING,
		PAUSED,
		LOADING_CHUNK,
		PRESTREAM,

		ERROR,
	};

private:
	WorkState workState;

	SndfileHandle *file;
	sf_count_t bufSize, count;

	std::string filename;
	short *buffer, *position, *period;
	int psize;

	void feedOut();
	void actionLoadFile();
	void actionNextChunk();

	void eventFinalBuffer(std::shared_ptr<RackoonIO::EventMessage>);
	std::function<void(RuFlacLoad::WorkState)> onStateChange;

public:
	RuFlacLoad();
	RackoonIO::FeedState feed(RackoonIO::Jack*);
	void setConfig(string,string);

	RackoonIO::RackState init();
	RackoonIO::RackState cycle();
	void block(RackoonIO::Jack*);

	void midiPause(int);
	void midiLoad(int);

	/* Unit Exposure */
	void setFilename(std::string);
	std::string getFilename();
	void cbStateChange(std::function<void(RuFlacLoad::WorkState)>);
};
#endif
