#include <Windows.h>
#include <sstream>
#include <math.h>
#include <iostream>
#include <vector>
#include <wtypes.h>
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

void setScaleRotTranslate(Mesh &mesh, float ry, float tx, float ty, float tz, float sx, float sy, float sz) {
	Matrix4x4 scale;
	scale.setScale(sx, sy, sz);
	Matrix4x4 translate;
	translate.setTranslate(tx, ty, tz);
	Matrix4x4 rotation;
	rotation.setYrot(ry);
	mesh.modelMesh = rotation * scale * translate;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pCmdLine, int nShowCmd)
{
	RECT desktop;
	const HWND hDesktop = GetDesktopWindow();
	GetWindowRect(hDesktop, &desktop);

	Win32WindowBuffer win32WindowBuffer(desktop.right, desktop.bottom); //960 x 540
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
	//cam.setOriginPosition(40, 15, -40);
	cam.setOriginPosition(30, 3, -20);
	//cam.setLookDirection(0.01, 0.7505, 1);
	cam.setLookDirection(0, 0, 1);
	cam.calculateViewMatrix();

	//define projection matrix
	cam.setNearPlane(0.1f);
	cam.setFarPlane(1000);
	cam.setAspectRatio(win32WindowBuffer.clientWidth, win32WindowBuffer.clientHeight);
	cam.setFieldOfView(60);
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
	//pipeline.directionalLight = Vector4D(1.0f, 0.0f, 0.0f, 0.0f); //light shining down and to the right
	
	//*******************************************Load meshes
	displayText(&win32WindowBuffer.hdc, L"Loading: %", 5, win32WindowBuffer.width / 2, win32WindowBuffer.height / 2);
	Mesh Raptor;
	Raptor.loadTexture("Textures/Raptor");
	Raptor.loadModel("Models/Raptor");

	displayText(&win32WindowBuffer.hdc, L"Loading: %", 10, win32WindowBuffer.width / 2, win32WindowBuffer.height / 2);
	Mesh Conditioner;
	Conditioner.loadTexture("Textures/Conditioner");
	Conditioner.loadModel("Models/Conditioner");

	displayText(&win32WindowBuffer.hdc, L"Loading: %", 12, win32WindowBuffer.width / 2, win32WindowBuffer.height / 2);
	Mesh ZombiDog;
	ZombiDog.loadTexture("Textures/ZombiDog");
	ZombiDog.loadModel("Models/ZombiDog");

	displayText(&win32WindowBuffer.hdc, L"Loading: %", 25, win32WindowBuffer.width / 2, win32WindowBuffer.height / 2);
	Mesh moskvitch;
	moskvitch.zCull = false;
	moskvitch.loadTexture("Textures/moskvitch");
	moskvitch.loadModel("Models/moskvitch");

	displayText(&win32WindowBuffer.hdc, L"Loading: %", 30, win32WindowBuffer.width / 2, win32WindowBuffer.height / 2);
	Mesh barrel;
	barrel.loadTexture("Textures/barrel");
	barrel.loadModel("Models/barrel");

	displayText(&win32WindowBuffer.hdc, L"Loading: %", 35, win32WindowBuffer.width / 2, win32WindowBuffer.height / 2);
	Mesh chair;
	chair.loadTexture("Textures/chair");
	chair.loadModel("Models/chair");

	displayText(&win32WindowBuffer.hdc, L"Loading: %", 40, win32WindowBuffer.width / 2, win32WindowBuffer.height / 2);
	Mesh bench;
	bench.loadTexture("Textures/bench");
	bench.loadModel("Models/bench");

	displayText(&win32WindowBuffer.hdc, L"Loading: %", 50, win32WindowBuffer.width / 2, win32WindowBuffer.height / 2);
	Mesh farmhouse;
	farmhouse.zCull = false;
	farmhouse.loadTexture("Textures/farmhouse");
	farmhouse.loadModel("Models/farmhouse");

	displayText(&win32WindowBuffer.hdc, L"Loading: %", 55, win32WindowBuffer.width / 2, win32WindowBuffer.height / 2);
	Mesh Fountain;
	Fountain.loadTexture("Textures/Fountain");
	Fountain.loadModel("Models/Fountain");

	displayText(&win32WindowBuffer.hdc, L"Loading: %", 65, win32WindowBuffer.width / 2, win32WindowBuffer.height / 2);
	Mesh Tavern;
	Tavern.zCull = false;
	Tavern.loadTexture("Textures/Tavern");
	Tavern.loadModel("Models/Tavern");

	displayText(&win32WindowBuffer.hdc, L"Loading: %", 75, win32WindowBuffer.width / 2, win32WindowBuffer.height / 2);
	Mesh skybox;
	skybox.zCull = false;
	skybox.loadTexture("Textures/skybox");
	skybox.loadModel("Models/skybox");

	displayText(&win32WindowBuffer.hdc, L"Loading: %", 90, win32WindowBuffer.width / 2, win32WindowBuffer.height / 2);
	Mesh terrain;
	terrain.zCull = false;
	terrain.loadTexture("Textures/terrain");
	terrain.loadModel("Models/terrain");

	displayText(&win32WindowBuffer.hdc, L"Loading: %", 95, win32WindowBuffer.width / 2, win32WindowBuffer.height / 2);
	Mesh bridge;
	bridge.zCull = false;
	bridge.loadTexture("Textures/Bridge");
	bridge.loadModel("Models/Bridge");
	
	Mesh canyon;
	canyon.loadTexture("Textures/canyon");
	canyon.loadModel("Models/canyon");
	
	//*******************************************************************Set initial position of meshes
	setScaleRotTranslate(Raptor, -90, 40, 0, 85, 0.2f, 0.2f, 0.2f);
	setScaleRotTranslate(Conditioner, -35, 18, 5, -26, 0.005f, 0.005f, 0.005f);
	setScaleRotTranslate(ZombiDog, 30, 28, 0, -25, 1.2f, 1.2f, 1.2f);
	setScaleRotTranslate(moskvitch, 0, 20, 0, -14, 2, 2, 2);
	setScaleRotTranslate(barrel, 50, 33, 0, -14, 0.02f, 0.02f, 0.02f);
	setScaleRotTranslate(chair, 50, 36.5, 1, -10.5, 1, 1, 1);
	setScaleRotTranslate(bench, -45, 38, 0, -24, 0.1f, 0.1f, 0.1f);
	setScaleRotTranslate(farmhouse, 50, 40, 0, -5, 0.4f, 0.4f, 0.4f);
	setScaleRotTranslate(Fountain, 0, 35, 0, -20, 0.01f, 0.01f, 0.01f);
	setScaleRotTranslate(Tavern, -35, 20, 0, -30, 1, 1, 1);
	setScaleRotTranslate(skybox, 0, 0, 150, 0, 0.05f, 0.05f, 0.05f);
	setScaleRotTranslate(terrain, 0, 0, 0, 0, 0.00001f, 0.00001f, 0.00001f);
	setScaleRotTranslate(bridge, 0, 34, 0, 85, 0.9f, 0.9f, 0.9f);
	
	setScaleRotTranslate(canyon, 0, 40, 0, -15, 7, 7, 7);
	
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

		static float rot = 0;
		rot += delta;
		
		cam.getInput(delta); //delta used to smooth mouse relative to how fast framerate is
		//Matrix4x4 rotation;
		//rotation.setYrot(rot * 0.05);
		//cam.lookDirection *= rotation;
		cam.calculateViewMatrix(); //after rotating lookDirection vector, recalculate camera matrix
		//cam.viewMatrix.m[3][1] = 2;

		//**********************************************************Render mesh objects
		
		pipeline.transform(&Conditioner);
		setScaleRotTranslate(Raptor, -90, 40, 0, 85, 0.2f, 0.2f, 0.2f);
		pipeline.transform(&Raptor);
		setScaleRotTranslate(Raptor, -90, 35, 0, 75, 0.3f, 0.3f, 0.3f);
		pipeline.transform(&Raptor);
		setScaleRotTranslate(Raptor, -90, 42, 0, 70, 0.1f, 0.1f, 0.1f);
		pipeline.transform(&Raptor);
		pipeline.transform(&ZombiDog);
		pipeline.transform(&moskvitch);
		setScaleRotTranslate(barrel, 50, 33, 0, -14, 0.02f, 0.02f, 0.02f);
		pipeline.transform(&barrel);
		setScaleRotTranslate(barrel, 50, 31, 0, -15, 0.02f, 0.02f, 0.02f);
		pipeline.transform(&barrel);
		pipeline.transform(&chair);
		pipeline.transform(&bench);
		pipeline.transform(&farmhouse);
		pipeline.transform(&Fountain);
		pipeline.transform(&Tavern);
		pipeline.transform(&bridge);
		pipeline.transform(&canyon);
		pipeline.transform(&terrain);
		pipeline.transform(&skybox);

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