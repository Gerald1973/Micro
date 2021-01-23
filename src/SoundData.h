/*
 * SoundData.h
 *
 *  Created on: Jan 23, 2021
 *      Author: private
 */

#ifndef SRC_SOUNDDATA_H_
#define SRC_SOUNDDATA_H_
#include <SDL2/sdl.h>
#include "GlobalConstants.h"

class SoundData {
public:
	int* getBuffer();
	Uint32 getDeviceId();
	void setDeviceId(Uint32 &deviceId);
	void setDesired(SDL_AudioSpec &desired);
	SDL_AudioSpec getDesired();
	void setObtained(SDL_AudioSpec &obtained);
	SDL_AudioSpec getObtained();
	void setBuffer(int* buffer);
	SoundData();
	virtual ~SoundData();
private:
	SDL_AudioSpec desired, obtained;
	Uint32 deviceId;
	int* buffer;
};

#endif /* SRC_SOUNDDATA_H_ */
