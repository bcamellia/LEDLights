#include "PushableBlock.h"

PushableBlock::PushableBlock(LEDMatrixNode* dis, int w, int h, int x, int y, Color c, InputManager* input) : LEDObject(dis,w,h,x,y,c,input){
	flag = OB_PUSH;
	vspd=0;
	inAir=true;
}

void PushableBlock::step(){
	if(inAir){
		vspd-=grav;
		y+=vspd;
	}else{
		vspd=0;
	}

	doGroundCheck();
	checkPush();

	pair<nodeConnection*, pair<int,int> > p1 = display->getMatrixFromPos(x,y);
	//pair<LEDMatrix*, pair<int,int> > p2 = display->getMatrixFromPos(x+width,y+height);
	if(p1.first){
		display = static_cast<LEDMatrixNode*>(p1.first->other);
		y = p1.second.first;
		x = p1.second.second;
	}

}

void PushableBlock::checkPush(){
	DisplayLED* left = 0;
	if(display->input->keyDown(input->RIGHT_KEY)){
		//if((int)x-1<0){return;}
		if(placeFree_x(x+width)){
			for(int yy=y; yy<=display->height&&yy<=y+height; yy++){
				left = display->getLED((int)x-1,(int)y);
				if(left&&left->flag==OB_PLAYER){
					x+=pushspd;
					return;
				}
			}
		}
	}else if(display->input->keyDown(input->LEFT_KEY)){
		//if((int)x+width>=display->width){return;}
		if(placeFree_x(x-1)){
			for(int yy=y; yy<=display->height&&yy<=y+height; yy++){
				//DisplayLED* left = &display->ledMatrix[(int)x+width][yy];
				left = display->getLED((int)x+width,(int)y);
				if(left&&left->flag==OB_PLAYER){
					x-=pushspd;
					return;
				}
			}
		}
	}
}

void PushableBlock::doGroundCheck(){
	if(vspd<=0){
		int x1 = x;
		int x2 = x+width-1;
		if(x1<0){
			x1=0;
		}else if(x1>=display->width){
			return;
		}
		if(x2>=display->width){
			x2 = display->width-1;
		}else if(x2<0){
			return;
		}
		int nexty = (int)y-1+(int)vspd;
		if(nexty>=display->height){
			return;
		}
		int y1 = (int)y-1;
		while(y1>=display->height){y1--;}
		for(int xx=x1; xx<=x2; xx++){
			for(int yy=y1; yy>=nexty; yy--){
				DisplayLED* left = display->getLED(xx,yy);
				if(left&&left->flag==OB_SOLID){
					y=yy+1;
					inAir=false;
					return;
				}
			}

		}
		inAir=true;
		return;
	}else if(vspd>0){
		inAir=true;
		return;
	}
	inAir=false;
}

bool PushableBlock::placeFree_x(int xpos){
	if(xpos>x){
		for(int yy=(int)y; yy<y+height&&y<display->width; yy++){
			for(int xx = (int)x+width-1; xx < xpos && xx<display->width; xx++){
				DisplayLED* left = display->getLED(xx,yy);
				if(left&&left->flag==OB_SOLID){
					return false;
				}
			}
		}
		return true;
	}else{
		for(int yy=(int)y; yy<y+height&&y<display->height; yy++){
			for(int xx = (int)x+1; xx >= xpos && xx>=0; xx--){
				DisplayLED* left = display->getLED(xx,yy);
				if(left&&left->flag==OB_SOLID){
					return false;
				}
			}
		}
		return true;
	}
}

bool PushableBlock::placeFree_y(int ypos){
	return true;
}