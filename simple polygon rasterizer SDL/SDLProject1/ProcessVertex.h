#ifndef PROCESSVERTEX_H
#define PROCESSVERTEX_H

#include "Camera.h"
#include "PerspectiveProjection.h"
#include "ProcessVertex.h"
#include "Vertex.h"
#include "WorldViewProjection.h"
#include "Object.h"
#include <iostream>
#include "SDL.h"

void processVerticiesf(Object object, double halfScreenWidth, double halfScreenHeight, SDL_Surface* screen, bool processTriangle = false);

void placePixel(SDL_Surface* screen, int x, int y, int r, int g, int b);

void arrangeVerticiesOnScreen(Vertex vertex, int halfscreenHeight, int halfscreenWidth);

void testVerticies(Vertex vertex);

#endif