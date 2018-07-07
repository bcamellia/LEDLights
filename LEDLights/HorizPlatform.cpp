#include "HorizPlatform.h"

int count = 0;
float acc = .01;
float vel = 1;


HorizPlatform::HorizPlatform(LEDMatrixNode* dis, int w, int h, int x, int y, Color c, int d, float s, int t, InputManager* input) : LEDObject(dis,w,h,x,y,c,input){
	flag = OB_H_PLAT;
	dir = d;
	time = t;
	count = time / 2;
	spd = s;
}

void HorizPlatform::step(){
	if(count<time){
		x+=spd*dir;
	}else{
		count = 0;
		dir*=-1;
	}
	count++;
	/*if(vel>=0&&vel<=1){
		vel-=acc;
	}else if(vel<0){
		acc*=-1;
		vel=0;
	}else{
		acc*=-1;
		vel=1;
	}*/
}

/*void HorizPlatform::pushMatrix(){
	for(int i=(int)x; i<display->width&&i<(int)x+width; i++){
		for(int j=(int)y; j<display->height&&j<(int)y+height; j++){
			if(i>=0&&j>=0){
				DisplayLED* d_led = display->getLED(i,j);
				VirtLED v_led = ledMatrix[i-(int)x][j-(int)y];
				d_led->setColor(v_led.c);
				d_led->flag = flag;
				d_led->ob = this;
			}
		}
	}
}*/