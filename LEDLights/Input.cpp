#include "Input.h"

//LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

InputManager::InputManager(){
	isMouseDown=false;
	isKeyDown=false;
	pinsSelected=false;
	for(int i=0; i<80; i++){
		key[i]=0;
	}
	mouse_x = 0;
	mouse_y = 0;
	isLeftRightReversed = false;
	LEFT_KEY = VK_LEFT;
	RIGHT_KEY = VK_RIGHT;
	DELETE_KEY = VK_DELETE;
}

void InputManager::updateMousePos(HWND hWnd, LPARAM lParam){
	if(isMouseDown){
		RECT rcWinDimensions;
		GetClientRect(hWnd,&rcWinDimensions);
		mouse_x = (((float)LOWORD(lParam)/(float)(rcWinDimensions.right)-.5)*2);
		mouse_y = (((float)HIWORD(lParam)/(float)(rcWinDimensions.bottom)-.5)*-2);
	}
}


void InputManager::reverseLeftRight(){
	isLeftRightReversed = !isLeftRightReversed;
	short o_RIGHT_KEY = RIGHT_KEY;
	RIGHT_KEY = LEFT_KEY;
	LEFT_KEY = o_RIGHT_KEY;
}

PVRTVec2 InputManager::getMousePos(){
	/*if(isMouseDown){

	}*
	POINT cursorPos;
	GetCursorPos(&cursorPos);
	ScreenToClient((*OGL)->hWnd,(&cursorPos));
	return PVRTVec2(cursorPos.x,cursorPos.y);*/
	return PVRTVec2(mouse_x,mouse_y);
}

bool InputManager::keyDown(short k){
	//return GetAsyncKeyState(key);
	return key[k]==1;
}