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

using namespace std;

/*Thread 1*/
struct DataThread1 {
	int deviceIdIn;
	int deviceIdOut;
	SDL_AudioSpec desired;
	int *buffer;
	Uint32 dequeued;
};

const int FREQUENCY = 44100;
const int SAMPLES = 1024;
int buffer[SAMPLES] = { 0 };
int dequeued = 0;

static int threadFunction(void *data) {
	DataThread1 *dataThread1 = (DataThread1*) data;
	SDL_AudioSpec desired, obtainedIn, obtainedOut;
	SDL_zero(obtainedIn);
	SDL_zero(obtainedOut);
	SDL_zero(desired);
	desired.freq = FREQUENCY;
	desired.format = AUDIO_S32SYS;
	desired.channels = 1;
	desired.samples = SAMPLES;
	desired.callback = nullptr;
	int deviceIdOut = SDL_OpenAudioDevice(nullptr, SDL_FALSE, &desired, &obtainedIn, 0);
	int deviceIdIn = SDL_OpenAudioDevice(nullptr, SDL_TRUE, &desired, &obtainedOut, 0);
	SDL_PauseAudioDevice(deviceIdOut, SDL_FALSE);
	SDL_PauseAudioDevice(deviceIdIn, SDL_FALSE);
	while (true) {
		dequeued = SDL_DequeueAudio(deviceIdIn, buffer, SAMPLES * 4);
		SDL_QueueAudio(deviceIdOut, buffer, dequeued);
		dataThread1->buffer = buffer;
		dataThread1->dequeued = dequeued;
		SDL_Delay(16);
	}
	return 0;
}

int main(int argv, char **args) {
	bool cap = true;

	const int FRAMES_PER_SECOND = 20;
	Timer timer;
	if (InitUtils::getInstance()->init()) {

		//int buffer[SAMPLES] = {0};
		DataThread1 *dataThread1 = new DataThread1();
		SDL_Thread *thread1 = SDL_CreateThread(threadFunction, "TestThread", dataThread1);
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
				case SDLK_ESCAPE:
					loop = false;
					break;
				}
			}
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
			SDL_RenderClear(renderer);
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
			float y = 0;
			float x = 0;
			const float halfScreen = (float) GlobalConstants::SCREEN_HEIGHT / 2.0;
			for (Uint32 c = 0; c < dequeued /4; c++) {
				x = ((float) c / (float) dequeued) * (float) GlobalConstants::SCREEN_WIDTH;
				y = ((float) buffer[c] / (float) INT32_MAX) * halfScreen;
				SDL_RenderDrawPointF(renderer, x, y + halfScreen);
			}
			SDL_RenderDrawPointF(renderer, 50, 50);
			SDL_RenderPresent(renderer);
			frame++;
			if ((cap == true) && (timer.get_ticks() < 1000 / FRAMES_PER_SECOND)) {
				SDL_Delay((1000 / FRAMES_PER_SECOND) - timer.get_ticks());
			}
		}
		SDL_Quit();
	}
	return 0;
}
