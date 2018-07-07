#ifndef LEDMATRIXNODE_H
#define LEDMATRIXNODE_H

#include "LEDMatrix.h"
#include <set>

typedef short CONNECTION_FLAG;

typedef pair<set<int>::iterator,bool> pinNodeRet;
typedef map<int, int>::iterator connectedVertsLookup_it;
//typedef pair< pair<CONNECTION_FLAG,LEDMatrix*> , pair<int,int> > nodeConnection;

struct nodeConnection{
	CONNECTION_FLAG flag1, flag2;
	vector<float> connectedPinVerts;
	map<int, int> connectedVertsLookup;
	LEDMatrix* other;
	int s1, s2, e1, e2;
	short index;
	nodeConnection(short f1, short f2, LEDMatrix* n, int start1, int end1, int start2, int end2, short index){
		flag1 = f1;
		flag2 = f2;
		other = n;
		s1 = start1;
		s2 = start2;
		e1 = end1;
		e2 = end2;
		this->index = index;
	}
};

class LEDMatrixNode : public LEDMatrix{
public:
	LEDMatrixNode(OGLRenderContext* other, InputManager* i_other, int w, int h, float ul_xpos, float ul_ypos,PhysicsWorld* pWorld);
	nodeConnection* addNode(LEDMatrixNode* node, short connectionFlag1, short connectionFlag2, int start1, int end1, int start2, int end2);
	vector<pinNode*>* getArrSelectedPins();
	GLuint* mPVbo;
	GLuint* mPIndexVbo;
	int numPinsWidth,numPinsHeight;
	int pinWidthOffset, pinHeightOffset;
	void checkSelectedPins(pinNode* n, LEDMatrixNode* otherSelDisplay);
	void shiftConnectedPins(int index, pinNode* n);
	void renderPinNodes(PVRTMat4 mWorld);
	void shiftNodeConnection(nodeConnection* node, nodeConnection* oNode, short newFlag, int start, int size);
	void removeNodeConnection(nodeConnection* node);
	void removeNodeFromLookup(nodeConnection* node);
	void rePaintScreen();
	void rePaintLEDRange(int start, int end, short flag, nodeConnection* node);
	int shiftPinsIntoArea(int start, int size, int max, short flag, nodeConnection* comp);
	nodeConnection* checkNodeisNodeConnection(int pos, short flag);
	bool canPlaceNodeConnection(int s1, int s2, nodeConnection* node, short flag);
	DisplayLED* getLED(int x, int y);
	pair<int,int> getRowColumn(nodeConnection* n, int x, int y);
	pair<nodeConnection*, pair<int,int> > getMatrixFromPos(int x, int y);
	vector<nodeConnection*> nodeConnections;
	nodeConnection*** connLookup;
	nodeConnection* selectedConnection;
	pinNode* lastSelectedNode;
};

#endif