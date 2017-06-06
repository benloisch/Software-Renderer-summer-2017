#include "SDL.h"
#include "Camera.h"
#include "Vertex.h"
#include "WorldViewProjection.h"
#include "PerspectiveProjection.h"
#include "ProcessVertex.h"
#include "Object.h"
#include "Triangle.h"
#include "LazyFooTimer.h"
#include <iostream>
#include <sstream>
#include <string>

const int screenWidth = 1366;
const int screenHeight = 768;
const double nearPlane = 1;
const double farPlane = 5000;
const double fieldOfView = 65;
const double halfScreenWidth = screenWidth / 2;
const double halfScreenHeight = screenHeight / 2;
const int framesPerSecond = 500;

int main(int argc, char* args[])
{
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_ShowCursor(0);
	SDL_Surface* screen;
	screen = SDL_SetVideoMode(screenWidth, screenHeight, 32, SDL_FULLSCREEN);
	bool running = true;
	SDL_Event event;

	//Next three lines lazyFoo Code
	Timer fps;
	Timer update;
	int frame = 0;
	int blur = 0;
	long int startTicks = 0; 

	double cameraXCoord = 0;
	double cameraYCoord = 0;
	double cameraZCoord = 0;

	double cameraXAngleUPDOWN = 0.5;
	double cameraYAngleLEFTRIGHT = 0.5;

	double mouseX = 0;
	double mouseY = 0;

	bool testMouse = true;
	bool testKeys = true;

	bool renderSecondCam = false;

	Uint8 *keystates = SDL_GetKeyState(NULL);

	Camera cam(0, 0, 0, 1, 0, 0);
	cam.CreateCamMatrix();
	cam.calculateCameraDeterminant();
	cam.calculateAdjointMatrix();
	cam.calculateInverseCameraMatrix();

	PerspectiveProj perspProj(nearPlane, farPlane, screenWidth, screenHeight, fieldOfView);
	perspProj.createPerspectiveProjMatrix();

	WVP worldViewProj;

	Object cubeFloor;
	
	for (double x = 0; x < 1000; x += 10){
		for (double z = 0; z < 1000; z += 10){
			cubeFloor.addVertex(x, -50, z, 1);
		}
	}
	
	Object cubeTest;

	int y = 0;
	for (int z = 0; z < 50; z += 3){
		y = 0;
		for (int y = 0; y < 30; y += 3){
			cubeTest.addVertex(100, 1.5 + y, -1 + z, 1);
			cubeTest.addVertex(100, 3 + y, -2.5 + z, 1);
			cubeTest.addVertex(100, 1 + y, -3 + z, 1);
		}
	}
	cubeTest.setColor(100, 150, 50);

	Object cubeTwo;

	cubeTwo.addVertex(15, 3, -1, 1);
	cubeTwo.addVertex(15, 9, -3, 1);
	cubeTwo.addVertex(15, 1, -9, 1);
	cubeTwo.setColor(100, 0, 255);

	SDL_WarpMouse(halfScreenWidth, halfScreenHeight);

	//LazyFoo Code next two lines
	fps.start();
	update.start();

	while (running){
		startTicks = SDL_GetTicks();

		SDL_WarpMouse(halfScreenWidth, halfScreenHeight);

		while (SDL_PollEvent(&event)){

			if (event.type == SDL_QUIT){
				running = false;
				break;
			}

			if (keystates[SDLK_ESCAPE]){
				running = false;
				break;
			}
		}

		testKeys = true;

		if (event.type == SDL_MOUSEMOTION){
			if (keystates[SDLK_RIGHT]){
				cam.moveCameraRight();
				testKeys = false;
			}

			if (keystates[SDLK_LEFT]){
				cam.moveCameraLeft();
				testKeys = false;
			}

			if (keystates[SDLK_UP]){
				cam.moveCameraForward();
				testKeys = false;
			}

			if (keystates[SDLK_DOWN]){
				cam.moveCameraBack();
				testKeys = false;
			}

			mouseX = event.motion.x;
			mouseY = event.motion.y;

			mouseX = mouseX - halfScreenWidth;
			mouseY = mouseY - halfScreenHeight;

			mouseX /= 5.0;
			mouseY /= 5.0;

			cam.rotateXAxis(mouseY);
			cam.rotateYAxis(mouseX);
			cam.multiplyRotationMatrix();
			cam.CreateCamMatrix();
			cam.calculateCameraDeterminant();
			cam.calculateAdjointMatrix();
			cam.calculateInverseCameraMatrix();
		}

		if (testKeys){
			if (keystates[SDLK_RIGHT]){
				cam.moveCameraRight();
				cam.CreateCamMatrix();
				cam.calculateCameraDeterminant();
				cam.calculateAdjointMatrix();
				cam.calculateInverseCameraMatrix();
			}

			if (keystates[SDLK_LEFT]){
				cam.moveCameraLeft();
				cam.CreateCamMatrix();
				cam.calculateCameraDeterminant();
				cam.calculateAdjointMatrix();
				cam.calculateInverseCameraMatrix();
			}

			if (keystates[SDLK_UP]){
				cam.moveCameraForward();
				cam.CreateCamMatrix();
				cam.calculateCameraDeterminant();
				cam.calculateAdjointMatrix();
				cam.calculateInverseCameraMatrix();
			}

			if (keystates[SDLK_DOWN]){
				cam.moveCameraBack();
				cam.CreateCamMatrix();
				cam.calculateCameraDeterminant();
				cam.calculateAdjointMatrix();
				cam.calculateInverseCameraMatrix();
			}
		}

		worldViewProj.multiplyWorldViewProjection(cubeFloor, cam, perspProj);
		worldViewProj.multiplyVertexByWVP(cubeFloor);
		processVerticiesf(cubeFloor, halfScreenWidth, halfScreenHeight, screen, false);
		
		worldViewProj.multiplyWorldViewProjection(cubeTest, cam, perspProj);
		worldViewProj.multiplyVertexByWVP(cubeTest);
		processVerticiesf(cubeTest, halfScreenWidth, halfScreenHeight, screen, true);

		worldViewProj.multiplyWorldViewProjection(cubeTwo, cam, perspProj);
		worldViewProj.multiplyVertexByWVP(cubeTwo);
		processVerticiesf(cubeTwo, halfScreenWidth, halfScreenHeight, screen, true);

		if (blur == 10){
			SDL_Flip(screen);
			blur = 0;
			SDL_FillRect(screen, &screen->clip_rect, SDL_MapRGB(screen->format, 20, 20, 20));
		}

		frame++;
		blur++;

		if (update.get_ticks() > 1000)
		{
			//cout << "FPS: " << frame / (fps.get_ticks() / 1000.f) << endl;
			update.start();
		}
		if ((SDL_GetTicks() - startTicks) < 1000 / framesPerSecond)
			SDL_Delay((1000 / framesPerSecond) - (SDL_GetTicks() - startTicks));
	}
	return 0;
}
