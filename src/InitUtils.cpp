/*
 * InitSound.cpp
 *
 *  Created on: 19 janv. 2021
 *      Author: Papa
 */

#include "InitUtils.h"

#include <SDL2/SDL.h>
#include <iostream>
#include <iomanip>

using namespace std;

void InitUtils::displayInfoInputSound() {
	int numberOfCaptureDevices = SDL_GetNumAudioDevices(1);
	cout << setw(39) << " Capture device name " << endl;
	for (int i = 0; i < numberOfCaptureDevices; i++) {
		cout << setw(3) << i << ")" << SDL_GetAudioDeviceName(i, 1) << endl;
	}
}

bool InitUtils::init() {
	int tmp = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
	bool result = false;
	if (tmp != 0) {
		cout << "ERROR: " << SDL_GetError() << endl;
	} else {
		result = true;
	}
	return result;
}



InitUtils::InitUtils() {
	cout << "InitUtils creation." << endl;
}

InitUtils::~InitUtils() {
	cout << "InitUtils destruction" << endl;
}

