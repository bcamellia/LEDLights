#ifndef PUSHABLEBLOCK_H
#define PUSHABLEBLOCK_H

#include "LEDObject.h"

class PushableBlock : public LEDObject{
public:
	PushableBlock(LEDMatrixNode* dis, int w, int h, int x, int y, Color c,InputManager* input);
	float vspd;
	bool inAir;
	void checkPush();
	void doGroundCheck();
	bool placeFree_x(int xpos);
	bool placeFree_y(int ypos);
	void step();
};

#endif