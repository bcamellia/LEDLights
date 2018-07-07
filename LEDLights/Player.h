#ifndef PLAYER_H
#define PLAYER_H
#include "LEDObject.h"
#include "Input.h"

class Player : public LEDObject{
public:
	Player(LEDMatrixNode* dis, int w, int h, int x, int y, Color c, InputManager* input);
	void doGroundCheck();
	bool placeFree_x(int xpos);
	void step();
};

#endif