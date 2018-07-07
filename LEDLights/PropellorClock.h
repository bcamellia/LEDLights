#ifndef PROPELLORCLOCK_H
#define PROPELLORCLOCK_H
#include "Renderer.h"
#include "LEDStuff.h"
//4x5 pixel letters
const float propLightSize = 0.025f;

struct LEDChar{
	Color* colors;
	LEDChar(char c){
		colors = new Color[4];
		for(int i=0; i<4; i++){
			colors[i] = C_RED;
		}
	};
};

class PropellorClock{
public:
	PropellorClock(OGLRenderContext* OGLRenderer, float x, float y, float width, float height);
	float width, height;
	OGLRenderContext* OGLRenderer;
	GLuint* mSurfVbo;
	GLuint* mSurfIndexVbo;
	GLuint* mLEDVbo;
	GLuint* mLEDIndexVbo;
	GLuint* mTexVbo;
	int indexSize;
	vector<Color> chars;
	void render();
};

#endif