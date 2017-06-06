#include "Camera.h"
#include "PerspectiveProjection.h"
#include "ProcessVertex.h"
#include "Vertex.h"
#include "WorldViewProjection.h"
#include "Object.h"
#include "Triangle.h"
#include <iostream>
#include "SDL.h"
using namespace std;

bool processVerticiesb = false;

double xReplace = 0;
double yReplace = 0;

void processVerticiesf(Object object, double halfScreenWidth, double halfScreenHeight, SDL_Surface* screen, bool processTriangle){
	if (processTriangle == false){
		for (unsigned int atX = 0; atX < object.objVerticies.size(); atX++){
			double zDepth = object.objVerticies.at(atX).getZPrime();
			if (object.objVerticies.at(atX).getXPrime() < 1 && object.objVerticies.at(atX).getXPrime() > -1 && object.objVerticies.at(atX).getYPrime() < 1 && object.objVerticies.at(atX).getYPrime() > -1 && zDepth > 0 && zDepth < 1){
				if (object.objVerticies.at(atX).getXPrime() < 0)
					xReplace = (halfScreenWidth + ((object.objVerticies.at(atX).getXPrime()) * halfScreenWidth));
				else
					xReplace = (halfScreenWidth + (object.objVerticies.at(atX).getXPrime() * halfScreenWidth));

				if (object.objVerticies.at(atX).getYPrime() > 0)
					yReplace = (halfScreenHeight - ((object.objVerticies.at(atX).getYPrime()) * halfScreenHeight));
				else
					yReplace = (halfScreenHeight - ((object.objVerticies.at(atX).getYPrime()) * halfScreenHeight));

				placePixel(screen, xReplace, yReplace, object.returnRed(), object.returnGreen(), object.returnBlue());
			}
		}
	}

	if (processTriangle){
		Triangle triangle;
		triangle.setWidthAndHeight(halfScreenWidth, halfScreenHeight);
		bool testTriangle = true;
		for (int triVertex = 0; triVertex < object.objVerticies.size(); triVertex += 3){
			if (testTriangle)
				triangle.testInsdieTriangle(object, triVertex);

			if (triangle.getInsideImagePlane()){
				triangle.sortVerticies(object, triVertex);//sorts verticies from top to bottom and calculates slopes
				triangle.calculateLineEquation();

				while (triangle.getScreenY() < triangle.getMiddleY()){
					triangle.calculatePixelXTopHalf();
					triangle.placePixelOnScreen(screen, object);
				}
				
				while (triangle.getScreenY() < triangle.getBottomY() - 1){
					triangle.calculatePixelXBottomHalf();
					triangle.placePixelOnScreen(screen, object);
				}
				//calculate equation of a line and put the variables inside Triangle class
				//calculate where x is from slopeMidToTop and where x is from slopeBotToTop
				//sort x small to big
				//fill in pixels between
			}
			else{
				triangle.setInsideImagePlane(true);
			}
		}
	}
}

void placePixel(SDL_Surface* screen, int x, int y, int r, int g, int b){
	Uint32* pixels = (Uint32*)screen->pixels;
	Uint32* pixel = pixels + y * screen->pitch / 4 + x;
	*pixel = SDL_MapRGB(screen->format, r, g, b);
}




