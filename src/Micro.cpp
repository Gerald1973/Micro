//============================================================================
// Name        : Micro.cpp
// Author      : Gérald Maréchal
// Version     :
// Copyright   : 
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include "InitUtils.h"
#include <SDL2/SDL.h>
#include <string>
#include "GlobalConstants.h"
#include "Timer.h"
#include "SoundData.h"

using namespace std;

int toggleFullScreen(SDL_Window *pWindow) {
	bool result = true;
	Uint32 fullscreenFlag = SDL_WINDOW_FULLSCREEN_DESKTOP;
	bool isFullscreen = SDL_GetWindowFlags(pWindow) & fullscreenFlag;
	int tmp = SDL_SetWindowFullscreen(pWindow, isFullscreen ? 0 : fullscreenFlag);
	if (tmp < 0) {
		std::cout << "Error : fullscreen not possible" << std::endl;
		result = false;
	}
	return result;
}

struct SoundDataInOut {
	SoundData *soundDataIn;
	SoundData *soundDataOut;
	Uint32 dequeued;
};

int threadFunction(void *data) {
	SoundDataInOut *soundDataInOut = (SoundDataInOut*) data;
	int deviceIdOut = soundDataInOut->soundDataOut->getDeviceId();
	int deviceIdIn = soundDataInOut->soundDataIn->getDeviceId();
	int *buffer = soundDataInOut->soundDataIn->getBuffer();
	SDL_PauseAudioDevice(deviceIdOut, SDL_FALSE);
	SDL_PauseAudioDevice(deviceIdIn, SDL_FALSE);
	while (true) {
		soundDataInOut->dequeued = SDL_DequeueAudio(deviceIdIn, buffer, GlobalConstants::SAMPLES * 4);
		if (soundDataInOut->dequeued > 0) {
			SDL_QueueAudio(deviceIdOut, buffer, soundDataInOut->dequeued);
		} else {
			buffer = new int[GlobalConstants::SAMPLES];
			soundDataInOut->soundDataIn->setBuffer(buffer);
		}
		SDL_Delay(20);
	}
	return 0;
}

SoundDataInOut* buildSoundDataInOut() {
	SoundDataInOut *result = new SoundDataInOut();
	SDL_AudioSpec desiredIn = { 0 };
	SDL_AudioSpec desiredOut = { 0 };
	SDL_AudioSpec obtainedIn = { 0 };
	SDL_AudioSpec obtainedOut = { 0 };
	desiredIn.freq = GlobalConstants::FREQUENCY;
	desiredIn.format = AUDIO_S32SYS;
	desiredIn.channels = 1;
	desiredIn.samples = GlobalConstants::SAMPLES;
	desiredIn.callback = nullptr;
	desiredOut.freq = GlobalConstants::FREQUENCY;
	desiredOut.format = AUDIO_S32SYS;
	desiredOut.channels = 1;
	desiredOut.samples = GlobalConstants::SAMPLES;
	desiredOut.callback = nullptr;
	Uint32 deviceIdOut = SDL_OpenAudioDevice(nullptr, SDL_FALSE, &desiredIn, &obtainedIn, 0);
	Uint32 deviceIdIn = SDL_OpenAudioDevice(nullptr, SDL_TRUE, &desiredOut, &obtainedOut, 0);
	//build out
	SoundData *soundDataIn = new SoundData();
	soundDataIn->setDesired(desiredIn);
	soundDataIn->setDeviceId(deviceIdIn);
	soundDataIn->setObtained(obtainedIn);
	//build in
	SoundData *soundDataOut = new SoundData();
	soundDataOut->setDesired(desiredOut);
	soundDataOut->setDeviceId(deviceIdOut);
	soundDataOut->setObtained(obtainedOut);
	result->soundDataIn = soundDataIn;
	result->soundDataOut = soundDataOut;
	result->dequeued = 0;
	return result;
}

void draw(SDL_Renderer *renderer, SDL_Window *pWindow, SoundDataInOut *soundDataInOut) {
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
	float x1 = 0;
	float y1 = 0;
	float x2 = 0;
	float y2 = 0;
	int w = 0;
	int h = 0;
	SDL_GetWindowSize(pWindow, &w, &h);
	const float halfScreen = (float) h / 2.0;
	Uint32 dequeued = soundDataInOut->dequeued;
	int *buffer = soundDataInOut->soundDataIn->getBuffer();
	Uint32 numberOfPoints = dequeued / 4;
	if (numberOfPoints > 0) {
		for (Uint32 c = 0; c < numberOfPoints-1; c++) {
			x1 = ((float) c / ((float) numberOfPoints)) * (float) w;
			y1 = ((float) buffer[c] / (float) INT32_MAX) * h;
			x2 = ((float) (c + 1) / ((float) numberOfPoints)) * (float) w;
			y2 = ((float) buffer[c+1] / (float) INT32_MAX) * h;
			SDL_RenderDrawLineF(renderer, x1, y1  + halfScreen, x2, y2  + halfScreen);
		}
	}
}

int main(int argv, char **args) {
	bool cap = true;
	Timer timer;
	if (InitUtils::getInstance()->init()) {
		SoundDataInOut *soundDataInOut = buildSoundDataInOut();
		SDL_Thread *thread1 = SDL_CreateThread(threadFunction, "TestThread", soundDataInOut);
		SDL_Window *pWindow = SDL_CreateWindow("Micro", 0, 0, GlobalConstants::SCREEN_WIDTH, GlobalConstants::SCREEN_HEIGHT,
				SDL_WINDOW_OPENGL | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
		SDL_Renderer *renderer = SDL_CreateRenderer(pWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_Event event;
		bool loop = true;
		float frame = 0;
		while (loop) {
			timer.start();
			while (SDL_PollEvent(&event)) {
				switch (event.type) {
				case SDL_KEYDOWN:
					switch (event.key.keysym.sym) {
					case SDLK_F11:
						toggleFullScreen(pWindow);
						break;
					case SDLK_ESCAPE:
						loop = false;
						break;
					}
				}
			}
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
			SDL_RenderClear(renderer);
			draw(renderer, pWindow, soundDataInOut);
			SDL_RenderPresent(renderer);
			frame++;
			if ((cap == true) && (timer.get_ticks() < 1000 / GlobalConstants::FRAMES_PER_SECOND)) {
				SDL_Delay((1000 / GlobalConstants::FRAMES_PER_SECOND) - timer.get_ticks());
			}
		}
		SDL_Quit();
	}
	return 0;
}
