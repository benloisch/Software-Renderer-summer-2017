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

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pCmdLine, int nShowCmd)
{
	Win32WindowBuffer win32WindowBuffer(1920, 1080);
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
	cam.setOriginPosition(-20, 0, -20);
	//cam.setLookDirection(0.01, 0.7505, 1);
	cam.setLookDirection(1, 0, 1);
	cam.calculateViewMatrix();

	//define projection matrix
	cam.setNearPlane(0.1f);
	cam.setFarPlane(1000);
	cam.setAspectRatio(win32WindowBuffer.clientWidth, win32WindowBuffer.clientHeight);
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

	Mesh mesh;
	Mesh mesh2;
	Mesh mesh3;
	Mesh mesh4;
	Mesh mesh5;
	Mesh mesh6;
	Mesh triangleMesh;

	mesh.zCull = false;
	mesh.loadTexture("Textures/moskvitch");
	mesh.loadModel("Models/moskvitch");

	mesh2.loadTexture("Textures/Conditioner");
	mesh2.loadModel("Models/Conditioner");

	mesh3.zCull = false;
	mesh3.loadTexture("Textures/Tavern");
	mesh3.loadModel("Models/Tavern");

	mesh4.loadTexture("Textures/Raptor");
	mesh4.loadModel("Models/Raptor");

	mesh5.loadTexture("Textures/ZombiDog");
	mesh5.loadModel("Models/ZombiDog");

	mesh6.zCull = false;
	mesh6.loadTexture("Textures/farmhouse");
	mesh6.loadModel("Models/farmhouse");

	triangleMesh.loadTexture("grid");
	triangleMesh.loadModel("Models/triangle");

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


		//**********************************************************Setup mesh matricies
		static float rot = 0;
		rot += delta;

		Matrix4x4 rotate;
		rotate.setYrot(rot);
		Matrix4x4 scale;
		scale.setScale(1, 1, 1);
		Matrix4x4 translate;
		translate.setTranslate(0, 0, 2);
		mesh.modelMesh = rotate * scale * translate;

		rotate.setIdentity();
		rotate.setYrot(0);
		scale.setIdentity();
		scale.setScale(0.005f, 0.005f, 0.005f);
		translate.setIdentity();
		translate.setTranslate(0, 0, 0.5);
		mesh2.modelMesh = rotate * scale * translate;

		rotate.setIdentity();
		rotate.setYrot(rot);
		scale.setIdentity();
		scale.setScale(1, 1, 1);
		translate.setIdentity();
		translate.setTranslate(-10, 0, 0);
		mesh3.modelMesh = rotate * scale * translate;

		rotate.setIdentity();
		rotate.setYrot(rot);
		scale.setIdentity();
		scale.setScale(0.01f, 0.01f, 0.01f);
		translate.setIdentity();
		translate.setTranslate(3, 0, 2);
		mesh4.modelMesh = rotate * scale * translate;

		rotate.setIdentity();
		rotate.setYrot(rot);
		scale.setIdentity();
		scale.setScale(1, 1, 1);
		translate.setIdentity();
		translate.setTranslate(3, 0, 0);
		mesh5.modelMesh = rotate * scale * translate;

		rotate.setIdentity();
		rotate.setYrot(0);
		scale.setIdentity();
		scale.setScale(1, 1, 1);
		translate.setIdentity();
		translate.setTranslate(0, 0, 1);
		triangleMesh.modelMesh = rotate * scale * translate;

		rotate.setIdentity();
		rotate.setYrot(rot);
		scale.setIdentity();
		scale.setScale(0.2f, 0.2f, 0.2f);
		translate.setIdentity();
		translate.setTranslate(0, 0, 15);
		mesh6.modelMesh = rotate * scale * translate;
		//**********************************************************Render mesh objects
		
		pipeline.transform(&mesh);
		int x = 0;
		for (int i = 0; i < 20; i++) {
			rotate.setYrot(rot);
			scale.setScale(1, 1, 1);
			translate.setTranslate((float)x, 0, 6);
			mesh.modelMesh = rotate * scale * translate;
			pipeline.transform(&mesh);

			//thread t(&Pipeline::transform, &pipeline, &mesh);
			//t.join();

			x += 4;
		}
		
		
		pipeline.transform(&mesh2);
		pipeline.transform(&mesh3);
		pipeline.transform(&mesh4);
		pipeline.transform(&mesh5);
		pipeline.transform(&mesh6);
		pipeline.transform(&triangleMesh);
		

		//thread t(&Pipeline::transform, &pipeline, &triangleMesh);
		//t.join();

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