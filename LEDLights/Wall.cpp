#include "Wall.h"

Wall::Wall(LEDMatrixNode* dis, int w, int h, int x, int y, Color c, InputManager* input) : LEDObject(dis,w,h,x,y,c,input){
	flag = OB_SOLID;
}