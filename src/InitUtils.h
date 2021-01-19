/*
 * InitUtils.h
 *
 *  Created on: 19 janv. 2021
 *      Author: Papa
 */

#ifndef INITUTILS_H_
#define INITUTILS_H_

class InitUtils {

public:
	static inline InitUtils* getInstance() {
		if (!instance) {
			instance = new InitUtils();
		}
		return instance;
	}

	static inline void destroy() {
		if (instance) {
			delete instance;
			instance = nullptr;
		}
	}

	void displayInfoInputSound();
	bool init();
	virtual ~InitUtils();
private:
	InitUtils();
	inline static InitUtils *instance = nullptr;
};

#endif /* INITUTILS_H_ */
