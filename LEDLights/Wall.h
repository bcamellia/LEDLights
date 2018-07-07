#ifndef WALL_H
#define WALL_H

#include "LEDObject.h"

class Wall : public LEDObject{
public:
	Wall(LEDMatrixNode* dis, int w, int h, int x, int y, Color c, InputManager* input);
};

#endif