//============================================================================
// Name        : Micro.cpp
// Author      : Gérald Maréchal
// Version     :
// Copyright   : 
// Description : Micropohone oscilloscope.
//============================================================================

#include <iostream>
#include <SDL2/SDL.h>
#include <string>
#include "GlobalConstants.h"
#include "SDLUtils.h"
#include "Timer.h"
#include "SoundData.h"
#include "SoundDataInOut.h"

using namespace std;


int threadFunction(void *data) {
	SoundDataInOut *soundDataInOut = (SoundDataInOut*) data;
	int deviceIdOut = soundDataInOut->getSoundDataOut()->getDeviceId();
	int deviceIdIn = soundDataInOut->getSoundDataIn()->getDeviceId();
	short *buffer = soundDataInOut->getSoundDataIn()->getBuffer();
	SDL_PauseAudioDevice(deviceIdOut, SDL_FALSE);
	SDL_PauseAudioDevice(deviceIdIn, SDL_FALSE);
	while (true) {
		soundDataInOut->setDequeued(SDL_DequeueAudio(deviceIdIn, buffer, GlobalConstants::SAMPLES * sizeof(buffer[0])));
		if (soundDataInOut->getDequeued() > 0) {
			SDL_QueueAudio(deviceIdOut, buffer, soundDataInOut->getDequeued());
		}
		SDL_Delay(20);
	}
	return 0;
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
	Uint32 dequeued = soundDataInOut->getDequeued();
	short *buffer = soundDataInOut->getSoundDataIn()->getBuffer();
	Uint32 numberOfPoints = dequeued / 2;
	if (numberOfPoints > 0) {
		for (Uint32 c = 0; c < numberOfPoints-1; c++) {
			x1 = ((float) c / ((float) numberOfPoints)) * (float) w;
			y1 = ((float) buffer[c] / (float) INT16_MAX) * (float) halfScreen;
			x2 = ((float) (c + 1) / ((float) numberOfPoints)) * (float) w;
			y2 = ((float) buffer[c+1] / (float) INT16_MAX) * (float) halfScreen;
			SDL_RenderDrawLineF(renderer, x1, y1  + halfScreen, x2, y2  + halfScreen);
		}
	}
}

int main(int argv, char **args) {
	bool cap = true;
	Timer* timer = new Timer();
	if (SDLUtils::getInstance()->init()) {
		SoundDataInOut *soundDataInOut = new SoundDataInOut();
		SDL_Thread *thread1 = SDL_CreateThread(threadFunction, "TestThread", soundDataInOut);
		SDL_Window *pWindow = SDL_CreateWindow("Micro", 0, 0, GlobalConstants::SCREEN_WIDTH, GlobalConstants::SCREEN_HEIGHT,
				SDL_WINDOW_OPENGL | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
		SDL_Renderer *renderer = SDL_CreateRenderer(pWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);
		SDL_Event event;
		bool loop = true;
		float frame = 0;
		while (loop) {
			timer->start();
			while (SDL_PollEvent(&event)) {
				switch (event.type) {
				case SDL_KEYDOWN:
					switch (event.key.keysym.sym) {
					case SDLK_F11:
						SDLUtils::getInstance()->toggleFullScreen(pWindow);
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
			if ((cap == true) && (timer->get_ticks() < 1000 / GlobalConstants::FRAMES_PER_SECOND)) {
				SDL_Delay((1000 / GlobalConstants::FRAMES_PER_SECOND) - timer->get_ticks());
			}
		}
		SDL_DetachThread(thread1);
		delete soundDataInOut;
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(pWindow);
		SDL_Quit();
		SDLUtils::getInstance()->destroy();
	}
	return 0;
}
