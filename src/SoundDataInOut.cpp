/*
 * SoundDataInOut.cpp
 *
 *  Created on: Jan 24, 2021
 *      Author: private
 */

#include "SoundDataInOut.h"

SoundDataInOut::SoundDataInOut() {
		SDL_AudioSpec desiredIn = { 0 };
		SDL_AudioSpec desiredOut = { 0 };
		SDL_AudioSpec obtainedIn = { 0 };
		SDL_AudioSpec obtainedOut = { 0 };
		desiredIn.freq = GlobalConstants::FREQUENCY;
		desiredIn.format = AUDIO_S16SYS;
		desiredIn.channels = 1;
		desiredIn.samples = GlobalConstants::SAMPLES;
		desiredIn.callback = nullptr;
		desiredOut.freq = GlobalConstants::FREQUENCY;
		desiredOut.format = AUDIO_S16SYS;
		desiredOut.channels = 1;
		desiredOut.samples = GlobalConstants::SAMPLES;
		desiredOut.callback = nullptr;
		Uint32 deviceIdOut = SDL_OpenAudioDevice(nullptr, SDL_FALSE, &desiredIn, &obtainedIn, 0);
		Uint32 deviceIdIn = SDL_OpenAudioDevice(nullptr, SDL_TRUE, &desiredOut, &obtainedOut, 0);
		//build in
		soundDataIn = new SoundData();
		soundDataIn->setDesired(desiredIn);
		soundDataIn->setDeviceId(deviceIdIn);
		soundDataIn->setObtained(obtainedIn);
		//build out
		soundDataOut = new SoundData();
		soundDataOut->setDesired(desiredOut);
		soundDataOut->setDeviceId(deviceIdOut);
		soundDataOut->setObtained(obtainedOut);
		dequeued = 0;;
}

SoundDataInOut::~SoundDataInOut() {
	delete soundDataIn;
	delete soundDataOut;
}

SoundData* SoundDataInOut::getSoundDataIn() {
	return soundDataIn;
}

SoundData* SoundDataInOut::getSoundDataOut() {
	return soundDataOut;
}

Uint32 SoundDataInOut::getDequeued() {
	return dequeued;
}

void SoundDataInOut::setSoundDataIn(SoundData *soundDataIn) {
	delete soundDataIn;
	this->soundDataIn = soundDataIn;
}

void SoundDataInOut::setSoundDataOut(SoundData *soundDataOut) {
	delete soundDataOut;
	this->soundDataOut = soundDataOut;

}

void SoundDataInOut::setDequeued(Uint32 dequeued) {
	this->dequeued = dequeued;
}
