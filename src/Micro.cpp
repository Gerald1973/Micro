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

const int FREQUENCY = 44100;
const int SAMPLES = 4096;
const int FRAMES_PER_SECOND = 30;
int buffer[SAMPLES] = { 0 };
Uint32 dequeued = 0;

static int threadFunction(void *data) {
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
		if (dequeued > 0) {
			SDL_QueueAudio(deviceIdOut, buffer, dequeued);
		} else {
			 buffer[SAMPLES] = { 0 };
		}
		SDL_Delay(1000 / FRAMES_PER_SECOND);
	}
	return 0;
}

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

int main(int argv, char **args) {
	bool cap = true;
	Timer timer;
	if (InitUtils::getInstance()->init()) {

		//int buffer[SAMPLES] = {0};
		SDL_Thread *thread1 = SDL_CreateThread(threadFunction, "TestThread", nullptr);
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
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
			float y = 0;
			float x = 0;

			int w = 0;
			int h = 0;
			SDL_GetWindowSize(pWindow, &w, &h);
			const float halfScreen = (float) h / 2.0;
			for (Uint32 c = 0; c < dequeued/4; c++) {
				x = ((float) c / ((float) dequeued / 4.0)) * (float) w;
				y = ((float) buffer[c] / (float) INT32_MAX) * halfScreen;
				SDL_RenderDrawPointF(renderer, x, y + halfScreen);
			}
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
