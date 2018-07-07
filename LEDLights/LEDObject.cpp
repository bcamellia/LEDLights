#include "LEDObject.h"

LEDObject::LEDObject(LEDMatrixNode* dis, int w, int h, int ox, int oy, Color c, InputManager* input){
	display = dis;
	width = w;
	height = h;
	x = ox;
	y = oy;
	flag = OB_NONSOLID;
	col = c;
	ledMatrix = new VirtLED*[width];
	for(int i=0; i<width; i++){
		ledMatrix[i] = new VirtLED[height];
		for(int j=0; j<height; j++){
			ledMatrix[i][j] = VirtLED(col);
		}
	}
	this->input = input;
}

void LEDObject::render(){
	pushMatrix();
}

void LEDObject::step(){}

void LEDObject::pushMatrix(){
	for(int i=(int)x; i<(int)x+width; i++){//display->width&&
		for(int j=(int)y; j<(int)y+height; j++){//display->height&&
			//if(i>=0){
				DisplayLED* d_led = 0;
				d_led = display->getLED(i,j);//&display->ledMatrix[i][j];
				if(d_led){
					VirtLED v_led = ledMatrix[i-(int)x][j-(int)y];
					d_led->setColor(v_led.c);
					d_led->flag = flag;
					d_led->ob = this;
				}
			//}
		}
	}
}