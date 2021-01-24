/*
 * InitSound.cpp
 *
 *  Created on: 19 janv. 2021
 *      Author: Papa
 */

#include <SDL2/SDL.h>
#include <iostream>
#include "SDLUtils.h"

using namespace std;

void SDLUtils::displayInfoInputSound() {
	int numberOfCaptureDevices = SDL_GetNumAudioDevices(1);
	cout << " Capture device name " << endl;
	for (int i = 0; i < numberOfCaptureDevices; i++) {
		cout << i << ") " << SDL_GetAudioDeviceName(i, 1) << endl;
	}
}

bool SDLUtils::init() {
	int tmp = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
	bool result = false;
	if (tmp != 0) {
		cout << "ERROR: " << SDL_GetError() << endl;
	} else {
		result = true;
	}
	return result;
}



SDLUtils::SDLUtils() {
	cout << "InitUtils creation." << endl;
}

SDLUtils::~SDLUtils() {
	cout << "InitUtils destruction" << endl;
}

int SDLUtils::toggleFullScreen(SDL_Window *pWindow) {
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

