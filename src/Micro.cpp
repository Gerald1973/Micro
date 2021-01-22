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

int main(int argv, char **args) {
	bool cap = true;
	const int FREQUENCY = 22050;
	const int FRAMES_PER_SECOND = 50;
	const int samples = FREQUENCY / FRAMES_PER_SECOND;
	SDL_AudioSpec desired, obtained;
	SDL_zero(obtained);
	SDL_zero(desired);
	desired.freq = FREQUENCY;
	desired.format = AUDIO_S32SYS;
	desired.channels = 1;
	desired.samples = samples;
	desired.callback = nullptr;
	Timer timer;
	if (InitUtils::getInstance()->init()) {

		int deviceIdOut = SDL_OpenAudioDevice(nullptr, SDL_FALSE, &desired, &obtained, 0);
		int deviceIdIn = SDL_OpenAudioDevice(nullptr, SDL_TRUE, &desired, &obtained, 0);
		if (deviceIdIn == 0) {
			cout << "ERROR: Can not open input sound device." << endl;
		} else {
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
				int buffer[samples];
				SDL_PauseAudioDevice(deviceIdOut, SDL_FALSE);
				SDL_PauseAudioDevice(deviceIdIn, SDL_FALSE);
				const Uint32 dequeued = SDL_DequeueAudio(deviceIdIn, buffer, sizeof(buffer)) / 4;
				SDL_QueueAudio(deviceIdOut, buffer, dequeued);
				SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
				SDL_RenderClear(renderer);
				SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
				float y = 0;
				float x = 0;
				const float halfScreen = (float) GlobalConstants::SCREEN_HEIGHT / 2.0;
				for (Uint32 c = 0; c < dequeued; c++) {
					x = ((float) c / (float) dequeued) * (float) GlobalConstants::SCREEN_WIDTH;
					y = ((float) buffer[c] / (float) INT32_MAX) * halfScreen;
					SDL_RenderDrawPointF(renderer, x, y + halfScreen);
				}
				SDL_RenderPresent(renderer);
				frame++;
				if ((cap == true) && (timer.get_ticks() < 1000 / FRAMES_PER_SECOND)) {
					SDL_Delay((1000 / FRAMES_PER_SECOND) - timer.get_ticks());
				}
			}
			SDL_CloseAudioDevice(deviceIdIn);
			SDL_Quit();
		}
	}
	return 0;
}
