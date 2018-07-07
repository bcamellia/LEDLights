#ifndef RENDERER_H
#define RENDERER_H

#include <iostream>
#include <stdio.h>
#include <windows.h>
#include <TCHAR.h>
#include <vector>
#include <cassert>
#include <string>
#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include "OGLES2Tools.h"

using namespace std;

#include <direct.h>
#define GetCurrentDir _getcwd

#define STRINGIFY(A) #A;

#define WINDOW_TITLE_PREFIX "Chapter 1"
#define WINDOW_TITLE "LED Test"
#define WINDOW_WIDTH 600
#define WINDOW_HEIGHT 600

#define VERTEX_ARRAY 0
#define TEXCOORD_ARRAY 1
#define COLOR_ARRAY 2
#define LED_LIGHTING_ARRAY 3

#define M_PROJ PVRTMat4::Ortho(-1.0,1.0,1.0,-1.0,0.0, 1.0f,PVRTMat4::OGL)

void ResizeFunction(int, int);
void RenderFunction(void);
void IdleFunction(void);
void TimerFunction(int);

struct shaderProgram
{
	GLuint id;
	GLuint vertShader, fragShader;
	GLuint* uniLoc;
	void Destroy(){
		if(&uniLoc){
			delete [] uniLoc;
		}
		GLenum ErrorCheckValue = glGetError();
		glUseProgram(0);
		glDetachShader(id, vertShader);
		glDetachShader(id, fragShader);
		glDeleteShader(fragShader);
		glDeleteShader(vertShader);
		glDeleteProgram(id);
		ErrorCheckValue = glGetError();
		if (ErrorCheckValue != GL_NO_ERROR)
		{
			cout<<"ERROR: Could not destroy the shaders: %s \n";
			exit(-1);
		}

	}
};


class OGLRenderContext{
public:
	shaderProgram* LEDDisplayShader;
	shaderProgram* LEDBorderShader;
	shaderProgram* pinNodeShader;
	shaderProgram* propelShader;

	bool init(HINSTANCE hInstance, HINSTANCE hPrevInstance, TCHAR *lpCmdLine, int nCmdShow);
	void initShaders();
	shaderProgram* compileShader(const char* pszVertShader, const char* pszFragShader, vector<pair<int,const char*> > attribs);
	void addInputManager(void* i);
	void destroyShaders();
	void destroyWindow();
	void loadTextures();
	void swapBuffers();
	//LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	HWND hWnd;
	HDC	hDC;

	EGLDisplay eglDisplay;
	EGLConfig eglConfig;
	EGLSurface eglSurface;
	EGLContext eglContext;
	EGLNativeWindowType	eglWindow;

	GLuint* TEX_LED_LIT;
	GLuint* TEX_LED_BLANK;
	GLuint* TEX_LED_GLOW;

};

#endif