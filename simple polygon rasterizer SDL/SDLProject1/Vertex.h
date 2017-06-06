#ifndef Vertex_H
#define Vertex_H

#include <math.h>
#include <iostream>
using namespace std;


//The vertex is being used as a vector class in the Cam Class
class Vertex
{
private:
	double x;
	double y;
	double z;
	double w;

	double xHomo;
	double yHomo;
	double zHomo;

	double xPrime;
	double yPrime;
	double zPrime;
	double wPrime;

public:

	Vertex()
	{
		x = 0;
		y = 0;
		z = 0;
		w = 1;

		xHomo = yHomo = zHomo = 0;

		xPrime = 0;
		yPrime = 0;
		zPrime = 0;
		wPrime = 0;
	}

	Vertex(double setX, double setY, double setZ, double setW = 1) : x(setX), y(setY), z(setZ), w(setW)
	{

	}

	void setX(double sX)
	{
		x = sX;
	}

	void setY(double sY)
	{
		y = sY;
	}

	void setZ(double sZ)
	{
		z = sZ;
	}

	void setW(double sW)
	{
		w = sW;
	}

	void setXPrime(double sX)
	{
		xPrime = sX;
	}

	void setYPrime(double sY)
	{
		yPrime = sY;
	}

	void setZPrime(double sZ)
	{
		zPrime = sZ;
	}

	void setWPrime(double sW)
	{
		wPrime = sW;
	}

	void setXHomo(double sX){
		xHomo = sX;
	}

	void setYHomo(double sY){
		yHomo = sY;
	}

	void setZHomo(double sZ){
		zHomo = sZ;
	}

	double getXHomo(){
		return xHomo;
	}

	double getYHomo(){
		return yHomo;
	}

	double getZHomo(){
		return zHomo;
	}

	double getX()
	{
		return x;
	}

	double getY()
	{
		return y;
	}

	double getZ()
	{
		return z;
	}

	double getW()
	{
		return w;
	}

	double getXPrime()
	{
		return xPrime;
	}

	double getYPrime()
	{
		return yPrime;
	}

	double getZPrime()
	{
		return zPrime;
	}

	double getWPrime()
	{
		return wPrime;
	}

	void normalize()
	{
		double xUnChanged = x;
		double yUnChanged = y;
		double zUnChanged = z;
		x = xUnChanged / sqrt((xUnChanged  * xUnChanged) + (yUnChanged  * yUnChanged) + (zUnChanged  * zUnChanged));
		y = yUnChanged / sqrt((xUnChanged  * xUnChanged) + (yUnChanged  * yUnChanged) + (zUnChanged  * zUnChanged));
		z = zUnChanged / sqrt((xUnChanged  * xUnChanged) + (yUnChanged  * yUnChanged) + (zUnChanged  * zUnChanged));
	}

	void displayVertexToConsole()
	{
		cout << "VERTEX" << endl;
		cout << x << " " << y << " " << z << " " << w << endl;
	}

	void displayVertexPrimeToConsole()
	{
		cout << "VERTEX PRIME" << endl;
		cout << xPrime << " " << yPrime << " " << zPrime <<  " " << wPrime << endl;
	}
};

#endif