/*
 * SoundDataInOut.h
 *
 *  Created on: Jan 24, 2021
 *      Author: private
 */

#ifndef SRC_SOUNDDATAINOUT_H_
#define SRC_SOUNDDATAINOUT_H_
#include "SoundData.h"

class SoundDataInOut {

public:
	SoundDataInOut();
	virtual ~SoundDataInOut();
	SoundData* getSoundDataIn();
	SoundData* getSoundDataOut();
	Uint32 getDequeued();
	void setSoundDataIn(SoundData *soundDataIn);
	void setSoundDataOut(SoundData *soundDataOut);
	void setDequeued(Uint32 dequeued);
private:
	SoundData *soundDataIn;
	SoundData *soundDataOut;
	Uint32 dequeued;
};

#endif /* SRC_SOUNDDATAINOUT_H_ */
