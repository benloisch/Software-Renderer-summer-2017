#include <Windows.h>
#include <sstream>
#include <math.h>
#include <iostream>
#include <vector>

//#include <thread>

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
	cam.setOriginPosition(0, 0, 0);
	//cam.setLookDirection(0.01, 0.7505, 1);
	cam.setLookDirection(0.01f, 0.7505f, 1);
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

	/*
	Mesh mesh;
	
	//top left triangle
	mesh.verticies.push_back(Vertex(Vector4D(-1.0f, -1.0f, 0.0f, 1.0f), Vector4D(0.0f, 1.0f, 0.0f, 0.0f), Vector4D(0.0f, 0.0f, 0.0f, 0.0f)));//bottom left
	mesh.verticies.push_back(Vertex(Vector4D(-1.0f, 1.0f, 0.0f, 1.0f), Vector4D(0.0f, 0.0f, 0.0f, 0.0f), Vector4D(0.0f, 0.0f, 0.0f, 0.0f)));//upper left
	mesh.verticies.push_back(Vertex(Vector4D(1.0f, 1.0f, 0.0f, 1.0f), Vector4D(1.0f, 0.0f, 0.0f, 0.0f), Vector4D(0.0f, 0.0f, 0.0f, 0.0f)));//upper right
	
	//bottom right triangle
	mesh.verticies.push_back(Vertex(Vector4D(1.0f, -1.0f, 0.0f, 1.0f), Vector4D(1.0f, 1.0f, 0.0f, 0.0f), Vector4D(0.0f, 0.0f, 0.0f, 0.0f)));//bottom right
	mesh.verticies.push_back(Vertex(Vector4D(-1.0f, -1.0f, 0.0f, 1.0f), Vector4D(0.0f, 1.0f, 0.0f, 0.0f), Vector4D(0.0f, 0.0f, 0.0f, 0.0f)));//bottom left
	mesh.verticies.push_back(Vertex(Vector4D(1.0f, 1.0f, 0.0f, 1.0f), Vector4D(1.0f, 0.0f, 0.0f, 0.0f), Vector4D(0.0f, 0.0f, 0.0f, 0.0f)));//upper right

	Mesh mesh2;

	//top left triangle
	mesh2.verticies.push_back(Vertex(Vector4D(-1.0f, -1.0f, 0.0f, 1.0f), Vector4D(0.0f, 1.0f, 0.0f, 0.0f), Vector4D(0.0f, 0.0f, 0.0f, 0.0f)));//bottom left
	mesh2.verticies.push_back(Vertex(Vector4D(-1.0f, 1.0f, 0.0f, 1.0f), Vector4D(0.0f, 0.0f, 0.0f, 0.0f), Vector4D(0.0f, 0.0f, 0.0f, 0.0f)));//upper left
	mesh2.verticies.push_back(Vertex(Vector4D(1.0f, 1.0f, 0.0f, 1.0f), Vector4D(1.0f, 0.0f, 0.0f, 0.0f), Vector4D(0.0f, 0.0f, 0.0f, 0.0f)));//upper right

	//bottom right triangle
	mesh2.verticies.push_back(Vertex(Vector4D(1.0f, -1.0f, 0.0f, 1.0f), Vector4D(1.0f, 1.0f, 0.0f, 0.0f), Vector4D(0.0f, 0.0f, 0.0f, 0.0f)));//bottom right
	mesh2.verticies.push_back(Vertex(Vector4D(-1.0f, -1.0f, 0.0f, 1.0f), Vector4D(0.0f, 1.0f, 0.0f, 0.0f), Vector4D(0.0f, 0.0f, 0.0f, 0.0f)));//bottom left
	mesh2.verticies.push_back(Vertex(Vector4D(1.0f, 1.0f, 0.0f, 1.0f), Vector4D(1.0f, 0.0f, 0.0f, 0.0f), Vector4D(0.0f, 0.0f, 0.0f, 0.0f)));//upper right

	//bottom right triangle
	//mesh.verticies.push_back(Vertex(Vector4D(0.1, 0.0f, 0.0f, 1.0f), Vector4D(0.0f, 0.0f, 0.0f, 0.0f), Vector4D(0.0f, 0.0f, 0.0f, 0.0f)));//bottom right
	//mesh.verticies.push_back(Vertex(Vector4D(0.0f, -1.0f, 0.0f, 1.0f), Vector4D(0.0f, 1.0f, 0.0f, 0.0f), Vector4D(0.0f, 0.0f, 0.0f, 0.0f)));//bottom left
	//mesh.verticies.push_back(Vertex(Vector4D(1.0f, -1.0f, 0.0f, 1.0f), Vector4D(1.0f, 1.0f, 0.0f, 0.0f), Vector4D(0.0f, 0.0f, 0.0f, 0.0f)));//upper right
	
	for (float i = 0; i < 1000; i++) {
		//top left triangle
		mesh.verticies.push_back(Vertex(Vector4D(-1.0f, -1.0f, i, 1.0f), Vector4D(0.0f, 1.0f, 0.0f, 0.0f), Vector4D(0.0f, 0.0f, 0.0f, 0.0f)));//bottom left
		mesh.verticies.push_back(Vertex(Vector4D(-1.0f, 1.0f, i, 1.0f), Vector4D(0.0f, 0.0f, 0.0f, 0.0f), Vector4D(0.0f, 0.0f, 0.0f, 0.0f)));//upper left
		mesh.verticies.push_back(Vertex(Vector4D(1.0f, 1.0f, i, 1.0f), Vector4D(1.0f, 0.0f, 0.0f, 0.0f), Vector4D(0.0f, 0.0f, 0.0f, 0.0f)));//upper right

		//bottom right triangle
		mesh.verticies.push_back(Vertex(Vector4D(1.0f, -1.0f, i, 1.0f), Vector4D(1.0f, 1.0f, 0.0f, 0.0f), Vector4D(0.0f, 0.0f, 0.0f, 0.0f)));//bottom right
		mesh.verticies.push_back(Vertex(Vector4D(-1.0f, -1.0f, i, 1.0f), Vector4D(0.0f, 1.0f, 0.0f, 0.0f), Vector4D(0.0f, 0.0f, 0.0f, 0.0f)));//bottom left
		mesh.verticies.push_back(Vertex(Vector4D(1.0f, 1.0f, i, 1.0f), Vector4D(1.0f, 0.0f, 0.0f, 0.0f), Vector4D(0.0f, 0.0f, 0.0f, 0.0f)));//upper right
	}
	
	for (float i = 0; i < 5; i++) {
		//top left triangle
		mesh.verticies.push_back(Vertex(Vector4D(-1.0f, -1.0f, 0.1f + (i / 10), 1.0f), Vector4D(0.0f, 1.0f, 0.0f, 0.0f), Vector4D(0.0f, 0.0f, 0.0f, 0.0f)));//bottom left
		mesh.verticies.push_back(Vertex(Vector4D(-1.0f, 1.0f, 0.1f + (i / 10), 1.0f), Vector4D(0.0f, 0.0f, 0.0f, 0.0f), Vector4D(0.0f, 0.0f, 0.0f, 0.0f)));//upper left
		mesh.verticies.push_back(Vertex(Vector4D(1.0f, 1.0f, 0.1f + (i / 10), 1.0f), Vector4D(1.0f, 0.0f, 0.0f, 0.0f), Vector4D(0.0f, 0.0f, 0.0f, 0.0f)));//upper right

		//bottom right triangle
		mesh.verticies.push_back(Vertex(Vector4D(1.0f, -1.0f, 0.1f + (i / 10), 1.0f), Vector4D(1.0f, 1.0f, 0.0f, 0.0f), Vector4D(0.0f, 0.0f, 0.0f, 0.0f)));//bottom right
		mesh.verticies.push_back(Vertex(Vector4D(-1.0f, -1.0f, 0.1f + (i / 10), 1.0f), Vector4D(0.0f, 1.0f, 0.0f, 0.0f), Vector4D(0.0f, 0.0f, 0.0f, 0.0f)));//bottom left
		mesh.verticies.push_back(Vertex(Vector4D(1.0f, 1.0f, 0.1f + (i / 10), 1.0f), Vector4D(1.0f, 0.0f, 0.0f, 0.0f), Vector4D(0.0f, 0.0f, 0.0f, 0.0f)));//upper right
	}
	

	mesh.texture = new ImageBMP;
	mesh.texture->loadBMP("smiley");

	mesh2.texture = new ImageBMP;
	mesh2.texture->loadBMP("wallpaper");
	*/

	Mesh mesh;
	Mesh mesh2;
	Mesh mesh3;
	Mesh mesh4;
	Mesh mesh5;

	mesh.loadTexture("Textures/moskvitch");
	mesh.loadModel("Models/moskvitch");

	mesh2.loadTexture("Textures/Conditioner");
	mesh2.loadModel("Models/Conditioner");

	mesh3.loadTexture("Textures/Tavern");
	mesh3.loadModel("Models/Tavern");

	mesh4.loadTexture("Textures/Raptor");
	mesh4.loadModel("Models/Raptor");

	mesh5.loadTexture("Textures/ZombiDog");
	mesh5.loadModel("Models/ZombiDog");

//	mesh.texture->saveBMP();

	//***********************************************************Old StarField3D code
	/*
	//Vertex stars[100000];
	vector<Vertex> stars(10000);
	for (int i = 0; i < 10000; i++)
		stars[i] = Vertex(static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 10.0)) - 5.0, static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 10.0)) - 5.0, 0, 1);
	*/

	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		//********************************************************multithreading draw buffer attempt
		/*
		thread funcTest(&Win32WindowBuffer::FillBufferColor, &win32WindowBuffer, 0, 0, 0);
		funcTest.join();

		thread funcTest2(&Win32WindowBuffer::FillBufferColor, &win32WindowBuffer, 100, 100, 0);
		funcTest2.join();

		thread funcTest3(&Win32WindowBuffer::FillBufferColor, &win32WindowBuffer, 255, 0, 0);
		funcTest3.join();

		thread funcTest4(&Win32WindowBuffer::FillBufferColor, &win32WindowBuffer, 0, 255, 0);
		funcTest4.join();


		//win32WindowBuffer.FillBufferColor(255, 0, 255);

		for (int y = 0; y < win32WindowBuffer.clientHeight; y++) {
		for (int x = 0; x < win32WindowBuffer.clientWidth; x++) {
		char *pixelComponent = win32WindowBuffer.bytebuffer + ((y * win32WindowBuffer.clientWidth + x) * 4);
		*(pixelComponent) = (unsigned char)255;
		*(pixelComponent + 1) = (unsigned char)255;
		*(pixelComponent + 2) = (unsigned char)255;
		*(pixelComponent + 3) = (unsigned char)0;
		}
		}


		//400
		//100
		//50
		//37
		//28
		//23

		//win32WindowBuffer.FillBufferColor(0, 255, 255);
		//win32WindowBuffer.FillBufferColor(0, 255, 255);
		//win32WindowBuffer.FillBufferColor(0, 255, 255);
		//win32WindowBuffer.FillBufferColor(0, 255, 255);
		//win32WindowBuffer.FillBufferColor(0, 255, 255);
		//win32WindowBuffer.FillBufferColor(0, 255, 255);
		*/

		//********************************************************Old StarField3D code
		/*
		//update stars
		//double delta = hpc.mtimePerFrame;

		//for (int i = 0; i < 10000; i++) {
		//	stars[i].v.z -= delta * 10;
		//}

		//reset any stars that go out of screen (into negative z)
		for (int j = 0; j < 1; j++) {
			for (int i = 0; i < 10000; i++) {
				if (stars[i].v.z <= 0) {
					//stars[i] = Vertex(static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 10.0)) - 5.0, static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 10.0)) - 5.0, 0, 1);
				}
			}
		}
		*/

		//***********************************************************Old starfield code
		/*
		static double rot;
		rot += delta * 100;

		//Matrix4x4 modelRotate;
		//modelRotate.setYrot(rot);
		//modelRotate.setRotArb(-1, 0, 0, rot);
		//Matrix4x4 modelTranslate;
		//modelTranslate.setTranslate(0, -200, 300);
		Matrix4x4 model;// = modelRotate * modelTranslate;

		Matrix4x4 viewProjection = model * cam.viewMatrix.inverse() * cam.projectionMatrix;

		//project verticies, divide by z (w), transform from NDC to screen space, finally draw 
		for (int i = 0; i < 10000; i++) {
			Vertex v = (stars[i].v * viewProjection);
			v.v /= v.v.w;

			if (v.v.x <= -1.0 || v.v.x >= 1.0 || v.v.y <= -1.0 || v.v.y >= 1.0 || v.v.z <= 0 || v.v.z >= 1)
				continue;

			int screenX = (int)((v.v.x + 1) * 0.5 * win32WindowBuffer.clientWidth);
			int screenY = (int)((v.v.y + 1) * 0.5 * win32WindowBuffer.clientHeight);

			char *pixelComponent = win32WindowBuffer.bytebuffer + ((screenY * win32WindowBuffer.clientWidth + screenX) * 4);
			*(pixelComponent) = (unsigned char)255;
			*(pixelComponent + 1) = (unsigned char)255;
			*(pixelComponent + 2) = (unsigned char)255;
			*(pixelComponent + 3) = (unsigned char)0;
		}
		*/

		//*********************************************************Get delta (time spent rendering a single frame in miliseconds)
		float delta = (float)hpc.mtimePerFrame;
		
		static float rot = 0;
		rot += delta;

		Matrix4x4 rotate;
		rotate.setYrot(0);
		Matrix4x4 scale;
		scale.setScale(1, 1, 1);
		Matrix4x4 translate;
		translate.setTranslate(0, 0, 2);
		mesh.modelMesh = rotate * scale * translate;

		rotate.setIdentity();
		rotate.setYrot(0);
		scale.setIdentity();
		scale.setScale(0.005, 0.005, 0.005);
		translate.setIdentity();
		translate.setTranslate(5, 0, 0);
		mesh2.modelMesh = rotate * scale * translate;

		rotate.setIdentity();
		rotate.setYrot(0);
		scale.setIdentity();
		scale.setScale(1, 1, 1);
		translate.setIdentity();
		translate.setTranslate(-10, 0, 0);
		mesh3.modelMesh = rotate * scale * translate;

		rotate.setIdentity();
		rotate.setYrot(0);
		scale.setIdentity();
		scale.setScale(0.01, 0.01, 0.01);
		translate.setIdentity();
		translate.setTranslate(3, 0, 2);
		mesh4.modelMesh = rotate * scale * translate;

		rotate.setIdentity();
		rotate.setYrot(0);
		scale.setIdentity();
		scale.setScale(1, 1, 1);
		translate.setIdentity();
		translate.setTranslate(3, 0, 0);
		mesh5.modelMesh = rotate * scale * translate;

		win32WindowBuffer.FillBufferBlack();
		//win32WindowBuffer.FillBufferColor(255, 255, 255);
		pipeline.clearDepthBuffer();

		//*********************************************************Get Mouse Input and move Camera
		cam.getInput(delta); //delta used to smooth mouse relative to how fast framerate is
		cam.calculateViewMatrix(); //after rotating lookDirection vector, recalculate camera matrix

		//**********************************************************Render mesh objects

		pipeline.transform(&mesh);
		rotate.setYrot(0);
		scale.setScale(1, 1, 1);
		translate.setTranslate(0, 0, 6);
		mesh.modelMesh = rotate * scale * translate;
		pipeline.transform(&mesh);

		pipeline.transform(&mesh2);
		pipeline.transform(&mesh3);
		pipeline.transform(&mesh4);
		pipeline.transform(&mesh5);

		//pipeline.transform verticies
			//perform z-culling first
			//project, (don't divide by w)
		//pipeline.clip verticies
			//check if entire triangle lies outside viewing frustum
			//clip in homogenous clip space
		//pipeline. shade triangle (model, lights)
			//sort verticies
			//generate vertex, 1/z depth, z depth, lighting, and texture gradients
			//step down (y axis) scan line, draw from left to right
			//interpolate the textures, 1/z depth, z depth, and lighting





		win32WindowBuffer.drawBuffer();

		hpc.Tick();
		//if (hpc.mtimePerFrame > 0.033)
			//Sleep(1000.0 * (hpc.mtimePerFrame - 0.033));
		displayFPS(hpc, *win32WindowBuffer.getWindowHandle());
		
	}

	return (int)msg.wParam;
}