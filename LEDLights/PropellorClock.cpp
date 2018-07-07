#include "PropellorClock.h"

float rota = 0;

PropellorClock::PropellorClock(OGLRenderContext* OGLRenderer, float x, float y, float width, float height){
	this->OGLRenderer = OGLRenderer;
	this->width = width;
	this->height = height;
	GLfloat border[8] = {x,y,x+width,y,x,y+height,x+width,y+height};
	GLfloat leds[40];
	GLfloat tex[40];
	vector<float> indices;

	float halfw = width/2;
	float halfs = propLightSize/2;
	int j=0;
	for(float i = y; i<y+(propLightSize*5); i+=propLightSize){
		leds[j*8] = x+halfw-halfs;
		leds[j*8+1] = i;
		leds[j*8+2] = x+halfw+halfs;
		leds[j*8+3] = i;
		leds[j*8+4] = x+halfw-halfs;
		leds[j*8+5] = i+propLightSize;
		leds[j*8+6] = x+halfw+halfs;
		leds[j*8+7] = i+propLightSize;
		short index = j*4;
		Color c = C_RED;
		chars.push_back(c);chars.push_back(c);chars.push_back(c);chars.push_back(c);
		indices.push_back(index);indices.push_back(index+1);indices.push_back(index+2);
		indices.push_back(index+1);indices.push_back(index+2);indices.push_back(index+3);

		tex[j*8]=0.0f;tex[j*8+1]=0.0f;tex[j*8+2]=1.0f;tex[j*8+3]=0.0f;
		tex[j*8+4]=0.0f;tex[j*8+5]=1.0f;tex[j*8+6]=1.0f;tex[j*8+7]=1.0f;

		j++;
	}

	indexSize = indices.size();

	mSurfVbo = new GLuint;
	mSurfIndexVbo = new GLuint;
	mLEDVbo = new GLuint;
	mLEDIndexVbo = new GLuint;
	mTexVbo = new GLuint;

	glGenBuffers(1, mSurfVbo);
	glGenBuffers(1, mSurfIndexVbo);
	glGenBuffers(1, mLEDVbo);
	glGenBuffers(1, mLEDIndexVbo);
	glGenBuffers(1, mTexVbo);

	glBindBuffer(GL_ARRAY_BUFFER, *mSurfVbo);
	glBufferData(GL_ARRAY_BUFFER,sizeof(GLfloat)*8,&border[0],GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, *mLEDVbo);
	glBufferData(GL_ARRAY_BUFFER,sizeof(GLfloat)*40,&leds[0],GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, *mTexVbo);
	glBufferData(GL_ARRAY_BUFFER,sizeof(GLfloat)*40,&tex[0],GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *mLEDIndexVbo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(GLfloat)*indexSize,&indices[0],GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void PropellorClock::render(){
	glEnableVertexAttribArray(VERTEX_ARRAY);
	
	glEnable(GL_TEXTURE_2D);

	glUseProgram(OGLRenderer->LEDBorderShader->id);

	PVRTMat4 m = M_PROJ*PVRTMat4::RotationZ(rota);

	rota+=.2;

	glUniformMatrix4fv(OGLRenderer->LEDBorderShader->uniLoc[0], 1, GL_FALSE, m.f);

	glBindBuffer(GL_ARRAY_BUFFER, *mSurfVbo);
	glVertexAttribPointer(VERTEX_ARRAY, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glUseProgram(OGLRenderer->propelShader->id);

	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glUniformMatrix4fv(OGLRenderer->propelShader->uniLoc[0], 1, GL_FALSE, m.f);

	glEnableVertexAttribArray(COLOR_ARRAY);
	glVertexAttribPointer(COLOR_ARRAY, 4, GL_FLOAT, GL_FALSE, sizeof(Color), &(chars[0].r));

	glEnableVertexAttribArray(TEXCOORD_ARRAY);

	glBindBuffer(GL_ARRAY_BUFFER, *mTexVbo);
	glVertexAttribPointer(TEXCOORD_ARRAY, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, *OGLRenderer->TEX_LED_LIT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glUniform1i(OGLRenderer->propelShader->uniLoc[1], 0);
	
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, *OGLRenderer->TEX_LED_BLANK);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glUniform1i(OGLRenderer->propelShader->uniLoc[2], 1);

	glBindBuffer(GL_ARRAY_BUFFER, *mLEDVbo);
	glVertexAttribPointer(VERTEX_ARRAY, 2, GL_FLOAT, GL_FALSE, 0, 0);

	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *mLEDIndexVbo);
	//glDrawElements(GL_TRIANGLES, indexSize, GL_UNSIGNED_SHORT, 0);
	glDrawArrays(GL_TRIANGLES, 0, 30);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	glDisableVertexAttribArray(VERTEX_ARRAY);
	glDisableVertexAttribArray(TEXCOORD_ARRAY);
	glDisableVertexAttribArray(COLOR_ARRAY);
}