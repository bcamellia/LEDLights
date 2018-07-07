#include "Renderer.h"
#include "LEDMatrixMainDisplay.h"
#include "Input.h"
#include "Player.h"
#include "Wall.h"
#include "PushableBlock.h"
#include "HorizPlatform.h"
#include "PropellorClock.h"

using namespace std;

OGLRenderContext* OGLRenderer;
vector<LEDMatrixNode*> displays;
InputManager* input;
Player* player;
PushableBlock* p;
HorizPlatform* plat;
vector<LEDObject*> walls;
PhysicsWorld* world;
PropellorClock* clock;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, TCHAR *lpCmdLine, int nCmdShow){
	
	OGLRenderer = new OGLRenderContext;
	OGLRenderer->init(hInstance, hPrevInstance, lpCmdLine, nCmdShow); //Initialize window.

	input = new InputManager(); //Handles Mouse and key input
	world = new PhysicsWorld(); //Used to check if user is clicking a pin
	world->initPhysics();
	input->OGL = &OGLRenderer;
	OGLRenderer->addInputManager(input);

	displays.push_back(new LEDMatrixNode(OGLRenderer,input,20,20,-.5,-.5,world)); //Create 2 LED Display Panels
	displays.push_back(new LEDMatrixNode(OGLRenderer,input,20,10,.5,.3,world));
	
	player = new Player(displays[0],5,5,10,10,C_BLUE,input); //Player
	
	/*walls.push_back(new Wall(displays[0],displays[0]->width,1,0,0,C_WHITE));
	walls.push_back(new Wall(displays[0],1,displays[0]->height,0,0,C_WHITE)); //3 Walls
	walls.push_back(new Wall(displays[1],displays[1]->width,1,0,0,C_WHITE));*/
	clock = new PropellorClock(OGLRenderer,0,-.4,.5,.5);

	p = new PushableBlock(displays[0],4,4,10,3,C_RED,input); //Push Block
	plat = new HorizPlatform(displays[0],5,1,5,5,C_GREEN,1,.04,100,input);

for(int i=0; i<50000; i++){
		glClear(GL_COLOR_BUFFER_BIT); //Clear screen
		
		LEDMatrixNode* otherSelDisplay = 0;

		//Clear LED's and check for input
		for(int j=0; j<displays.size(); j++){
			displays[j]->rePaintScreen();
			vector<pinNode*>* selPtr = displays[j]->getArrSelectedPins();
			if(selPtr){
				otherSelDisplay = displays[j];
			}
		}
		pinNode* n = 0;
		if(input->isMouseDown){
			n = static_cast<pinNode*>(world->rayTest(input->mouse_x,input->mouse_y));
		}
		for(int j=0; j<displays.size(); j++){
			displays[j]->checkSelectedPins(n,otherSelDisplay);
		}

		//Update objects, each object sends a matrix of LED's based on position to its LED Display.
		/*for(int j=0; j<walls.size(); j++){
			walls[j]->render();
		}*/
		
		//p->render();
		//plat->render();
		player->render();
		//p->step();
		player->step();
		//plat->step();
		
		//Render Lit LED's
		for(int j=0; j<displays.size(); j++){
			displays[j]->renderLEDS(M_PROJ);
		}
		//clock->render();
		OGLRenderer->swapBuffers(); //Everything rendered, now make it visible. It renders 50,000 times or until you quit.
	}
}