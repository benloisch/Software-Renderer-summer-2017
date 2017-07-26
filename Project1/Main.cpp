#include <Windows.h>
#include <sstream>
#include <math.h>
#include <iostream>
#include <vector>

#include <thread>

using namespace std;

#include "High_Performance_Counter.h"
#include "Win32WindowBuffer.h"
//#include "BufferInterface.h"
#include "Matrix4x4.h"
#include "Vector4D.h"
#include "Vertex.h"
#include "Camera.h"
#include "Mesh.h"
#include "Pipeline.h"

void displayFPS(HighPerformanceCounter timer, HWND windowHandle)
{
	static int frames;
	frames++;

	static double second;
	second += timer.mtimePerFrame;

	if (second >= 1.0)
	{
		std::wostringstream outs;
		outs.precision(6);
		outs << "FPS: " << frames << " MSPF: " << 1000 * timer.mtimePerFrame;
		SetWindowText(windowHandle, outs.str().c_str());

		frames = 0;
		second = 0;
	}
}

void displayText(HDC *hdc, LPCWSTR lpcwstr, int integer, int x, int y) {
	wchar_t text[100];
	for (int i = 0; i < 100; i++)
		text[i] = L'\0';
	int size = 0;
	for (; lpcwstr[size] != '\0'; size++)
		text[size] = lpcwstr[size];

	wchar_t intbuffer[20];
	wsprintfW(intbuffer, L"%d", integer);

	for (int i = 0; i < 20; i++) {
		text[size] = intbuffer[i];
		size++;

		if (intbuffer[i] == '\0')
			break;
	}

	TextOut(*hdc, x, y, text, size);
}

//********************************************************multithreading attempt
/*void testFunc(Win32WindowBuffer *win32buf) {
	win32buf->FillBufferColor(255, 0, 255);
}*/

//call main so we can create a console for output
int main() {
	return WinMain(GetModuleHandle(NULL), NULL, NULL, SW_SHOW);
}

void setScaleTranslate(Mesh &mesh, float tx, float ty, float tz, float sx, float sy, float sz) {
	Matrix4x4 scale;
	scale.setScale(sx, sy, sz);
	Matrix4x4 translate;
	translate.setTranslate(tx, ty, tz);
	mesh.modelMesh = scale * translate;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pCmdLine, int nShowCmd)
{
	Win32WindowBuffer win32WindowBuffer(1920, 1080); //960 x 540
	if (!win32WindowBuffer.initializeWindow(hInstance, nShowCmd))
		return -1;

	MSG msg = { 0 };

	HighPerformanceCounter hpc;
	hpc.Start();

	//*******************************************Setup Level
	//load models and their textures
	//apply matrix transforms

	//*******************************************Setup camera
	//define camera position / rotation
	Camera cam;
	cam.setOriginPosition(0, 0, 0);
	//cam.setLookDirection(0.01, 0.7505, 1);
	cam.setLookDirection(0, 0, 1);
	cam.calculateViewMatrix();

	//define projection matrix
	cam.setNearPlane(0.1f);
	cam.setFarPlane(1000);
	cam.setAspectRatio(win32WindowBuffer.clientWidth / 2, win32WindowBuffer.clientHeight / 2);
	cam.setFieldOfView(70);
	cam.calculateProjectionMatrix();

	//*******************************************Setup input
	ShowCursor(false);
	SetCursorPos(win32WindowBuffer.clientWidth / 2, win32WindowBuffer.clientHeight / 2);
	GetCursorPos(&cam.sMouse);

	//*******************************************Setup pipeline
	Pipeline pipeline;
	pipeline.msg = &msg;
	pipeline.setCamera(&cam);
	pipeline.setScreenBuffer(win32WindowBuffer.bytebuffer);

	//*******************************************Set directional light
	pipeline.directionalLight = Vector4D(1.0f, 0.0f, 0.0f, 0.0f); //light shining down and to the right
	
	Mesh moskvitch;
	moskvitch.zCull = false;
	moskvitch.loadTexture("Textures/moskvitch");
	moskvitch.loadModel("Models/moskvitch");

	Mesh Conditioner;
	Conditioner.loadTexture("Textures/Conditioner");
	Conditioner.loadModel("Models/Conditioner");

	Mesh Tavern;
	Tavern.zCull = false;
	Tavern.loadTexture("Textures/Tavern");
	Tavern.loadModel("Models/Tavern");

	Mesh Raptor;
	Raptor.loadTexture("Textures/Raptor");
	Raptor.loadModel("Models/Raptor");

	Mesh ZombiDog;
	ZombiDog.loadTexture("Textures/ZombiDog");
	ZombiDog.loadModel("Models/ZombiDog");

	Mesh farmhouse;
	farmhouse.zCull = false;
	farmhouse.loadTexture("Textures/farmhouse");
	farmhouse.loadModel("Models/farmhouse");

	Mesh Fountain;
	Fountain.loadTexture("Textures/Fountain");
	Fountain.loadModel("Models/Fountain");

	Mesh barrel;
	barrel.loadTexture("Textures/barrel");
	barrel.loadModel("Models/barrel");

	Mesh skybox;
	skybox.zCull = false;
	skybox.loadTexture("Textures/skybox");
	skybox.loadModel("Models/skybox");

	Mesh chair;
	chair.loadTexture("Textures/chair");
	chair.loadModel("Models/chair");

	Mesh bench;
	bench.loadTexture("Textures/bench");
	bench.loadModel("Models/bench");
	
	Mesh terrain;
	terrain.zCull = false;
	terrain.loadTexture("Textures/terrain");
	terrain.loadModel("Models/terrain");

	Mesh bridge;
	bridge.zCull = false;
	bridge.loadTexture("Textures/Bridge");
	bridge.loadModel("Models/Bridge");
	
	Mesh canyon;
	//canyon.zCull = false;
	canyon.loadTexture("Textures/canyon");
	canyon.loadModel("Models/canyon");


	Mesh triangle;
	triangle.loadTexture("Fountain");
	triangle.loadModel("Models/triangle");

	
	setScaleTranslate(moskvitch, 0, 0, 0, 1, 1, 1);
	setScaleTranslate(Conditioner, 3, 1, 0, 0.005f, 0.005f, 0.005f);
	setScaleTranslate(Tavern, 10, 0, 0, 1, 1, 1);
	setScaleTranslate(Raptor, 17, 0, 0, 0.01f, 0.01f, 0.01f);
	setScaleTranslate(ZombiDog, 20, 0, 0, 1, 1, 1);
	setScaleTranslate(farmhouse, 30, 0, 0, 0.4f, 0.4f, 0.4f);
	setScaleTranslate(Fountain, 39, 0, 0, 0.01f, 0.01f, 0.01f);
	setScaleTranslate(barrel, -3, 0, 0, 0.01f, 0.01f, 0.01f);
	setScaleTranslate(skybox, 0, 150, 0, 0.05f, 0.05f, 0.05f);
	setScaleTranslate(chair, -10, 0, 0, 1, 1, 1);
	setScaleTranslate(bench, -13, 0, 0, 0.1f, 0.1f, 0.1f);
	setScaleTranslate(terrain, 0, 0, 0, 0.00001f, 0.00001f, 0.00001f);
	setScaleTranslate(bridge, -10, 0, -10, 0.2f, 0.2f, 0.2f);
	setScaleTranslate(canyon, 50, 0, 30, 1, 1, 1);
	
	setScaleTranslate(triangle, 0, -0.5, 2, 1, 1, 1);

	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		//*********************************************************Clear back/depth buffer for drawing
		win32WindowBuffer.FillBufferBlack();
		pipeline.clearDepthBuffer();

		//*********************************************************Get delta (time spent rendering a single frame in miliseconds)
		float delta = (float)hpc.mtimePerFrame;
		
		//*********************************************************Get Mouse Input and move Camera
		cam.getInput(delta); //delta used to smooth mouse relative to how fast framerate is
		cam.calculateViewMatrix(); //after rotating lookDirection vector, recalculate camera matrix
		//cam.viewMatrix.m[3][1] = 2;

		//**********************************************************Setup mesh matricies
		static float rot = 0;
		rot += delta;

		//**********************************************************Render mesh objects
		
		pipeline.transform(&moskvitch);
		pipeline.transform(&Conditioner);
		pipeline.transform(&Tavern);
		pipeline.transform(&Raptor);
		pipeline.transform(&ZombiDog);
		pipeline.transform(&farmhouse);
		pipeline.transform(&Fountain);
		pipeline.transform(&barrel);
		pipeline.transform(&skybox);
		pipeline.transform(&chair);
		pipeline.transform(&bench);
		pipeline.transform(&bridge);
		pipeline.transform(&terrain);
		pipeline.transform(&canyon);
		
		pipeline.transform(&triangle);

		//***********************************************************Flip buffer
		win32WindowBuffer.drawBuffer();

		//***********************************************************Render debug text
		displayText(&win32WindowBuffer.hdc, L"Triangles Loaded: ", trianglesLoaded, 0, 0);
		displayText(&win32WindowBuffer.hdc, L"Triangles Rendered: ", pipeline.trianglesRendered, 0, 20);
		pipeline.trianglesRendered = 0;

		hpc.Tick();
		displayFPS(hpc, *win32WindowBuffer.getWindowHandle());
		
	}

	return (int)msg.wParam;
}