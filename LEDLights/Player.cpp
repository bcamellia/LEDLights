#include "Player.h"
#include "HorizPlatform.h"

float vspd = 0;
int canJumpTimer = 0;
bool canJump = true;
float xacc = 0;
bool inAir = true;

Player::Player(LEDMatrixNode* dis, int w, int h, int x, int y, Color c, InputManager* input) : LEDObject(dis,w,h,x,y,c,input){
	flag = OB_PLAYER;
}

void Player::step(){
	if(inAir){
		vspd-=grav;
		y+=vspd;
	}else{
		vspd=0;
	}
	if(display->input->keyDown(input->RIGHT_KEY)){
		if(placeFree_x(x+width-1+xspd)){
			x+=xspd;
		}
	}else if(display->input->keyDown(input->LEFT_KEY)){
		if(placeFree_x(x-xspd)){
			x-=xspd;
		}
	}if(display->input->keyDown(VK_UP)&&!inAir&&canJump){
		vspd = ivspd;
		y+=vspd;
		canJump=false;
	}

	canJumpTimer++;
	if(canJumpTimer==canJumpWaitTime){
		canJump=true;
		canJumpTimer=0;
	}

	if(xacc!=0){
		if(xacc<=-grav){
			xacc+=grav;
			if(placeFree_x(x-xacc)){
				x+=xacc;
			}
		}else if(xacc>=grav){
			xacc-=grav;
			if(placeFree_x(x+width+xacc)){
				x+=xacc;
			}
		}
	}else{
		xacc=0;
	}

	pair<nodeConnection*, pair<int,int> > p1 = display->getMatrixFromPos(x+1,y); //update current display player belongs to
	pair<nodeConnection*, pair<int,int> > p2 = display->getMatrixFromPos(x+width-1,y);
	if(p2.first){
		if(p2.first->flag1==FLAG_LEFT&&FLAG_LEFT==p2.first->flag2){
			input->reverseLeftRight();
			x=0;
		}
	}if(p1.first){
		if(p1.first->flag1==FLAG_RIGHT&&FLAG_RIGHT==p1.first->flag2){//&&!input->isLeftRightReversed){
			input->reverseLeftRight();
		}
	}
	if(!p1.first&&!p2.first&&input->isLeftRightReversed&&!input->keyDown(input->LEFT_KEY)&&!input->keyDown(input->RIGHT_KEY)){
		input->reverseLeftRight();
	}
	//input->reverseLeftRight();

	if(p1.first&&p1.first==p2.first){
		display = static_cast<LEDMatrixNode*>(p1.first->other);
		y = p1.second.first;
		x = p1.second.second;
		switch(p1.first->flag1){
			case FLAG_RIGHT:
			case FLAG_LEFT:
				switch(p1.first->flag2){
				case FLAG_RIGHT:
					x = display->width-width;
					break;
				case FLAG_LEFT:
					x=0;
				case FLAG_TOP:
					vspd+=fabs(xacc);
					break;
				case FLAG_BOTTOM:
					vspd-=fabs(xacc);
				break;
				default:
					break;
				}
			break;
			case FLAG_TOP:
				switch(p1.first->flag2){
				case FLAG_RIGHT:
					xacc = -vspd;
					vspd=0;
					break;
				case FLAG_LEFT:
					xacc = vspd;
					vspd=0;
				case FLAG_TOP:
					vspd*=-1;
					break;
				default:
					break;
				}
			break;
			case FLAG_BOTTOM:
				switch(p1.first->flag2){
				case FLAG_RIGHT:
					xacc = vspd;
					vspd=0;
					break;
				case FLAG_LEFT:
					xacc = -vspd;
					vspd=0;
				case FLAG_BOTTOM:
					vspd*=-1;
					break;
				default:
					break;
				}
			break;
		}
	}

	doGroundCheck();

	_RPT1(_CRT_WARN,"X: %i ",(int)x);
	_RPT1(_CRT_WARN,"Y: %i\n",(int)y);

}

void Player::doGroundCheck(){
	int x1 = x;
	int x2 = x+width-1;
	
	if(vspd<=0){
		int nexty = (int)y-1+(int)vspd;
		
		int y1 = (int)y-1;

		for(int xx=x1; xx<=x2; xx++){
			for(int yy=y1; yy>=nexty; yy--){
				DisplayLED* left = display->getLED(xx,yy);
				if(left&&(left->flag==(OB_SOLID)||left->flag==(OB_PUSH))){
					y=yy+1;
					inAir=false;
					return;
				}else if(left&&(left->flag==OB_H_PLAT)){
					y=yy+1;
					HorizPlatform* plat = static_cast<HorizPlatform*>(left->ob);
					if(plat->dir==1){
						if(placeFree_x(x+width+plat->spd)){
							x+=plat->spd;
						}
					}else if(placeFree_x(x-plat->spd)){
							x-=plat->spd;						
					}
					
					inAir=false;
					return;
				}
			}

		}
		inAir=true;
		return;
	}else{
		int nexty = (int)y+height+(int)vspd;
		int y1 = (int)y+height;

		for(int xx=x1; xx<=x2; xx++){
			for(int yy=y1; yy<=nexty; yy++){
				DisplayLED* left = display->getLED(xx,yy);
				if(left&&(left->flag==(OB_SOLID)||left->flag==(OB_PUSH))){
					y=yy-height;
					vspd = 0;
					inAir=true;
					return;
				}/*else if(left&&(left->flag==OB_H_PLAT)){
					y=yy+1;
					HorizPlatform* plat = static_cast<HorizPlatform*>(left->ob);
					if(plat->dir==1){
						if(placeFree_x(x+width+plat->spd)){
							x+=plat->spd;
						}
					}else if(placeFree_x(x-plat->spd)){
							x-=plat->spd;						
					}
					
					inAir=false;
					return;
				}*/
			}

		}
		inAir=true;
		return;
	}
	inAir=false;
}

bool Player::placeFree_x(int xpos){
	DisplayLED* left = 0;
	if(xpos>x){
		for(int yy=(int)y; yy<y+height; yy++){
			for(int xx = (int)x+width; xx <= xpos; xx++){
				left = display->getLED(xx,yy);
				if(left&&(left->flag==OB_SOLID||left->flag==(OB_PUSH))){
					return false;
				}
			}
		}
		return true;
	}else{
		for(int yy=(int)y; yy<y+height; yy++){
			for(int xx = (int)x; xx >= xpos; xx--){
				left = display->getLED(xx,yy);
				if(left&&(left->flag==OB_SOLID||left->flag==(OB_PUSH))){
					return false;
				}
			}
		}
		return true;
	}
}