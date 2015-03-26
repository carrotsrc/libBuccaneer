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
#ifndef RUECHO_H
#define RUECHO_H
#include "pconfig.h"
#include "framework/rack/RackUnit.h"
class RuEcho : public RackoonIO::RackUnit
{
public:
	enum WorkState {
		IDLE,
		READY,
		PRIMING,
		RUNNING,
		BYPASS
	};

private:
	FILE *fp;

	WorkState workState;
	unsigned int mDelay, sampleRate, bufSize, dLevel;
	float feedDecay;
	PcmSample *frameBuffer, *fDelay, *processedPeriod, *feedbackPeriod;
	void add(short*, int);
	bool remainder;

	void midiFeedbackDecay(int);
	std::function<void(int)> onDecayChange;
public:
	RuEcho();
	RackoonIO::FeedState feed(RackoonIO::Jack*);
	void setConfig(string,string);

	RackoonIO::RackState init();
	RackoonIO::RackState cycle();
	void block(RackoonIO::Jack*);
	void onDecayChangeCallback(std::function<void(int)>);
};

#endif
