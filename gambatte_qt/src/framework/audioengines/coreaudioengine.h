/***************************************************************************
 *   Copyright (C) 2008 by Sindre Aamås                                    *
 *   aamas@stud.ntnu.no                                                    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License version 2 as     *
 *   published by the Free Software Foundation.                            *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License version 2 for more details.                *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   version 2 along with this program; if not, write to the               *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#ifndef CORE_AUDIO_ENGINE_H
#define CORE_AUDIO_ENGINE_H

#include "../audioengine.h"
#include <ringbuffer.h>
#include <AudioUnit/AudioUnit.h>
#include <pthread.h>

class CoreAudioEngine : public AudioEngine {
	enum { UNIT_CLOSED = 0, UNIT_OPENED, UNIT_INITED };

	RingBuffer<SInt16> rbuf;
	AudioUnit outUnit;
	int outUnitState;
	pthread_mutex_t *mutex;
	pthread_cond_t *availCond;
	Float64 rateEst;
	Float64 rateVar;
	bool running;
	
	static ComponentResult renderProc(void *inRefCon, AudioUnitRenderActionFlags *inActionFlags, const AudioTimeStamp *inTimeStamp,
							   UInt32 inBusNumber, UInt32 inNumFrames, AudioBufferList *ioData);
	unsigned read(void *stream, unsigned frames, Float64 rateScalar);
	int doInit(int rate, unsigned latency);
	int doWrite(void *buffer, unsigned frames);
	
public:
	CoreAudioEngine();
	~CoreAudioEngine();
	void uninit();
	int write(void *buffer, unsigned frames);
	int write(void *buffer, unsigned samples, BufferState &preBufState_out, RateEst::Result &rate_out);
	const BufferState bufferState() const;
	const RateEst::Result rateEstimate() const;
	void pause();
};

#endif
