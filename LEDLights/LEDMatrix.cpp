#include "LEDMatrix.h"
#include "PVRTMatrix.h"

int lo=0; int ko=0;
float rot = 0.0f;
int stride = 4;
int time = 0;

LEDMatrix::LEDMatrix(OGLRenderContext* other,InputManager* i_other, int w, int h, float ul_xpos, float ul_ypos, PhysicsWorld* pWorld){

	width = w;
	height = h;

	world = pWorld;

	OGLRenderer = other;
	input = i_other;

	xCornerOffset = 0;
	yCornerOffset = 0;

	float halfWidth = lightSize*width/2;
	float halfHeight = lightSize*height/2;

	//pinsAreConnected = false;
	//bounds = new BoundingBox(PVRTVec2(0,h*lightSize),PVRTVec2(w*lightSize,h*lightSize),PVRTVec2(0,0),PVRTVec2(w*lightSize,0));
	//bounds->translate(ul_xpos,ul_ypos);

	ledMatrix = new DisplayLED*[width];
	for(int i=0; i<width; i++){
		ledMatrix[i] = new DisplayLED[height];
	}

	GLfloat border[8] = {xCornerOffset-borderWidth-halfWidth,yCornerOffset-borderWidth-halfHeight,
		(xCornerOffset+lightSize*width)+borderWidth-halfWidth,yCornerOffset-borderWidth-halfHeight,
		xCornerOffset-borderWidth-halfWidth,(yCornerOffset+lightSize*height)+borderWidth-halfHeight,
		(xCornerOffset+lightSize*width)+borderWidth-halfWidth,(yCornerOffset+lightSize*height)+borderWidth-halfHeight};

	GLfloat origverts[8] = {xCornerOffset-halfWidth,yCornerOffset-halfHeight, // Position
		xCornerOffset+lightSize-halfWidth,yCornerOffset-halfHeight,
		xCornerOffset-halfWidth,yCornerOffset+lightSize-halfHeight,
		xCornerOffset+lightSize-halfWidth,yCornerOffset+lightSize-halfHeight};

	float incr = lightSize;
	
	std::vector<GLfloat> tex;
	std::vector<PVRTVec2> verts;
	std::vector<unsigned short> indices;
	int numLED = height*width;
	//verts.reserve(numLED);
	//indices.resize(numLED*6);


	for(int i=0; i<width; i++){
		for(int j=0; j<height; j++){
			unsigned short index = (unsigned short)verts.size();
			//int ii = (j*height*6)+i*6;

			verts.push_back(PVRTVec2(origverts[0],origverts[1]));//0
			verts.push_back(PVRTVec2(origverts[2],origverts[3]));//1
			verts.push_back(PVRTVec2(origverts[4],origverts[5]));//2
			verts.push_back(PVRTVec2(origverts[6],origverts[7]));//3

			indices.push_back(index);indices.push_back(index+1);indices.push_back(index+2);
			indices.push_back(index+1);indices.push_back(index+2);indices.push_back(index+3);

			DisplayLED led = DisplayLED(ledList.size(),&ledList);
			ledMatrix[i][j] = led;
			Color c = C_RED;
			ledList.push_back(c);ledList.push_back(c);
			ledList.push_back(c);ledList.push_back(c);
			//ledList[ii+4]=(led);ledList[ii+5]=(led);

			tex.push_back(0.0f);tex.push_back(0.0f);tex.push_back(1.0f);tex.push_back(0.0f);
			tex.push_back(0.0f);tex.push_back(1.0f);tex.push_back(1.0f);tex.push_back(1.0f);
			origverts[1]+=incr;origverts[3]+=incr;origverts[5]+=incr;origverts[7]+=incr;
		}
		origverts[1] = yCornerOffset-halfHeight; origverts[0] += incr;
		origverts[3] = yCornerOffset-halfHeight; origverts[2] += incr;
		origverts[5] = yCornerOffset+lightSize-halfHeight; origverts[4] += incr;
		origverts[7] = yCornerOffset+lightSize-halfHeight; origverts[6] += incr;
	}

	indexSize = indices.size();

	mVbo = new GLuint;
	mIndexVbo = new GLuint;
	mTexVbo = new GLuint;
	mBVbo = new GLuint;

	glGenBuffers(1, mVbo);
	glGenBuffers(1, mIndexVbo);
	glGenBuffers(1, mTexVbo);
	glGenBuffers(1, mBVbo);

	glBindBuffer(GL_ARRAY_BUFFER, *mVbo);
	glBufferData(GL_ARRAY_BUFFER, verts.size()*sizeof(PVRTVec2), &verts[0].x, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, *mTexVbo);
	glBufferData(GL_ARRAY_BUFFER, tex.size()*sizeof(GLfloat), &tex[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, *mBVbo);
	glBufferData(GL_ARRAY_BUFFER, 8*sizeof(GLfloat), &border[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *mIndexVbo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(unsigned short), &indices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	xCornerOffset = ul_xpos;
	yCornerOffset = ul_ypos;

	btBoxShape* colShape = new btBoxShape(btVector3(((float)width*lightSize)/2,((float)height*lightSize)/2,.004));
	//btConvexShape* colShape= new btConvex2dShape(box);
	btTransform t;
	t.setIdentity();
	t.setOrigin(btVector3(xCornerOffset,yCornerOffset,0.0));
	btVector3 localInertia(0,0,0);
	//colShape->calculateLocalInertia(1.0,localInertia);
	body = new btCollisionObject();
	body->setCollisionShape(colShape);
	body->setWorldTransform(t);
	world->m_dynamicsWorld->addCollisionObject(body);
	//btTransform g = t.inverse();

	/*for(int i=0; i<height; i++){
	connLookup[FLAG_LEFT][i] = 0;
	connLookup[FLAG_RIGHT][i] = 0;
	}

	btRigidBody* rbA = m_colObj;
	btRigidBody* rbB = m_carBody;
	axisInA = btVector3(0,M_PI/2,0);
	pivotInB = rbB->getCenterOfMassTransform().inverse()(rbA->getCenterOfMassTransform()(pivotInA));
	axisInB = rbB ?
	(rbB->getCenterOfMassTransform().getBasis().inverse()*(rbA->getCenterOfMassTransform().getBasis() * axisInA)) :
	rbA->getCenterOfMassTransform().getBasis() * axisInA;
	btHingeConstraint* m_constraint = new btHingeConstraint(*rbA,*rbB,pivotInA,pivotInB,axisInA,axisInB);*/for(int i=0; i<height; i++){
		DisplayLED* led = &ledMatrix[0][i];
		led->setColor(C_WHITE);
		led->flag=OB_SOLID;
		led->ob = 0;

		led = &ledMatrix[width-1][i];
		led->setColor(C_WHITE);
		led->flag=OB_SOLID;
		led->ob = 0;
	}

	for(int i=0; i<width; i++){
		DisplayLED* led = &ledMatrix[i][0];
		led->setColor(C_WHITE);
		led->flag=OB_SOLID;
		led->ob = 0;

		led = &ledMatrix[i][height-1];
		led->setColor(C_WHITE);
		led->flag=OB_SOLID;
		led->ob = 0;
	}
}

void LEDMatrix::rePaintScreen(){
	for(int i=1;i<width-1; i++){
		for(int j=1; j<height-1; j++){
			DisplayLED* led = &ledMatrix[i][j];
			led->setColor(C_TRANS);
			led->flag=OB_DISPLAY;
			led->ob = 0;
		}
	}
}

void LEDMatrix::checkSelectedPins(pinNode* n, vector<pinNode*>* otherSelPins){}

vector<pinNode*>* LEDMatrix::getArrSelectedPins(){
	return 0;
}

void LEDMatrix::renderLEDS(PVRTMat4 mProj){

	float m[16];
	body->getWorldTransform().getOpenGLMatrix(m);
	PVRTMat4 world = PVRTMat4(m);
	PVRTMat4 matrix = mProj*world;

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnableVertexAttribArray(TEXCOORD_ARRAY);
	glEnableVertexAttribArray(VERTEX_ARRAY);
	glEnableVertexAttribArray(COLOR_ARRAY);

	glUseProgram(OGLRenderer->LEDBorderShader->id);

	glUniformMatrix4fv(OGLRenderer->LEDBorderShader->uniLoc[0], 1, GL_FALSE, matrix.f);

	glBindBuffer(GL_ARRAY_BUFFER, *mBVbo);
	glVertexAttribPointer(VERTEX_ARRAY, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	renderPinNodes(matrix);

	glUseProgram(OGLRenderer->LEDDisplayShader->id);


	glUniformMatrix4fv(OGLRenderer->LEDDisplayShader->uniLoc[0], 1, GL_FALSE, matrix.f);

	glVertexAttribPointer(COLOR_ARRAY, 4, GL_FLOAT, GL_FALSE, sizeof(Color), &(ledList[0].r));

	glBindBuffer(GL_ARRAY_BUFFER, *mTexVbo);
	
	glVertexAttribPointer(TEXCOORD_ARRAY, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, *OGLRenderer->TEX_LED_LIT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glUniform1i(OGLRenderer->LEDDisplayShader->uniLoc[1], 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, *OGLRenderer->TEX_LED_BLANK);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glUniform1i(OGLRenderer->LEDDisplayShader->uniLoc[2], 1);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, *OGLRenderer->TEX_LED_GLOW);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glUniform1i(OGLRenderer->LEDDisplayShader->uniLoc[3], 1);

	glBindBuffer(GL_ARRAY_BUFFER, *mVbo);
	glVertexAttribPointer(VERTEX_ARRAY, 2, GL_FLOAT, GL_FALSE, sizeof(PVRTVec2), 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *mIndexVbo);
	glDrawElements(GL_TRIANGLES, indexSize, GL_UNSIGNED_SHORT, 0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	glDisableVertexAttribArray(COLOR_ARRAY);
	glDisableVertexAttribArray(VERTEX_ARRAY);
	glDisableVertexAttribArray(TEXCOORD_ARRAY);

}