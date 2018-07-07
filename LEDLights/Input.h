#ifndef INPUTCONTROL_H
#define INPUTCONTROL_H

#include <Windows.h>
#include <PVRTVector.h>
#include "Renderer.h"
#include "LEDStuff.h"

class InputManager{
public:
	InputManager();
	OGLRenderContext** OGL;
	PVRTVec2 getMousePos();
	float mouse_x, mouse_y;
	bool isMouseDown;
	bool isKeyDown;
	bool pinsSelected;
	short key[400];
	void reverseLeftRight();
	bool isLeftRightReversed;
	vector<short> keysDown;
	bool keyDown(short k);
	void updateMousePos(HWND hWnd, LPARAM lParam);
	short LEFT_KEY;
	short RIGHT_KEY;
	short DELETE_KEY;
};

#endif