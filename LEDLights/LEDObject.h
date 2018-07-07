#ifndef LEDOBJECT_H
#define LEDOBJECT_H

#include "Input.h"
#include "LEDMatrixNode.h"

class LEDObject{
public:
	LEDObject(LEDMatrixNode* dis, int w, int h, int ox, int oy, Color c, InputManager* input);
	virtual void render();
	virtual void step();
	virtual void pushMatrix();
	InputManager* input;
	LEDMatrixNode* display;
	int width, height;
	float x, y;
	short flag;
	Color col;
	VirtLED** ledMatrix;
};

#endif