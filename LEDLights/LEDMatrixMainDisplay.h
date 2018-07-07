#ifndef LEDMATRIXMAINDISPLAY_H
#define LEDMATRIXMAINDISPLAY_H

#include "LEDMatrixNode.h"

class LEDMatrixMain{
public:
	LEDMatrixMain(OGLRenderContext* other, InputManager* i_other);
	void addNode(LEDMatrixNode* node, short connectionFlag = 0, int start = 0, int end = 0);
	DisplayLED* getLED(int x, int y);
	vector<LEDMatrixNode*> nodes;
	vector<nodeConnection> nodeConnections;
	void rePaintScreen();
	void renderLEDS();
};

#endif