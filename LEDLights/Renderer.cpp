#include "Renderer.h"
#include "Input.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
InputManager* manager;
void OGLRenderContext::addInputManager(void* i){
	manager = static_cast<InputManager*>(i);
}

bool TestEGLError(HWND hWnd, char* pszLocation)
{
	/*
		eglGetError returns the last error that has happened using egl,
		not the status of the last called function. The user has to
		check after every single egl call or at least once every frame.
	*/
	EGLint iErr = eglGetError();
	if (iErr != EGL_SUCCESS)
	{
#ifndef NO_GDI
		TCHAR pszStr[256];
		_stprintf(pszStr, _T("%s failed (%d).\n"), pszLocation, iErr);
		MessageBox(hWnd, pszStr, _T("Error"), MB_OK|MB_ICONEXCLAMATION);
#endif
		return false;
	}

	return true;
}

#ifndef NO_GDI
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		/*
			Here we are handling 2 system messages: screen saving and monitor power.
			They are especially relevent on mobile devices.
		*/
#ifndef UNDER_CE
		case WM_SYSCOMMAND:
		{
			switch (wParam)
			{
				case SC_SCREENSAVE:					// Screensaver trying to start ?
				case SC_MONITORPOWER:				// Monitor trying to enter powersave ?
				return 0;							// Prevent this from happening
			}
			break;
		}
#endif
		// Handles the close message when a user clicks the quit icon of the window
		case WM_CLOSE:
			{
			//g_bDemoDone = true;
			PostQuitMessage(0);
			return 1;
		}
		case WM_KEYUP:{
			//short y= wParam;
			//manager->key=0;
			manager->key[wParam]=0;
			break;
		}
		case WM_KEYDOWN:{
			//if(!manager->isKeyDown){
				manager->key[wParam] = 1;
				//manager->keysDown.push_back(wParam);
			//}
			break;
		}
		case WM_LBUTTONDOWN:
		{
			//doButtonDown(hWnd,pData,ePVRShellButtonLeft,lParam);
			//pData->m_pShell->updateMousePosition(hWnd,lParam,1);
			manager->isMouseDown = true;
			manager->updateMousePos(hWnd,lParam);
			break;
		}
	case WM_LBUTTONUP:
		{
			//pData->m_pShell->updateMousePosition(hWnd,lParam,2);
			//if(!doButtonUp(hWnd,pData,ePVRShellButtonLeft))
			//	return false;
			manager->isMouseDown = false;
		break;
		}
	/*case WM_RBUTTONDOWN:
		{
			doButtonDown(hWnd,pData,ePVRShellButtonRight,lParam);
			break;
		}
	case WM_RBUTTONUP:
		{
			if(!doButtonUp(hWnd,pData,ePVRShellButtonRight))
				return false;
			break;
		}
	case WM_MBUTTONDOWN:
		{
			doButtonDown(hWnd,pData,ePVRShellButtonMiddle,lParam);
			break;
		}
	case WM_MBUTTONUP:
		{
			if(!doButtonUp(hWnd,pData,ePVRShellButtonMiddle))
				return false;
			break;
		}*/
	case WM_MOUSEMOVE:
		{
			manager->updateMousePos(hWnd,lParam);	
		}
		default:
			break;
	}

	// Calls the default window procedure for messages we did not handle
	return DefWindowProc(hWnd, message, wParam, lParam);
}
#endif

bool OGLRenderContext::init(HINSTANCE hInstance, HINSTANCE hPrevInstance, TCHAR *lpCmdLine, int nCmdShow){
	
	/*
		Step 0 - Create a EGLNativeWindowType that we can use for OpenGL ES output
	*/
#ifndef NO_GDI
	// Register the windows class
	WNDCLASS sWC;
    sWC.style = CS_HREDRAW | CS_VREDRAW;
	sWC.lpfnWndProc = WndProc;
    sWC.cbClsExtra = 0;
    sWC.cbWndExtra = 0;
    sWC.hInstance = hInstance;
    sWC.hIcon = 0;
    sWC.hCursor = 0;
    sWC.lpszMenuName = 0;
	sWC.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);
    sWC.lpszClassName = WINDOW_TITLE_PREFIX;
	unsigned int nWidth = WINDOW_WIDTH;
	unsigned int nHeight = WINDOW_HEIGHT;

	ATOM registerClass = RegisterClass(&sWC);
	if (!registerClass)
	{
		MessageBox(0, _T("Failed to register the window class"), _T("Error"), MB_OK | MB_ICONEXCLAMATION);
	}
#if defined(UNDER_CE)
	// Get the monitor dimensions
	{
		HMONITOR	hMonitor;
		BOOL		bRet;
		POINT		p;
		MONITORINFO sMInfo;

		p.x			= 0;
		p.y			= 0;
		hMonitor	= MonitorFromPoint(p, MONITOR_DEFAULTTOPRIMARY);
		sMInfo.cbSize = sizeof(sMInfo);
		bRet = GetMonitorInfo(hMonitor, &sMInfo);
		if (!bRet)
		{
			MessageBox(0, _T("Failed to get monitor info"), _T("Error"), MB_OK|MB_ICONEXCLAMATION);
			goto cleanup;
		}

		nWidth = sMInfo.rcMonitor.right - sMInfo.rcMonitor.left;
		nHeight = sMInfo.rcMonitor.bottom - sMInfo.rcMonitor.top;
	}
#endif
	// Create the eglWindow
	RECT	sRect;
	SetRect(&sRect, 0, 0, nWidth, nHeight);
	AdjustWindowRectEx(&sRect, WS_CAPTION | WS_SYSMENU, false, 0);
	hWnd = CreateWindow( WINDOW_TITLE_PREFIX, _T(WINDOW_TITLE), WS_VISIBLE | WS_SYSMENU,
						 0, 0, nWidth, nHeight, NULL, NULL, hInstance, NULL);
	eglWindow = hWnd;

	// Get the associated device context
	hDC = GetDC(hWnd);
	if (!hDC)
	{
		MessageBox(0, _T("Failed to create the device context"), _T("Error"), MB_OK|MB_ICONEXCLAMATION);
		destroyWindow();
	}
#endif
	/*
		Step 1 - Get the default display.
		EGL uses the concept of a "display" which in most environments
		corresponds to a single physical screen. Since we usually want
		to draw to the main screen or only have a single screen to begin
		with, we let EGL pick the default display.
		Querying other displays is platform specific.
	*/
	eglDisplay = eglGetDisplay(hDC);

    if(eglDisplay == EGL_NO_DISPLAY)
         eglDisplay = eglGetDisplay((EGLNativeDisplayType) EGL_DEFAULT_DISPLAY);
	/*
		Step 2 - Initialize EGL.
		EGL has to be initialized with the display obtained in the
		previous step. We cannot use other EGL functions except
		eglGetDisplay and eglGetError before eglInitialize has been
		called.
		If we're not interested in the EGL version number we can just
		pass NULL for the second and third parameters.
	*/
	EGLint iMajorVersion, iMinorVersion;
	if (!eglInitialize(eglDisplay, &iMajorVersion, &iMinorVersion))
	{
#ifndef NO_GDI
		MessageBox(0, _T("eglInitialize() failed."), _T("Error"), MB_OK|MB_ICONEXCLAMATION);
#endif
		destroyWindow();
	}

	/*
		Step 3 - Make OpenGL ES the current API.
		EGL provides ways to set up OpenGL ES and OpenVG contexts
		(and possibly other graphics APIs in the future), so we need
		to specify the "current API".
	*/
	eglBindAPI(EGL_OPENGL_ES_API);
	if (!TestEGLError(hWnd, "eglBindAPI"))
	{
		destroyWindow();
	}

	/*
		Step 4 - Specify the required configuration attributes.
		An EGL "configuration" describes the pixel format and type of
		surfaces that can be used for drawing.
		For now we just want to use the default Windows surface,
		i.e. it will be visible on screen. The list
		has to contain key/value pairs, terminated with EGL_NONE.
	 */
	const EGLint pi32ConfigAttribs[] =
	{
		EGL_LEVEL,				0,
		EGL_SURFACE_TYPE,		EGL_WINDOW_BIT,
		EGL_RENDERABLE_TYPE,	EGL_OPENGL_ES2_BIT,
		EGL_NATIVE_RENDERABLE,	EGL_FALSE,
		EGL_DEPTH_SIZE,			EGL_DONT_CARE,
		EGL_NONE
	};

	/*
		Step 5 - Find a config that matches all requirements.
		eglChooseConfig provides a list of all available configurations
		that meet or exceed the requirements given as the second
		argument. In most cases we just want the first config that meets
		all criteria, so we can limit the number of configs returned to 1.
	*/
	int iConfigs;
	if (!eglChooseConfig(eglDisplay, pi32ConfigAttribs, &eglConfig, 1, &iConfigs) || (iConfigs != 1))
	{
#ifndef NO_GDI
		MessageBox(0, _T("eglChooseConfig() failed."), _T("Error"), MB_OK|MB_ICONEXCLAMATION);
#endif
		destroyWindow();
	}

	/*
		Step 6 - Create a surface to draw to.
		Use the config picked in the previous step and the native window
		handle when available to create a window surface. A window surface
		is one that will be visible on screen inside the native display (or
		fullscreen if there is no windowing system).
		Pixmaps and pbuffers are surfaces which only exist in off-screen
		memory.
	*/
	eglSurface = eglCreateWindowSurface(eglDisplay, eglConfig, eglWindow, NULL);

    if(eglSurface == EGL_NO_SURFACE)
    {
        eglGetError(); // Clear error
        eglSurface = eglCreateWindowSurface(eglDisplay, eglConfig, NULL, NULL);
	}

	if (!TestEGLError(hWnd, "eglCreateWindowSurface"))
	{
		destroyWindow();
	}

	/*
		Step 7 - Create a context.
		EGL has to create a context for OpenGL ES. Our OpenGL ES resources
		like textures will only be valid inside this context
		(or shared contexts)
	*/
	EGLint ai32ContextAttribs[] = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE };
	eglContext = eglCreateContext(eglDisplay, eglConfig, NULL, ai32ContextAttribs);
	if (!TestEGLError(hWnd, "eglCreateContext"))
	{
		destroyWindow();
	}

	/*
		Step 8 - Bind the context to the current thread and use our
		window surface for drawing and reading.
		Contexts are bound to a thread. This means you don't have to
		worry about other threads and processes interfering with your
		OpenGL ES application.
		We need to specify a surface that will be the target of all
		subsequent drawing operations, and one that will be the source
		of read operations. They can be the same surface.
	*/
	eglMakeCurrent(eglDisplay, eglSurface, eglSurface, eglContext);
	if (!TestEGLError(hWnd, "eglMakeCurrent"))
	{
		destroyWindow();
	}


	
	/*std::cout<<"INFO: OpenGL Version:";
	const GLubyte* version = glGetString(GL_VERSION);
	if(version[0]=='1'){
	//	assert(false);//"Must have at least OpenGL 2.0");
	}
    printf("%s",version);*/

    glClearColor(.8f, .8f, .8f, 1.0f);

	loadTextures();
    initShaders();

	return true;
}

void OGLRenderContext::initShaders(){
	
	/* Lookup "OpenGLSL ES" for more info on what shaders are*/

	const char* VertexShader[4];
	const char* FragmentShader[4];

	VertexShader[0] =
	STRINGIFY(
	attribute highp vec2 inVertex;
	attribute mediump vec4 myUV;
	attribute mediump vec4 inColor;
	varying mediump vec4 outColor;
	uniform mediump mat4 myPMVMatrix;
	varying mediump vec2 texCoords;
	void main(void)
	{
		gl_Position = myPMVMatrix*vec4(inVertex,0.0,1.0);
		texCoords = myUV.st;
		outColor = inColor;
	});
	
	FragmentShader[0] = 
	STRINGIFY(
	precision mediump float;
	uniform sampler2D tex1;
	uniform sampler2D tex2;
	uniform sampler2D tex3;
	varying mediump vec4 outColor;
	varying mediump vec2 texCoords;
	void main(void)
	{
		bool isLit = outColor.w == 1.0;
		if(isLit){
			gl_FragColor = texture2D(tex1,texCoords)*outColor;
		}else{
			gl_FragColor = texture2D(tex2,texCoords); //Unlit
		}
	});

	VertexShader[1] =
	STRINGIFY(
	attribute highp vec2 inVertex;
	uniform mediump mat4 myPMVMatrix;
	void main(void)
	{
		gl_Position = myPMVMatrix*vec4(inVertex,0.0,1.0);
	});

	FragmentShader[1] = 
	STRINGIFY(
	void main(void)
	{
		gl_FragColor = vec4(0.0,0.0,0.0,1.0);
	});

	VertexShader[2] =
	STRINGIFY(
	attribute highp vec2 inVertex;
	uniform mediump mat4 myPMVMatrix;
	void main(void)
	{
		gl_Position = myPMVMatrix*vec4(inVertex,0.0,1.0);
	});

	FragmentShader[2] = 
	STRINGIFY(
	uniform mediump vec4 inColor;
	void main(void)
	{
		gl_FragColor = inColor;
	});

	VertexShader[3] =
	STRINGIFY(
	attribute highp vec2 inVertex;
	attribute mediump vec4 myUV;
	attribute mediump vec4 inColor;
	varying mediump vec4 outColor;
	uniform mediump mat4 myPMVMatrix;
	varying mediump vec2 texCoords;
	void main(void)
	{
		gl_Position = myPMVMatrix*vec4(inVertex,0.0,1.0);
		texCoords = myUV.st;
		//outColor = inColor;
	});

	FragmentShader[3] = 
	STRINGIFY(
	precision mediump float;
	uniform sampler2D tex1;
	uniform sampler2D tex2;
	varying mediump vec4 outColor;
	varying mediump vec2 texCoords;
	void main(void)
	{
		//bool isLit = outColor.w == 1.0;
		//if(isLit){
			gl_FragColor = texture2D(tex1,texCoords)*vec4(1.0,0.0,0.0,1.0);
		//}else{
		//	gl_FragColor = texture2D(tex2,texCoords); //Unlit
		//}
	});

	std::vector<std::pair<int,const char*> > attributes;
	attributes.push_back(std::make_pair((int)VERTEX_ARRAY,"inVertex"));
	attributes.push_back(std::make_pair((int)TEXCOORD_ARRAY,"myUV"));
	attributes.push_back(std::make_pair((int)COLOR_ARRAY,"inColor"));
	
	LEDDisplayShader = compileShader(VertexShader[0], FragmentShader[0],  attributes);

	attributes.resize(0);
	LEDBorderShader = compileShader(VertexShader[1], FragmentShader[1],  attributes);

	//attributes.push_back(std::make_pair((int)COLOR_ARRAY,"inColor"));
	pinNodeShader = compileShader(VertexShader[2], FragmentShader[2],  attributes);

	//std::vector<std::pair<int,const char*> > attributes;
	attributes.push_back(std::make_pair((int)VERTEX_ARRAY,"inVertex"));
	attributes.push_back(std::make_pair((int)TEXCOORD_ARRAY,"myUV"));
	attributes.push_back(std::make_pair((int)COLOR_ARRAY,"inColor"));

	propelShader = compileShader(VertexShader[3], FragmentShader[3],  attributes);

	LEDDisplayShader->uniLoc = new GLuint[4];
	LEDDisplayShader->uniLoc[0] = glGetUniformLocation(LEDDisplayShader->id,"myPMVMatrix");
	LEDDisplayShader->uniLoc[1] = glGetUniformLocation(LEDDisplayShader->id,"tex1");
	LEDDisplayShader->uniLoc[2] = glGetUniformLocation(LEDDisplayShader->id,"tex2");
	LEDDisplayShader->uniLoc[3] = glGetUniformLocation(LEDDisplayShader->id,"tex3");

	LEDBorderShader->uniLoc = new GLuint[1];
	LEDBorderShader->uniLoc[0] = glGetUniformLocation(LEDBorderShader->id,"myPMVMatrix");

	pinNodeShader->uniLoc = new GLuint[1];
	pinNodeShader->uniLoc[0] = glGetUniformLocation(pinNodeShader->id,"myPMVMatrix");
	pinNodeShader->uniLoc[1] = glGetUniformLocation(pinNodeShader->id,"inColor");

	propelShader->uniLoc = new GLuint[2];
	propelShader->uniLoc[0] = glGetUniformLocation(propelShader->id,"myPMVMatrix");
	propelShader->uniLoc[1] = glGetUniformLocation(propelShader->id,"tex1");
	propelShader->uniLoc[2] = glGetUniformLocation(propelShader->id,"tex2");

	glUseProgram(LEDDisplayShader->id);

}

shaderProgram* OGLRenderContext::compileShader(const char* pszVertShader, const char* pszFragShader, vector<pair<int,const char*> > attribs){
	
	shaderProgram* shader = new shaderProgram();

	shader->fragShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(shader->fragShader, 1, (const char**)&pszFragShader, NULL);

	// Compile the source code
	glCompileShader(shader->fragShader);

	// Check if compilation succeeded
	GLint bShaderCompiled;
	glGetShaderiv(shader->fragShader, GL_COMPILE_STATUS, &bShaderCompiled);
	if (!bShaderCompiled)
	{
		// An error happened, first retrieve the length of the log message
		int i32InfoLogLength, i32CharsWritten;
		glGetShaderiv(shader->fragShader, GL_INFO_LOG_LENGTH, &i32InfoLogLength);

		// Allocate enough space for the message and retrieve it
		char* pszInfoLog = new char[i32InfoLogLength];
		glGetShaderInfoLog(shader->fragShader, i32InfoLogLength, &i32CharsWritten, pszInfoLog);

		/*
		Displays the message in a dialog box when the application quits
		using the shell PVRShellSet function with first parameter prefExitMessage.
		*/
		char* pszMsg = new char[i32InfoLogLength+256];
		strcpy(pszMsg, "Failed to compile fragment shader: ");
		strcat(pszMsg, pszInfoLog);
		//PVRShellSet(prefExitMessage, pszMsg);
		assert(false);
		delete [] pszMsg;
		delete [] pszInfoLog;
		
	}

	// Loads the vertex shader in the same way
	shader->vertShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(shader->vertShader, 1, (const char**)&pszVertShader, NULL);
	glCompileShader(shader->vertShader);
	glGetShaderiv(shader->vertShader, GL_COMPILE_STATUS, &bShaderCompiled);
	if (!bShaderCompiled)
	{
		int i32InfoLogLength, i32CharsWritten;
		glGetShaderiv(shader->vertShader, GL_INFO_LOG_LENGTH, &i32InfoLogLength);
		char* pszInfoLog = new char[i32InfoLogLength];
		glGetShaderInfoLog(shader->vertShader, i32InfoLogLength, &i32CharsWritten, pszInfoLog);
		char* pszMsg = new char[i32InfoLogLength+256];
		strcpy(pszMsg, "Failed to compile vertex shader: ");
		strcat(pszMsg, pszInfoLog);
		//PVRShellSet(prefExitMessage, pszMsg);

		delete [] pszMsg;
		delete [] pszInfoLog;
		assert(false);
	}
	shader->id = glCreateProgram();

	// Attach the fragment and vertex shaders to it
	glAttachShader(shader->id, shader->fragShader);
	glAttachShader(shader->id, shader->vertShader);

	// Bind the custom attributes
	for(int i=0; i<attribs.size(); i++){
		std::pair<int,const char*> att = attribs[i];
		glBindAttribLocation(shader->id, att.first, att.second);
	}
	// Link the program
	glLinkProgram(shader->id);

	// Check if linking succeeded in the same way we checked for compilation success
	GLint bLinked;
	glGetProgramiv(shader->id, GL_LINK_STATUS, &bLinked);
	if (!bLinked)
	{
		int i32InfoLogLength, i32CharsWritten;
		glGetProgramiv(shader->id, GL_INFO_LOG_LENGTH, &i32InfoLogLength);
		char* pszInfoLog = new char[i32InfoLogLength];
		glGetProgramInfoLog(shader->id, i32InfoLogLength, &i32CharsWritten, pszInfoLog);
		char* pszMsg = new char[i32InfoLogLength+256];
		strcpy(pszMsg, "Failed to link program: ");
		strcat(pszMsg, pszInfoLog);
		//PVRShellSet(prefExitMessage, pszMsg);

		delete [] pszMsg;
		delete [] pszInfoLog;
		assert(false);
	}

	return shader;

}

void OGLRenderContext::destroyWindow(){
	eglMakeCurrent(eglDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
	eglTerminate(eglDisplay);

	/*
		Step 11 - Destroy the eglWindow.
		Again, this is platform specific and delegated to a separate function.
	*/
#ifndef NO_GDI
	// Release the device context
	if (hDC) ReleaseDC(hWnd, hDC);

	// Destroy the eglWindow
	if (hWnd) DestroyWindow(hWnd);
#endif
}

void OGLRenderContext::swapBuffers(){
	eglSwapBuffers(eglDisplay, eglSurface);
}

void OGLRenderContext::loadTextures(){
	TEX_LED_LIT = new GLuint;
	TEX_LED_BLANK = new GLuint;
	TEX_LED_GLOW = new GLuint;

	char cCurrentPath[FILENAME_MAX];
	if(!GetCurrentDir(cCurrentPath, sizeof(cCurrentPath))){
		MessageBox(0, _T("Couldn't find working directory"), _T("Error"), MB_OK | MB_ICONEXCLAMATION);
        destroyWindow();
	}
	cCurrentPath[sizeof(cCurrentPath) - 1] = '/0'; /* not really required */
	string filename("\\Resource\\led_blank.pvr");
	string dir(cCurrentPath);
	filename = dir + filename;
	//MessageBox(0, _T(cCurrentPath), _T("Error"), MB_OK | MB_ICONEXCLAMATION);
	if (PVRTTextureLoadFromPVR(filename.c_str(), TEX_LED_BLANK)!=PVR_SUCCESS)
	{
		string msg = string("ERROR: Could not load texture: ") + string(filename);
		MessageBox(0, _T(msg.c_str()), _T("Error"), MB_OK | MB_ICONEXCLAMATION);
        destroyWindow();
	}

	filename = dir + string("\\Resource\\led.pvr");
	if (PVRTTextureLoadFromPVR(filename.c_str(), TEX_LED_LIT)!=PVR_SUCCESS)
	{
		string msg = string("ERROR: Could not load texture: ") + string(filename);
		MessageBox(0, _T(msg.c_str()), _T("Error"), MB_OK | MB_ICONEXCLAMATION);
        destroyWindow();
	}

	filename = dir + string("\\Resource\\led_glow.pvr");
	if (PVRTTextureLoadFromPVR(filename.c_str(), TEX_LED_GLOW)!=PVR_SUCCESS)
	{
		string msg = string("ERROR: Could not load texture: ") + string(filename);
		MessageBox(0, _T(msg.c_str()), _T("Error"), MB_OK | MB_ICONEXCLAMATION);
        destroyWindow();
	}

}