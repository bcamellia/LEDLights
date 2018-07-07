#ifndef LEDSTUFF_H
#define LEDSTUFF_H

#include "PhysicsWorld.h"

const short OB_SOLID = 1;
const short OB_DISPLAY = 2;
const short OB_NONSOLID = 3;
const short OB_PUSH = 4;
const short OB_PLAYER = 5;
const short OB_H_PLAT = 6;

struct Color{
	float r,g,b,a;
	Color(float rr, float gg, float bb, float aa){
		r=rr; g=gg; b=bb; a=aa;
	}
	Color(){
		r=0; g=0; b=0; a=0;
	}
	bool operator==(const Color& other) const{
		return (this->a==other.a)&&(this->b==other.b)&&(this->g==other.g)&&(this->r==other.r);
	}
	bool operator!=(const Color& other) const{
		return !(*this==other);
	}
};

#define C_RED Color(1,0,0,1)
#define C_GREEN Color(0,1,0,1)
#define C_BLUE Color(0,0,1,1)
#define C_WHITE Color(1,1,1,1)
#define C_BLACK Color(0,0,0,1)
#define C_TRANS Color(0,0,0,0)

struct VirtLED{
	Color c;
	VirtLED(){};
	VirtLED(Color other){
		c = other;
	}
};

struct DisplayLED{
	Color c;
	short cindex;
	void* ob;
	short flag;
	vector<Color>* colorIndex;
	DisplayLED(){};
	DisplayLED(short index, vector<Color>* arr){
		c = C_BLUE;
		flag = OB_DISPLAY;
		colorIndex = arr;
		cindex = index;
		ob = 0;
	}
	void setColor(Color col){
		for(int i=cindex; i<cindex+4; i++){
			colorIndex->at(i)=col;
		}
		c = col;
	}
};

struct pinNode{
	btCollisionObject* colObj;
	pair<int,int> xy;
	vector<GLfloat> pinVerts;
	void* ob;
	pinNode(){}
	pinNode(btCollisionObject* colObj, pair<int,int> xy, vector<GLfloat> pinIds, void* ob){
		this->colObj = colObj; this->xy = xy; this->pinVerts = pinIds; this->ob = ob;
	}
	float* getRect(){
		static float rect[8];
		for(int i=0; i<8; i++){
			rect[i] = pinVerts[i];
		}
		return rect;
	}
	float* getPoint(){
		static float point[2];
		for(int i=0; i<2; i++){
			point[i] = pinVerts[i+2];
		}
		return point;
	}
};


#endif