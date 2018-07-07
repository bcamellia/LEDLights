#include "LEDMatrixMainDisplay.h"

LEDMatrixMain::LEDMatrixMain(OGLRenderContext* other, InputManager* i_other){

}
	
void LEDMatrixMain::addNode(LEDMatrixNode* node, short connectionFlag, int start, int end){
	/*nodes.push_back(node);
	if(nodes.size()>0){
		if(connectionFlag!=0){
			switch(connectionFlag){
			case FLAG_TOP:
				if(end-start<node->width){
					nodeConnections.push_back(nodeConnection(make_pair(FLAG_TOP,node),make_pair(start,end)));
				}
			/*case FLAG_LEFT:
			case FLAG_RIGHT:
			case FLAG_BOTTOM:*
			}
		}
	}*/
}

DisplayLED* LEDMatrixMain::getLED(int x, int y){
	/*if(y>=nodes[0]->height){
		for(int i=0; i<nodeConnections.size(); i++){
			nodeConnection node = nodeConnections[i];
			if(node.first.first==FLAG_TOP){
				if(x>=node.second.first&&x<=node.second.first){
					int row = y-nodes[0]->height;
					return &(node.first.second)->ledMatrix[x][row];
				}
			}
		}
	}*/
	return 0;
}

void LEDMatrixMain::rePaintScreen(){
	for(int i=0; i<nodes.size(); i++){
		nodes[i]->rePaintScreen();
	}
}

void LEDMatrixMain::renderLEDS(){
	/*for(int i=0; i<nodes.size(); i++){
		nodes[i]->renderLEDS();
	}*/
}