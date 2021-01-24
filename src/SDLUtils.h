/*
 * InitUtils.h
 *
 *  Created on: 19 janv. 2021
 *      Author: Papa
 */

#ifndef INITUTILS_H_
#define INITUTILS_H_

class SDLUtils {

public:
	static inline SDLUtils* getInstance() {
		if (!instance) {
			instance = new SDLUtils();
		}
		return instance;
	}

	static inline void destroy() {
		if (instance) {
			delete instance;
		}
	}

	int toggleFullScreen(SDL_Window *pWindow);
	void displayInfoInputSound();
	bool init();

private:
	~SDLUtils();
	SDLUtils();
	inline static SDLUtils *instance = nullptr;
};

#endif /* INITUTILS_H_ */
