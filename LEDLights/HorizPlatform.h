#ifndef HORIZPLATFORM_H
#define HORIZPLATFORM_H

#include "LEDObject.h"

class HorizPlatform : public LEDObject{
public:
	HorizPlatform(LEDMatrixNode* dis, int w, int h, int x, int y, Color c, int d, float s, int t, InputManager* input);
	short dir;
	float spd;
	int time;
	void step();
	//void pushMatrix();
};

#endif