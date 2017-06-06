#ifndef SCANLINE_H
#define SCANLINE_H

#include "Camera.h"
#include "PerspectiveProjection.h"
#include "ProcessVertex.h"
#include "Vertex.h"
#include "WorldViewProjection.h"
#include "Object.h"
#include <iostream>
#include "SDL.h"

class Triangle{
private:

	bool insideImagePlane;
	double topY;
	double topX;
	double middleY;
	double middleX;
	double bottomY;
	double bottomX;
	double slopeMidToTop;
	double slopeBotToTop;
	double slopeBotToMid;
	double halfScreenWidth;
	double halfScreenHeight;
	double pixelXOne;
	double pixelXTwo;
	double screenY;
	double bValueOne;
	double bValueTwo;
	double bValueThree;
	double screenXOne;
	double screenXTwo;
public:
	Triangle(){
		insideImagePlane = true;
		topY = topX = middleY = middleX = bottomY = bottomX = 0;
		slopeMidToTop = slopeBotToMid = slopeBotToTop = 0;
		halfScreenWidth = halfScreenHeight = 0;
		pixelXOne = pixelXTwo = 0;
		screenY = 0;
		bValueOne = bValueTwo = bValueThree = 0;
		screenXOne = screenXTwo = 0;
	}

	void setWidthAndHeight(double halfwidth, double halfheight){
		halfScreenHeight = halfheight;
		halfScreenWidth = halfwidth;
	}

	bool getInsideImagePlane(){
		return insideImagePlane;
	}

	void setInsideImagePlane(bool insidePlane){
		insideImagePlane = insidePlane;
	}

	void testInsdieTriangle(Object object, int index){
		for (int indexSpot = index; indexSpot < index + 3; indexSpot++){
			double zDepth = object.objVerticies.at(indexSpot).getZPrime();
			if (object.objVerticies.at(indexSpot).getXPrime() > 1 || object.objVerticies.at(indexSpot).getXPrime() < -1 || object.objVerticies.at(indexSpot).getYPrime() > 1 || object.objVerticies.at(indexSpot).getYPrime() < -1 || zDepth < 0 || zDepth > 1){
				insideImagePlane = false;
				break;
			}
		}
	}

	void sortVerticies(Object object, int index){
		
		for (int indexing = index; indexing < index + 3; indexing++){
			if (object.objVerticies.at(indexing).getXPrime() < 0)
				object.objVerticies.at(indexing).setXPrime((halfScreenWidth + ((object.objVerticies.at(indexing).getXPrime()) * halfScreenWidth)));
			else
				object.objVerticies.at(indexing).setXPrime((halfScreenWidth + (object.objVerticies.at(indexing).getXPrime() * halfScreenWidth)));

			if (object.objVerticies.at(indexing).getYPrime() > 0)
				object.objVerticies.at(indexing).setYPrime((halfScreenHeight - ((object.objVerticies.at(indexing).getYPrime()) * halfScreenHeight)));
			else
				object.objVerticies.at(indexing).setYPrime((halfScreenHeight - ((object.objVerticies.at(indexing).getYPrime()) * halfScreenHeight)));
		}
		
		
		topX = object.objVerticies.at(index).getXPrime();
		topY = object.objVerticies.at(index).getYPrime();

		middleX = object.objVerticies.at(index + 1).getXPrime();
		middleY = object.objVerticies.at(index + 1).getYPrime();

		bottomX = object.objVerticies.at(index + 2).getXPrime();
		bottomY = object.objVerticies.at(index + 2).getYPrime();
		
		if (middleY < topY){
			double tempTopY = topY;
			double tempTopX = topX;

			topY = middleY;
			topX = middleX;
		
			middleY = tempTopY;
			middleX = tempTopX;
		}

		if (bottomY < topY){
			double tempTopY = topY;
			double tempTopX = topX;

			topY = bottomY;
			topX = bottomX;

			bottomY = tempTopY;
			bottomX = tempTopX;
		}

		if (bottomY < middleY){
			double tempMidY = middleY;
			double tempMidX = middleX;

			middleY = bottomY;
			middleX = bottomX;

			bottomY = tempMidY;
			bottomX = tempMidX;
		}

		//cout << "TOP " << "X: " << topX << " Y: " << topY << endl;
		//cout << "MID " << "X: " << middleX << " Y: " << middleY << endl;
		//cout << "Bottom" << "X: " << bottomX << " Y: " << bottomY << endl;

		if (topY && middleY == 0)
			slopeMidToTop = 0;
		else if (topX && middleX == 0)
			slopeMidToTop = 0;
		else
			slopeMidToTop = (topY - middleY) / (topX - middleX);

		if (topY && bottomY == 0)
			slopeBotToTop = 0;
		else if (topX && bottomX == 0)
			slopeBotToTop = 0;
		else
			slopeBotToTop = (topY - bottomY) / (topX - bottomX);
		
		if (middleY && bottomY == 0)
			slopeBotToMid = 0;
		else if (middleX && bottomX == 0)
			slopeBotToMid = 0;
		else
			slopeBotToMid = (middleY - bottomY) / (middleX - bottomX);

		//cout << "Slope MIDTOTOP " << slopeMidToTop << endl;
		//cout << "Slope BotToTop " << slopeBotToTop << endl;
		//cout << "Slope BotToMiddle " << slopeBotToMid << endl;
	}

	void calculateLineEquation(){
		bValueOne = middleY - (slopeMidToTop * middleX);
		bValueTwo = topY - (slopeBotToTop * topX);
		bValueThree = middleY - (slopeBotToMid * middleX);
		//cout << bValueOne << " \ " << bValueTwo << " \ " << bValueThree << endl;
		screenY = topY;
	}

	void calculatePixelXTopHalf(){
		screenXOne = (screenY - bValueOne) / slopeMidToTop;
		screenXTwo = (screenY - bValueTwo) / slopeBotToTop;

		//cout << bValueOne << " \ " << bValueTwo << endl;

		if (screenXOne < 0)
			screenXOne = -screenXOne;
		if (screenXTwo < 0)
			screenXTwo = -screenXTwo;

		if (screenXTwo < screenXOne){
			double tempScreenX = screenXTwo;
			screenXTwo = screenXOne;
			screenXOne = tempScreenX;
		}

		screenY++;
		//cout << "SCREENX " << screenXOne << " \ " << screenXTwo << endl;
	}

	void calculatePixelXBottomHalf(){
		screenXOne = (screenY - bValueThree) / slopeBotToMid;
		screenXTwo = (screenY - bValueTwo) / slopeBotToTop;

		//cout << bValueOne << " \ " << bValueTwo << endl;

		if (screenXOne < 0)
			screenXOne = -screenXOne;
		if (screenXTwo < 0)
			screenXTwo = -screenXTwo;

		if (screenXTwo < screenXOne){
			double tempScreenX = screenXTwo;
			screenXTwo = screenXOne;
			screenXOne = tempScreenX;
		}

		screenY++;
		//cout << "SCREENX " << screenXOne << " \ " << screenXTwo << endl;
	}

	void placePixelOnScreen(SDL_Surface* screen, Object object){
		double tempX = screenXOne;
		//cout << "SCREENX " << screenXOne << " \ " << screenXTwo << endl;
		while (tempX < screenXTwo){
			placePixel(screen, tempX, screenY, object.returnRed(), object.returnGreen(), object.returnBlue());
			tempX++;
			//cout << "SCREENX " << screenXOne << " \ " << screenXTwo << endl;
		}
	}

	double getMiddleY(){
		return middleY;
	}

	double getBottomY(){
		return bottomY;
	}

	double getScreenY(){
		return screenY;
	}
};

#endif