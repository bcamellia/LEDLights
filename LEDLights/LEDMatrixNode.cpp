#include "LEDMatrixNode.h"

LEDMatrixNode::LEDMatrixNode(OGLRenderContext* other, InputManager* i_other, int w, int h, float ul_xpos, float ul_ypos,PhysicsWorld* pWorld) : LEDMatrix(other, i_other, w, h, ul_xpos, ul_ypos,pWorld){
	
	float halfWidth = lightSize*width/2;
	float halfHeight = lightSize*height/2;

	float incr = lightSize;
	int num = (width*16)+height*16;
	int count=0;

	float mHeight = 0+(incr*height)-halfHeight;
	float x = 0+pinOffset-halfWidth;
	float x2 = 0+pinWidth-halfWidth;

	vector<float> pinVertsBuff;
	vector<unsigned short> pinIndices;

	numPinsWidth=width-6;
	numPinsHeight=height-6;

	pinWidthOffset=2;
	pinHeightOffset=2;

	pins = new pinNode**[4];
	pins[FLAG_TOP] = new pinNode*[numPinsWidth];
	pins[FLAG_BOTTOM] = new pinNode*[numPinsWidth];
	pins[FLAG_LEFT] = new pinNode*[numPinsHeight];
	pins[FLAG_RIGHT] = new pinNode*[numPinsHeight];


	btBoxShape* shape = new btBoxShape(btVector3(fabs(x2-x)/2,pinLength/2,.004));
	vector<GLfloat> pv;
	pv.resize(8);

	for (int i=0; i<pinWidthOffset; i++){
		x+=incr; x2+=incr;
	}

	for(int i=0;i<(numPinsWidth)*16;i+=16){
		//Top
		pv[0] = x;			pv[4] = x2;
		pv[1] = mHeight;	pv[5] = mHeight;
		pv[2] = x;			pv[6] = x2;
		pv[3] = mHeight+pinLength;pv[7] = mHeight+pinLength;

		btCollisionObject* obj = new btCollisionObject();
		pinNode* pin = new pinNode(obj,make_pair(FLAG_TOP,i/16),pv,this);
		pin->colObj = obj;
		btTransform t; t.setIdentity();
		t.setOrigin(btVector3(ul_xpos+(x+x2)/2,ul_ypos+mHeight+pinLength/2,0));
		obj->setWorldTransform(t);
		obj->setUserPointer(pin);
		obj->setCollisionShape(shape);
		pins[FLAG_TOP][i/16] = pin;
		world->m_dynamicsWorld->addCollisionObject(obj);

		for(int j=0; j<8; j++){
			pinVertsBuff.push_back(pv[j]);
		}
		//Bottom
		pv[0] = x;			pv[4] = x2;
		pv[1] = 0-halfHeight;	pv[5] = 0-halfHeight;
		pv[2] = x;			pv[6] = x2;
		pv[3] = 0-pinLength-halfHeight;pv[7] = 0-pinLength-halfHeight;

		obj = new btCollisionObject();
		pin = new pinNode(obj,make_pair(FLAG_BOTTOM,i/16),pv,this);
		pin->colObj = obj;
		t; t.setIdentity();
		t.setOrigin(btVector3(ul_xpos+(x+x2)/2,ul_ypos-halfHeight-pinLength/2,0));
		obj->setWorldTransform(t);
		obj->setUserPointer(pin);
		obj->setCollisionShape(shape);
		pins[FLAG_BOTTOM][i/16] = pin;
		world->m_dynamicsWorld->addCollisionObject(obj);
		for(int j=0; j<8; j++){
			pinVertsBuff.push_back(pv[j]);
		}
		x+=incr; x2+=incr;
		pinIndices.push_back(count);pinIndices.push_back(count+1);pinIndices.push_back(count+2);
		pinIndices.push_back(count+1);pinIndices.push_back(count+2);pinIndices.push_back(count+3);
		pinIndices.push_back(count+4);pinIndices.push_back(count+5);pinIndices.push_back(count+6);
		pinIndices.push_back(count+5);pinIndices.push_back(count+6);pinIndices.push_back(count+7);
		count+=8;
	}
	float mWidth = 0+incr*width-halfWidth;
	float y = 0+pinOffset-halfHeight;
	float y2 = 0+pinWidth-halfHeight;

	for(int i=0; i<pinHeightOffset; i++){
		y+=incr; y2+=incr;
	}
	for(int i=width*16;i<(width*16)+(numPinsHeight)*16;i+=16){
		int pinIndex = (i-width*16)/16;
		//Left
		pv[0] = 0-halfWidth;			pv[4] = 0-halfWidth;
		pv[1] = y;									pv[5] = y2;
		pv[2] = 0-pinLength-halfWidth;	pv[6] = 0-pinLength-halfWidth;
		pv[3] = y;									pv[7] = y2;
		btCollisionObject* obj = new btCollisionObject();
		pinNode* pin = new pinNode(obj,make_pair(FLAG_LEFT,pinIndex),pv,this);
		pin->colObj = obj;
		btTransform t; t.setIdentity();
		t.setOrigin(btVector3(ul_xpos-halfWidth-pinLength/2,ul_ypos+(y+y2)/2,0));
		t.setRotation(btQuaternion(0,0,sin(M_PI_4),sin(M_PI_4)));
		obj->setWorldTransform(t);
		obj->setUserPointer(pin);
		obj->setCollisionShape(shape);
		pins[FLAG_LEFT][pinIndex] = pin;
		world->m_dynamicsWorld->addCollisionObject(obj);
		for(int j=0; j<8; j++){
			pinVertsBuff.push_back(pv[j]);
		}
		//Right
		pv[0] = mWidth;				pv[4] = mWidth;
		pv[1] = y;					pv[5] = y2;
		pv[2] = mWidth+pinLength;	pv[6] = mWidth+pinLength;
		pv[3] = y;					pv[7] = y2;

		obj = new btCollisionObject();
		pin = new pinNode(obj,make_pair(FLAG_RIGHT,pinIndex),pv,this);
		pin->colObj = obj;
		t; t.setIdentity();
		t.setOrigin(btVector3(ul_xpos+mWidth+pinLength/2,ul_ypos+(y+y2)/2,0));
		t.setRotation(btQuaternion(0,0,sin(M_PI_4),sin(M_PI_4)));
		obj->setWorldTransform(t);
		obj->setUserPointer(pin);
		obj->setCollisionShape(shape);
		pins[FLAG_RIGHT][pinIndex] = pin;
		world->m_dynamicsWorld->addCollisionObject(obj);
		for(int j=0; j<8; j++){
			pinVertsBuff.push_back(pv[j]);
		}

		y+=incr; y2+=incr;
		pinIndices.push_back(count);pinIndices.push_back(count+1);pinIndices.push_back(count+2);
		pinIndices.push_back(count+1);pinIndices.push_back(count+2);pinIndices.push_back(count+3);
		pinIndices.push_back(count+4);pinIndices.push_back(count+5);pinIndices.push_back(count+6);
		pinIndices.push_back(count+5);pinIndices.push_back(count+6);pinIndices.push_back(count+7);
		count+=8;
	}

	numPinIndices = pinIndices.size();

	mPVbo = new GLuint;
	mPIndexVbo = new GLuint;

	glGenBuffers(1, mPVbo);
	glGenBuffers(1, mPIndexVbo);

	glBindBuffer(GL_ARRAY_BUFFER, *mPVbo);
	glBufferData(GL_ARRAY_BUFFER, num*sizeof(GLfloat), &pinVertsBuff[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *mPIndexVbo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, pinIndices.size()*sizeof(unsigned short), &pinIndices[0], GL_STATIC_DRAW);

	
	connLookup = new nodeConnection**[4];
	connLookup[FLAG_TOP] = new nodeConnection*[width];
	connLookup[FLAG_BOTTOM] = new nodeConnection*[width];
	for(int i=0; i<width; i++){
		connLookup[FLAG_BOTTOM][i] = 0;
		connLookup[FLAG_TOP][i] = 0;
	}
	connLookup[FLAG_LEFT] = new nodeConnection*[height];
	connLookup[FLAG_RIGHT] = new nodeConnection*[height];
	for(int i=0; i<height; i++){
		connLookup[FLAG_LEFT][i] = 0;
		connLookup[FLAG_RIGHT][i] = 0;
	}

	lastSelectedNode = 0;
}

nodeConnection* LEDMatrixNode::addNode(LEDMatrixNode* node, short connectionFlag1, short connectionFlag2, int start1, int end1, int start2, int end2){

	assert(end1-start1==end2-start2);
	nodeConnection* n = 0;

	switch(connectionFlag1){
	case FLAG_TOP:
		n = new nodeConnection(connectionFlag1,connectionFlag2,node,start1,end1,start2,end2,nodeConnections.size());
		for(int i=start1; i<=end1; i++){
			DisplayLED* led = &ledMatrix[i][height-1];
			led->setColor(C_TRANS);
			led->flag=OB_DISPLAY;
		}
		nodeConnections.push_back(n);
		for(int i=start1; i<=end1; i++){
			connLookup[connectionFlag1][i] = n;
		}
		break;
	case FLAG_BOTTOM:

		n = new nodeConnection(connectionFlag1,connectionFlag2,node,start1,end1,start2,end2,nodeConnections.size());
		for(int i=start1; i<=end1; i++){
			DisplayLED* led = &ledMatrix[i][0];
			led->setColor(C_TRANS);
			led->flag=OB_DISPLAY;
		}
		nodeConnections.push_back(n);
		for(int i=start1; i<=end1; i++){
			connLookup[connectionFlag1][i] = n;
		}

		break;
	case FLAG_LEFT:
		n = new nodeConnection(connectionFlag1,connectionFlag2,node,start1,end1,start2,end2,nodeConnections.size());
		for(int i=start1; i<=end1; i++){
			DisplayLED* led = &ledMatrix[0][i];
			led->setColor(C_TRANS);
			led->flag=OB_DISPLAY;
		}
		nodeConnections.push_back(n);
		for(int i=start1; i<=end1; i++){
			connLookup[connectionFlag1][i] = n;
		}
		break;
	case FLAG_RIGHT:

		n = new nodeConnection(connectionFlag1,connectionFlag2,node,start1,end1,start2,end2,nodeConnections.size());
		for(int i=start1; i<=end1; i++){
			DisplayLED* led = &ledMatrix[width-1][i];
			led->setColor(C_TRANS);
			led->flag=OB_DISPLAY;
		}
		nodeConnections.push_back(n);
		for(int i=start1; i<=end1; i++){
			connLookup[connectionFlag1][i] = n;
		}
		break;
	}

	return n;
}

pair<int,int> LEDMatrixNode::getRowColumn(nodeConnection* n, int x, int y){
	int row, column;
	LEDMatrix* o = n->other;
	switch(n->flag1){
	case FLAG_BOTTOM:
		switch(n->flag2){
		case FLAG_BOTTOM:
			row = abs(y);
			column = (x-n->s1)+n->s2;
			break;
		case FLAG_TOP:
			row = o->height+y-1;
			column = (x-n->s1)+n->s2;
			break;
		case FLAG_LEFT:
			row = x-n->s1+n->s2;
			column = abs(y);
			break;
		case FLAG_RIGHT:
			row = x-n->s1+n->s2;
			column = o->width+y-1;
			break;
		}
		break;
	case FLAG_TOP:
		switch(n->flag2){
		case FLAG_BOTTOM:
			row = y-height;
			column = (x-n->s1)+n->s2;
			break;
		case FLAG_TOP:
			row = o->height-(y-height)-1;
			column = (x-n->s1)+n->s2;
			break;
		case FLAG_LEFT:
			row = x-n->s1+n->s2;
			column = y-height;
			break;
		case FLAG_RIGHT:
			row = x-n->s1+n->s2;
			column = o->width-(y-height)-1;
			break;
		}
		break;
	case FLAG_LEFT:
		switch(n->flag2){
		case FLAG_BOTTOM:
			row = abs(x);
			column = (y-n->s1)+n->s2;
			break;
		case FLAG_TOP:
			row = o->height+x-1;
			column = (y-n->s1)+n->s2;
			break;
		case FLAG_LEFT:
			row = (y-n->s1)+n->s2;
			column = abs(x)-1;
			break;
		case FLAG_RIGHT:
			row = (y-n->s1)+n->s2;
			column = o->width+x-1;
			break;
		}
		break;
	case FLAG_RIGHT:
		switch(n->flag2){
		case FLAG_BOTTOM:
			row = x-width;
			column = (y-n->s1)+n->s2;
			break;
		case FLAG_TOP:
			row = o->height-(x-width)-1;
			column = (y-n->s1)+n->s2;
			break;
		case FLAG_LEFT:
			row = (y-n->s1)+n->s2;
			column = x-width;
			break;
		case FLAG_RIGHT:
			row = (y-n->s1)+n->s2;
			column = o->width-(x-width)-1;
			break;
		}
		break;
	}
	return make_pair(row,column);
}

pair<nodeConnection*, pair<int,int> > LEDMatrixNode::getMatrixFromPos(int x, int y){
	nodeConnection* n = 0;
	if(y>=height){
		if(x<width&&x>=0){
			n = connLookup[FLAG_TOP][int(x)];
		}else{return make_pair(n,make_pair(0,0));}
		if(n){
			pair<int,int> rowColumn = getRowColumn(n,x,y);
			return make_pair(n, rowColumn);
		}
		return make_pair(n,make_pair(0,0));
	}else if(y<0){
		if(x<width&&x>=0){
			n = connLookup[FLAG_BOTTOM][int(x)];
		}else{return make_pair(n,make_pair(0,0));}
		if(n){
			pair<int,int> rowColumn = getRowColumn(n,x,y);
			return make_pair(n,rowColumn);
		}
		return make_pair(n,make_pair(0,0));
	}else if(x>=width){
		if(y<height&&y>=0){
			n = connLookup[FLAG_RIGHT][int(y)];
		}else{return make_pair(n,make_pair(0,0));}
		if(n){
			pair<int,int> rowColumn = getRowColumn(n,x,y);
			return make_pair(n, rowColumn);
		}
		return make_pair(n,make_pair(0,0));
	}else if(x<0){
		if(y<height&&y>=0){
			n = connLookup[FLAG_LEFT][int(y)];
		}else{return make_pair(n,make_pair(0,0));}
		if(n){
			pair<int,int> rowColumn = getRowColumn(n,x,y);
			return make_pair(n, rowColumn);
		}
		return make_pair(n,make_pair(0,0));
	}
	else{

		return make_pair(n,make_pair(0,0));
	}

}

DisplayLED* LEDMatrixNode::getLED(int x, int y){
	if(y>=height){
		nodeConnection* n = 0;
		if(x<width&&x>=0){	//make sure isnt out of range
			n = connLookup[FLAG_TOP][int(x)];
		}else{return 0;}
		if(n){
			pair<int,int> rowColumn = getRowColumn(n,x,y);
			int row = rowColumn.first;
			int column = rowColumn.second;
			LEDMatrix* o = n->other;
			if(row<o->height){
				return &o->ledMatrix[column][row];
			}
		}
		return 0;
	}else if(y<0){
		nodeConnection* n = 0;
		if(x<width&&x>=0){
			n = connLookup[FLAG_BOTTOM][int(x)];
		}else{return 0;}
		if(n){
			LEDMatrix* o = n->other;
			pair<int,int> rowColumn = getRowColumn(n,x,y);
			int row = rowColumn.first;
			int column = rowColumn.second;
			if(row>0){
				return &o->ledMatrix[column][row];
			}
		}
		return 0;
	}else if(x>=width){
		nodeConnection* n = 0;
		if(y<height&&y>=0){
			n = connLookup[FLAG_RIGHT][int(y)];
		}else{return 0;}
		if(n){
			LEDMatrix* o = n->other;
			pair<int,int> rowColumn = getRowColumn(n,x,y);
			int row = rowColumn.first;
			int column = rowColumn.second;
			if(row>0){
				return &o->ledMatrix[column][row];
			}
		}
		return 0;
	}else if(x<0){
		nodeConnection* n = 0;
		if(y<height&&y>=0){
			n = connLookup[FLAG_LEFT][int(y)];
		}else{return 0;}
		if(n){
			LEDMatrix* o = n->other;
			pair<int,int> rowColumn = getRowColumn(n,x,y);
			int row = rowColumn.first;
			int column = rowColumn.second;
			//if(row>0){
			return &o->ledMatrix[column][row];
			//}
		}
		return 0;
	}
	//else{
	//if(x>-1&&x<width&&y>-1&&y<height){
	return &ledMatrix[x][y];
	/*}else{
	return 0;
	}*/
	//}

}

vector<pinNode*>* LEDMatrixNode::getArrSelectedPins(){
	if(selectedPinNodes.size()>0){
		return &selectedPinNodes;
	}
	return 0;
}

int LEDMatrixNode::shiftPinsIntoArea(int start, int size, int max, short flag, nodeConnection* comp){
	/*int i;
	int mid = size/2;
	for(i=start-mid; i<start; i++){
		nodeConnection* o = checkNodeisNodeConnection(i,flag);
		if(i<0||(o!=comp&&o!=0)){break;}//comp&&o&&o!=comp
	}
	while(i<0){
		i++;
		mid++;
	}
	int top = (i-1<0) ? 0 : i-1;
	for(i; i<start+mid; i++){
		nodeConnection* o = checkNodeisNodeConnection(i,flag);
		if(i>=max||(o!=comp&&o!=0)){top=start-mid-1;break;}
	}
	int len = (start+mid)-i;
	if(size%2!=0){len++;}
	int ret = (top-mid<0) ? 0 : top-mid;
	if(len==0){return ret;}
	int g = 0;
	int index;
	for(g; g<len; g++){
		index = top-g;
		nodeConnection* o = checkNodeisNodeConnection(index,flag);
		if(index<0||(o!=comp&&o!=0)){return -1;}
	}
	return index;//start-len-top-g;*/
	for(int i=start; i<start+size; i++){
		nodeConnection* o = checkNodeisNodeConnection(i,flag);
		if(i>=max||(o!=comp&&o!=0)){return -1;}//comp&&o&&o!=comp
	}
	return start;

}

nodeConnection* LEDMatrixNode::checkNodeisNodeConnection(int pos, short flag){
	pos+=pinHeightOffset;
	if(nodeConnections.size()>0){
		for(int i=nodeConnections.size()-1; i>=0; i--){
			nodeConnection* testNode = nodeConnections[i];
			if(testNode->flag1==flag&&testNode->connectedVertsLookup.find(pos)!=testNode->connectedVertsLookup.end()){
				return testNode;
			}
		}
	}
	return 0;
}


void LEDMatrixNode::checkSelectedPins(pinNode* n, LEDMatrixNode* otherSelDisplay){

	if(input->keyDown(input->DELETE_KEY)&&selectedConnection){
		removeNodeConnection(selectedConnection);
	}

	if(n){
		bool isPartOfConnectedNodes = false;
		nodeConnection* node = 0;
		int index = 0;
		nodeConnection* testNode = 0;
		//if(lastSelectedNode){
		testNode=checkNodeisNodeConnection(n->xy.second,n->xy.first);
		//}
		if(testNode||selectedConnection){
			if(!selectedConnection){
				isPartOfConnectedNodes = true;
				node = testNode;
			}else if(n!=lastSelectedNode&&n->ob == this){//&&selectedConnection == testNode){
				shiftConnectedPins(selectedConnection->index,n);
				lastSelectedNode = n;
				return;
			}
		}else{
			/*testNode=checkNodeisNodeConnection(n->xy.second,n->xy.first);
			if(testNode&&!selectedConnection){
			isPartOfConnectedNodes = true;
			node = testNode;
			}else if(testNode&&n->ob == this&&selectedConnection == testNode){
			shiftConnectedPins(testNode->index,n);
			lastSelectedNode = n;
			return;
			}*/
		}
		if(((selectedPinNodes.size()==0&&n->ob==this)||(selectedPinNodes.size()>0&&n->ob!=selectedPinNodes[0]->ob))&&otherSelDisplay==NULL){
			if(!isPartOfConnectedNodes&&!selectedConnection){
				selectedPinVerts.resize(4);
				selectedPinNodes.resize(0);
				float* verts = n->getPoint();
				selectedPinVerts[0]=verts[0];
				selectedPinVerts[1]=verts[1];
				selectedPinNodes.push_back(n);
				selectedVertsLookup[0] = 2;
			}else if(!selectedConnection){
				selectedConnection = node;
				LEDMatrixNode* other = static_cast<LEDMatrixNode*>(node->other);
				other->selectedConnection = other->nodeConnections[node->index];
			}
		}else if(selectedPinNodes.size()>0&&selectedPinNodes[0]->ob==n->ob&&n->ob==this){
			if (n->xy.first!=selectedPinNodes[0]->xy.first){
				return;
			}
			pinNode* eq = 0;

			float* verts = n->getPoint();

			short FLAG = selectedPinNodes.front()->xy.first;

			pinNode* f = (selectedPinNodes.front()->xy.second<n->xy.second) ? selectedPinNodes.front() : n;
			pinNode* back = selectedPinNodes.back();
			pinNode* l = (selectedPinNodes.back()->xy.second>n->xy.second) ? selectedPinNodes.back() : n;
			pinNode* min = (f->xy.second<l->xy.second) ? f : l;
			pinNode* max = (min==f) ? l : f;

			for(int i=min->xy.second; i<=max->xy.second; i++){
				if(checkNodeisNodeConnection(i,FLAG)){
					return;
				}
			}

			int offset = 0;
			for(int i=min->xy.second; i<=max->xy.second; i++){
				pinNode* next_n = pins[FLAG][i];
				for(int j=0; j<selectedPinNodes.size(); j++){
					eq = selectedPinNodes[j];
					if(eq == next_n){
						break;
					}
				}
				if(eq!=next_n){
					verts = next_n->getPoint();
					if(next_n->xy.second>back->xy.second){
						selectedPinNodes.push_back(next_n);
						selectedVertsLookup[selectedVertsLookup.size()] = selectedPinVerts.size()+2;
					}else{
						selectedPinNodes.insert(selectedPinNodes.begin()+offset,next_n);
						int size = selectedVertsLookup.size();
						map<int, int> copy = selectedVertsLookup;
						for(int l=offset; l<size; l++){
							selectedVertsLookup[l+1] = copy[l];
						}
						selectedVertsLookup[offset] = selectedPinVerts.size()+2;
						offset++;
					}
					selectedPinVerts.push_back(verts[0]);
					selectedPinVerts.push_back(verts[1]);
					selectedPinVerts.push_back(0);selectedPinVerts.push_back(0);
				}
			}
		}else if(n->ob==this&&selectedPinNodes.size()==0&&otherSelDisplay){

			if(otherSelDisplay!=this){
				pinNode* first = otherSelDisplay->selectedPinNodes[0];
				pinNode* min = first;
				pinNode* max = first;
				for(int i=1; i<otherSelDisplay->selectedPinNodes.size(); i++){
					if(otherSelDisplay->selectedPinNodes[i]->xy.second<min->xy.second){
						min = otherSelDisplay->selectedPinNodes[i];
					}
					if(otherSelDisplay->selectedPinNodes[i]->xy.second>max->xy.second){
						max = otherSelDisplay->selectedPinNodes[i];
					}
				}

				int size = otherSelDisplay->selectedPinNodes.size();
				int s1, s2, e1, e2;
				s2 = min->xy.second; e2 = max->xy.second;
				e1 = size-1;

				float m[16];
				float om[16];
				body->getWorldTransform().getOpenGLMatrix(m);
				otherSelDisplay->body->getWorldTransform().inverse().getOpenGLMatrix(om);
				PVRTMat4 world = PVRTMat4(om)*PVRTMat4(m);
				bool diffSides = n->xy.first%2==max->xy.first%2 ? false : true;
				bool outSideBounds = false;
				int offset;

				switch(n->xy.first){
				case FLAG_BOTTOM:
				case FLAG_TOP:

					s1 = shiftPinsIntoArea(n->xy.second,size,numPinsWidth,n->xy.first,0);
					if(s1<0){return;}

					offset = pinWidthOffset;

					for(int i=s1; i<s1+size; i++){
						int v1;

						v1 = otherSelDisplay->selectedVertsLookup[i-s1];

						int v2 = v1+1;
						pinNode* node = pins[n->xy.first][i];
						float* verts = node->getPoint();
						PVRTVec4 v = (M_PROJ*world)*PVRTVec4(verts[0],verts[1],0,1); //Transform into other display's view
						otherSelDisplay->selectedPinVerts[v1] = v.x;
						otherSelDisplay->selectedPinVerts[v2] = v.y;
					}

					break;
				case FLAG_RIGHT:
				case FLAG_LEFT:


					s1 = shiftPinsIntoArea(n->xy.second,size,numPinsHeight,n->xy.first,0);
					if(s1<0){return;}
					offset = pinHeightOffset;

					for(int i=s1; i<s1+size; i++){
						int v1;
						v1 = otherSelDisplay->selectedVertsLookup[i-s1];
						int v2 = v1+1;
						pinNode* node = pins[n->xy.first][i];
						float* verts = node->getPoint();
						PVRTVec4 v = (M_PROJ*world)*PVRTVec4(verts[0],verts[1],0,1); //Transform into other display's view
						otherSelDisplay->selectedPinVerts[v1] = v.x;
						otherSelDisplay->selectedPinVerts[v2] = v.y;
					}

					break;
				}

				//s1+=offset;
				//e1+=offset;

			
					//s2 += otherSelDisplay->pinHeightOffset;
					//e2 += offset;
					
				
				nodeConnection* node_c = addNode(otherSelDisplay,n->xy.first,max->xy.first,s1,e1+s1,s2,e2);
				nodeConnection* o_node_c = otherSelDisplay->addNode(this,max->xy.first,n->xy.first,s2,e2,s1,e1+s1);


				map<int, int>::iterator it;

				for(it = otherSelDisplay->selectedVertsLookup.begin(); it!= otherSelDisplay->selectedVertsLookup.end(); it++){
					node_c->connectedVertsLookup.insert(make_pair(s1+it->first,it->second));
				}
				for(it = otherSelDisplay->selectedVertsLookup.begin(); it!= otherSelDisplay->selectedVertsLookup.end(); it++){
					o_node_c->connectedVertsLookup.insert(make_pair(s2+it->first,it->second-2));
				}
				o_node_c->connectedPinVerts = otherSelDisplay->selectedPinVerts;

				otherSelDisplay->selectedPinNodes.clear();
				otherSelDisplay->selectedPinVerts.clear();
				otherSelDisplay->selectedVertsLookup.clear();

			}
		}

		lastSelectedNode = n;
		return;
	}else if(!input->isMouseDown){
		selectedVertsLookup.clear();
		selectedPinVerts.clear();
		selectedPinNodes.clear();
		lastSelectedNode = 0;
		selectedConnection = 0;
	}

}

bool LEDMatrixNode::canPlaceNodeConnection(int s1, int s2, nodeConnection* node, short flag){
	nodeConnection* comp;
	for(int i = s1; i<s2; i++){
		comp = checkNodeisNodeConnection(i,flag);
		if(comp&&comp!=node){
			return false;
		}
	}
	return true;
}

void LEDMatrixNode::shiftConnectedPins(int index, pinNode* n){
	nodeConnection* nodeConn = nodeConnections[index];
	LEDMatrixNode* otherLED = static_cast<LEDMatrixNode*>(nodeConn->other);
	nodeConnection* otherNodeConn = otherLED->nodeConnections[index];


	int s2 = nodeConn->s2;
	int e2 = nodeConn->e2;
	int size = otherNodeConn->connectedVertsLookup.size();
	int s1;
	int offset = nodeConn->s1;
	float m[16];
	float om[16];

	bool diffSides = n->xy.first%2==nodeConn->flag2%2 ? false : true;
	bool verticesHost = nodeConn->connectedPinVerts.size()>0;
	bool outSideBounds = false;

	PVRTMat4 world;

	if(!verticesHost){
		body->getWorldTransform().getOpenGLMatrix(m);
		otherLED->body->getWorldTransform().inverse().getOpenGLMatrix(om);
		world = PVRTMat4(om)*PVRTMat4(m);
	}

	switch(n->xy.first){
	case FLAG_BOTTOM:
	case FLAG_TOP:

		s1 = shiftPinsIntoArea(n->xy.second,size,numPinsWidth,n->xy.first,nodeConn);
		if(s1<0){return;}

		for(int i=0; i<size; i++){
			int v1;

			v1 = nodeConn->connectedVertsLookup[i+offset];

			int v2 = v1+1;
			pinNode* node = pins[n->xy.first][i+s1];

			float* verts = node->getPoint();
			if(verticesHost){
				nodeConn->connectedPinVerts[v1]=verts[0];
				nodeConn->connectedPinVerts[v2]=verts[1];
			}else{
				PVRTVec4 v = (M_PROJ*world)*PVRTVec4(verts[0],verts[1],0,1); //Transform into other display's view
				otherNodeConn->connectedPinVerts[v1]=v.x;
				otherNodeConn->connectedPinVerts[v2]=v.y;
			}
		}

		break;
	case FLAG_RIGHT:
	case FLAG_LEFT:

		s1 = shiftPinsIntoArea(n->xy.second,size,numPinsHeight,n->xy.first,nodeConn);
					
		if(s1<0){return;}

		for(int i=0; i<size; i++){
			int v1;

			v1 = nodeConn->connectedVertsLookup[i+offset];

			int v2 = v1+1;
			pinNode* node = pins[n->xy.first][i+s1];
			float* verts = node->getPoint();
			if(verticesHost){
				nodeConn->connectedPinVerts[v1]=verts[0];
				nodeConn->connectedPinVerts[v2]=verts[1];
			}else{
				PVRTVec4 v = (M_PROJ*world)*PVRTVec4(verts[0],verts[1],0,1); //Transform into other display's view
				otherNodeConn->connectedPinVerts[v1]=v.x;
				otherNodeConn->connectedPinVerts[v2]=v.y;
			}
		}

		break;
	}

	map<int, int>::iterator it;
	map<int, int> copy = nodeConn->connectedVertsLookup;
	nodeConn->connectedVertsLookup.clear();
	int i = 0;

	for(it = copy.begin(); it!= copy.end(); it++){
		nodeConn->connectedVertsLookup.insert(make_pair(i+s1,it->second));
		i++;
	}

	shiftNodeConnection(nodeConn, otherNodeConn,n->xy.first,s1,size);

}

void LEDMatrixNode::removeNodeConnection(nodeConnection* node){
	removeNodeFromLookup(node);
	nodeConnections.erase(nodeConnections.begin()+node->index);
	LEDMatrixNode* other = static_cast<LEDMatrixNode*>(node->other);
	other->removeNodeFromLookup(other->nodeConnections[node->index]);
	other->nodeConnections.erase(other->nodeConnections.begin()+node->index);
	selectedConnection=0;
	other->selectedConnection=0;
	for(int i=node->index; i<nodeConnections.size(); i++){ //Update other nodes' indices
		nodeConnections[i]->index--;
		other->nodeConnections[i]->index--;
	}
}

void LEDMatrixNode::removeNodeFromLookup(nodeConnection* node){
	DisplayLED* led;
	
		switch(node->flag1){
		case FLAG_LEFT:
			for(int i = node->s1; i < node->e1+pinHeightOffset; i++){
		connLookup[node->flag1][i] = 0;
			led = &ledMatrix[0][i];
			led->setColor(C_WHITE);
			led->flag=OB_SOLID;
			}
			break;
		case FLAG_RIGHT:
			for(int i = node->s1; i < node->e1+pinHeightOffset; i++){
		connLookup[node->flag1][i] = 0;
			led = &ledMatrix[width-1][i];
			led->setColor(C_WHITE);
			led->flag=OB_SOLID;
			}
			break;
		case FLAG_TOP:
			for(int i = node->s1; i < node->e1+pinWidthOffset; i++){
			connLookup[node->flag1][i] = 0;
			led = &ledMatrix[i][height-1];
			led->setColor(C_WHITE);
			led->flag=OB_SOLID;
			}
			break;
		case FLAG_BOTTOM:
			for(int i = node->s1; i < node->e1+pinWidthOffset; i++){
		connLookup[node->flag1][i] = 0;
			led = &ledMatrix[i][0];
			led->setColor(C_WHITE);
			led->flag=OB_SOLID;
			}
			break;
		}
}

void LEDMatrixNode::rePaintLEDRange(int start, int end, short flag,nodeConnection* node){
	DisplayLED* led;
		switch(flag){
		case FLAG_LEFT:
			for(int i=start+pinHeightOffset; i<end+pinHeightOffset; i++){
			connLookup[flag][i] = node;
			led = &ledMatrix[0][i];
			led->setColor(C_TRANS);
			led->flag=OB_DISPLAY;
			led->ob=0;
			}
			break;
		case FLAG_RIGHT:
			for(int i=start+pinHeightOffset; i<end+pinHeightOffset; i++){
			connLookup[flag][i] = node;
			led = &ledMatrix[width-1][i];
			led->setColor(C_TRANS);
			led->flag=OB_DISPLAY;
			led->ob=0;
			}
			break;
		case FLAG_TOP:
			for(int i=start+pinWidthOffset; i<end+pinWidthOffset; i++){
			connLookup[flag][i] = node;
			led = &ledMatrix[i][height-1];
			led->setColor(C_TRANS);
			led->flag=OB_DISPLAY;
			led->ob=0;
			}
			break;
		case FLAG_BOTTOM:
			for(int i=start+pinWidthOffset; i<end+pinWidthOffset; i++){
			connLookup[flag][i] = node;
			led = &ledMatrix[i][0];
			led->setColor(C_TRANS);
			led->flag=OB_DISPLAY;
			led->ob=0;
			}
			break;
		}	
}

void LEDMatrixNode::shiftNodeConnection(nodeConnection* node, nodeConnection* oNode, short newFlag, int start, int size){

	removeNodeFromLookup(node);
	rePaintLEDRange(start,start+size,newFlag,node);

	node->s1 = start;
	node->e1 = start+size;
	node->flag1 = newFlag;
	oNode->s2 = start;
	oNode->e2 = start+size;
	oNode->flag2 = newFlag;
}

void LEDMatrixNode::rePaintScreen(){
	__super::rePaintScreen();
	DisplayLED* led;
	for(int i=0; i<nodeConnections.size(); i++){
		nodeConnection* node = nodeConnections[i];
		rePaintLEDRange(node->s1,node->e1,node->flag1,node);
	}
}

void LEDMatrixNode::renderPinNodes(PVRTMat4 mWorld){

	glUseProgram(OGLRenderer->pinNodeShader->id);

	glUniformMatrix4fv(OGLRenderer->pinNodeShader->uniLoc[0], 1, GL_FALSE, mWorld.f);
	Color sc = C_RED;
	Color rc = C_BLACK;
	glLineWidth(2.0);

	glUniform4fv(OGLRenderer->pinNodeShader->uniLoc[1], 1, &sc.r);
	if(selectedPinNodes.size()>0){
		for(int i=2; i<selectedPinVerts.size(); i+=4){
			selectedPinVerts[i] = input->mouse_x-xCornerOffset;
			selectedPinVerts[i+1] = input->mouse_y-yCornerOffset;
		}
		glVertexAttribPointer(VERTEX_ARRAY, 2, GL_FLOAT, GL_FALSE, 0, &selectedPinVerts[0]);
		glDrawArrays(GL_LINES, 0, selectedPinVerts.size()/2);
	}

	glUniform4fv(OGLRenderer->pinNodeShader->uniLoc[1], 1, &rc.r);
	for(int i=0; i<nodeConnections.size(); i++){
		nodeConnection* node = nodeConnections[i];
		if(node->connectedPinVerts.size()>0){
			if(selectedConnection == node){
				glUniform4fv(OGLRenderer->pinNodeShader->uniLoc[1], 1, &sc.r);
				glVertexAttribPointer(VERTEX_ARRAY, 2, GL_FLOAT, GL_FALSE, 0, &node->connectedPinVerts[0]);
				glDrawArrays(GL_LINES, 0, node->connectedPinVerts.size()/2);
				glUniform4fv(OGLRenderer->pinNodeShader->uniLoc[1], 1, &rc.r);
				continue;
			}
			glVertexAttribPointer(VERTEX_ARRAY, 2, GL_FLOAT, GL_FALSE, 0, &node->connectedPinVerts[0]);
			glDrawArrays(GL_LINES, 0, node->connectedPinVerts.size()/2);
		}
	}

	glUseProgram(OGLRenderer->LEDBorderShader->id);

	glBindBuffer(GL_ARRAY_BUFFER, *mPVbo);
	glVertexAttribPointer(VERTEX_ARRAY, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *mPIndexVbo);
	glDrawElements(GL_TRIANGLES, numPinIndices, GL_UNSIGNED_SHORT, 0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}