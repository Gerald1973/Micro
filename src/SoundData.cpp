/*
 * SoundData.cpp
 *
 *  Created on: Jan 23, 2021
 *      Author: private
 */

#include "SoundData.h"

/**
 * Constructor.
 * the buffer is initialized to zero.
 * the desired is initialized to zero.
 * the deviceid is initialized to zero;
 * the obtained is initialized to zero.
 */
SoundData::SoundData() {
	this->buffer = new short[GlobalConstants::SAMPLES];
	this->desired = { 0 };
	this->deviceId = 0;
	this->obtained = { 0 };
}

Uint32 SoundData::getDeviceId() {
	return this->deviceId;
}

void SoundData::setDeviceId(Uint32 &deviceId) {
	this->deviceId = deviceId;
}

void SoundData::setDesired(SDL_AudioSpec &desired) {
	this->desired = desired;
}

SDL_AudioSpec SoundData::getDesired() {
	return this->desired;
}

void SoundData::setObtained(SDL_AudioSpec &audioSpec) {
	this->obtained = audioSpec;
}

SDL_AudioSpec SoundData::getObtained() {
	return this->obtained;
}

SoundData::~SoundData() {
}

short* SoundData::getBuffer() {
	return this->buffer;
}

void SoundData::setBuffer(short *buffer) {
	this->buffer = buffer;
}
