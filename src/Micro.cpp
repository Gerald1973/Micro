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
	desired.format = AUDIO_F32SYS;
	desired.channels = 1;
	desired.samples = 4096;
	desired.callback = NULL;
	string deviceName = "";
	int choice = 0;
	if (InitUtils::getInstance()->init()) {
		InitUtils::getInstance()->displayInfoInputSound();
		cout << "Enter the desired capture device :";
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cin >> choice;
		deviceName = SDL_GetAudioDeviceName(choice, 1);
		cout << "The selected device is : " << deviceName << endl;
		int deviceIdIn = SDL_OpenAudioDevice(deviceName.c_str(), SDL_TRUE,
				&desired, &obtained, 0);
		if (deviceIdIn == 0) {
			cout << "ERROR: Can not open the device : " << deviceName.c_str()
					<< endl;
		} else {
			SDL_Window* pWindow = SDL_CreateWindow("Micro", 0, 0, GlobalConstants::SCREEN_WIDTH, GlobalConstants::SCREEN_HEIGHT,
						SDL_WINDOW_OPENGL | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
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
				Uint8 buffer[1024];
				const Uint32 bytesDequeued = SDL_DequeueAudio(deviceIdIn,
						buffer, sizeof(buffer));
				for (Uint32 c = 0; c < bytesDequeued / 4; c++) {
					//cout << unsigned(buffer[c]) << endl;
					Uint32 tmp = unsigned(buffer[c]) << 24;
					tmp = tmp | (unsigned(buffer[c + 1]) << 16);
					tmp = tmp | (unsigned(buffer[c + 2]) << 8);
					tmp = tmp | unsigned(buffer[c + 3]);

					cout << (int) tmp << endl;
				}
				SDL_Delay(16);
			}
			SDL_CloseAudioDevice(deviceIdIn);
			SDL_Quit();
		}
	}
	return 0;
}
