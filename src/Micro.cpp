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

using namespace std;

int main(int argv, char **args) {
	SDL_AudioSpec desired, obtained;
	SDL_zero(obtained);
	SDL_zero(desired);
	desired.freq = 44100;
	desired.format = AUDIO_S32SYS;
	desired.channels = 1;
	desired.samples = 4096;
	desired.callback = NULL;
	string deviceName = "";
	int choice = 0;
	if (InitUtils::getInstance()->init()) {
		InitUtils::getInstance()->displayInfoInputSound();
		cout << "Enter the desired capture device :" << endl;
		cin >> choice;
		deviceName = SDL_GetAudioDeviceName(choice, 1);
		cout << "The selected device is : " << deviceName << endl;
		int deviceIdIn = SDL_OpenAudioDevice(deviceName.c_str(), SDL_TRUE, &desired, &obtained, 0);
		if (deviceIdIn == 0) {
			cout << "ERROR: Can not open the device : " << deviceName.c_str() << endl;
		} else {
			SDL_Window *pWindow = SDL_CreateWindow("Micro", 0, 0, GlobalConstants::SCREEN_WIDTH, GlobalConstants::SCREEN_HEIGHT,
					SDL_WINDOW_OPENGL | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
			SDL_Renderer *renderer = SDL_CreateRenderer(pWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE | SDL_RENDERER_PRESENTVSYNC);
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
			SDL_Event event;
			bool loop = true;
			while (loop) {
				SDL_PauseAudioDevice(deviceIdIn, SDL_FALSE);
				while (SDL_PollEvent(&event)) {
					switch (event.type) {
					case SDL_KEYDOWN:
					case SDLK_ESCAPE:
						loop = false;
						break;
					}
				}
				int buffer[1024];
				const Uint32 bytesDequeued = SDL_DequeueAudio(deviceIdIn, buffer, sizeof(buffer));
				SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
				SDL_RenderClear(renderer);
				SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
				for (Uint32 c = 0; c < bytesDequeued / 4; c++) {
					float y = ( buffer[c] / (float) INT32_MAX);
					y = y * ((float) GlobalConstants::SCREEN_HEIGHT / 2.0);
					//cout << y << endl;
					SDL_RenderDrawPoint(renderer, c, y + GlobalConstants::SCREEN_HEIGHT / 2.0);
				}
				SDL_RenderPresent(renderer);
			}
			SDL_CloseAudioDevice(deviceIdIn);
			SDL_Quit();
		}
	}
	return 0;
}
