#ifndef LEDMATRIX_H
#define LEDMATRIX_H
#include "Renderer.h"
#include "Input.h"
#include "LEDStuff.h"

#define _CRTDBG_MAP_ALLOC
#define _USE_MATH_DEFINES
#include <stdlib.h>
#include <crtdbg.h>
#include <math.h>
#include <map>

enum {FLAG_TOP, FLAG_LEFT, FLAG_BOTTOM, FLAG_RIGHT};

const int lightSpacing = 2;

const float lightSize = 0.025f;
const float borderWidth = 0.05f;
const float pinLength = lightSize*4;
const float pinOffset = lightSize/4;
const float pinWidth = pinOffset*2;

#define grav .04f
#define ivspd 1.2f
#define xspd .2f
#define canJumpWaitTime 100
#define pushspd .2f


class LEDMatrix{
public:
	GLuint* mTexVbo;
	GLuint* mVbo;
	GLuint* mIndexVbo;
	GLuint* mBVbo;
	int height;
	int width;
	float xCornerOffset;
	float yCornerOffset;
	LEDMatrix(OGLRenderContext* other, InputManager* i_otherm, int w, int h, float ul_xpos, float ul_ypos,PhysicsWorld* pWorld);
	virtual void rePaintScreen();
	void renderLEDS(PVRTMat4 mProj);
	virtual void renderPinNodes(PVRTMat4 mWorld) = 0;
	virtual void checkSelectedPins(pinNode* n, vector<pinNode*>* otherSelPins);
	virtual vector<pinNode*>* getArrSelectedPins();
	DisplayLED** ledMatrix;
	vector<Color> ledList;
	unsigned int indexSize;
	OGLRenderContext* OGLRenderer;
	InputManager* input;
	pinNode*** pins;
	int numPinIndices;
	PhysicsWorld* world;
	vector<float> selectedPinVerts;
	vector<pinNode*> selectedPinNodes;
	map<int, int> selectedVertsLookup;
	//vector< vector<pinNode*> > connectedPinNodes;
	//bool pinsAreConnected;
	//BoundingBox* bounds;
	btCollisionObject* body;
};

#endif