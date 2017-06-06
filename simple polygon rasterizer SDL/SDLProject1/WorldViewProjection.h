#ifndef WORLDVIEWPROJECTION_H
#define WORLDVIEWPROJECTION_H

#include <iostream>
#include "PerspectiveProjection.h"
#include "Camera.h"
#include "Vertex.h"
#include "Object.h"
using namespace std;

class WVP
{
private:
	double wvpMatrix[4][4];
	double wvpMatrixCopy[4][4];
	double wvpMatrixCopyTwo[4][4];
	double wvpMatrixCopyThree[4][4];
public:
	WVP()
	{
		for (int x = 0; x < 4; x++){
			for (int y = 0; y < 4; y++){
				wvpMatrix[x][y] = 0;
				wvpMatrixCopy[x][y] = 0;
				wvpMatrixCopyTwo[x][y] = 0;
				wvpMatrixCopyThree[x][y] = 0;
			}
		}
	}

	void multiplyWorldViewProjection(Object objMatrix, Camera camMatrix, PerspectiveProj perspProj){
		//Multiply objMatrix * InversecamMatrix
		wvpMatrixCopy[0][0] = (objMatrix.getElement(11) * camMatrix.getInverseMatrixElement(11)) + (objMatrix.getElement(12) * camMatrix.getInverseMatrixElement(21)) + (objMatrix.getElement(13) * camMatrix.getInverseMatrixElement(31)) + (objMatrix.getElement(14) * camMatrix.getInverseMatrixElement(41));
		wvpMatrixCopy[0][1] = (objMatrix.getElement(11) * camMatrix.getInverseMatrixElement(12)) + (objMatrix.getElement(12) * camMatrix.getInverseMatrixElement(22)) + (objMatrix.getElement(13) * camMatrix.getInverseMatrixElement(32)) + (objMatrix.getElement(14) * camMatrix.getInverseMatrixElement(42));
		wvpMatrixCopy[0][2] = (objMatrix.getElement(11) * camMatrix.getInverseMatrixElement(13)) + (objMatrix.getElement(12) * camMatrix.getInverseMatrixElement(23)) + (objMatrix.getElement(13) * camMatrix.getInverseMatrixElement(33)) + (objMatrix.getElement(14) * camMatrix.getInverseMatrixElement(43));
		wvpMatrixCopy[0][3] = (objMatrix.getElement(11) * camMatrix.getInverseMatrixElement(14)) + (objMatrix.getElement(12) * camMatrix.getInverseMatrixElement(24)) + (objMatrix.getElement(13) * camMatrix.getInverseMatrixElement(34)) + (objMatrix.getElement(14) * camMatrix.getInverseMatrixElement(44));

		wvpMatrixCopy[1][0] = (objMatrix.getElement(21) * camMatrix.getInverseMatrixElement(11)) + (objMatrix.getElement(22) * camMatrix.getInverseMatrixElement(21)) + (objMatrix.getElement(23) * camMatrix.getInverseMatrixElement(31)) + (objMatrix.getElement(24) * camMatrix.getInverseMatrixElement(41));
		wvpMatrixCopy[1][1] = (objMatrix.getElement(21) * camMatrix.getInverseMatrixElement(12)) + (objMatrix.getElement(22) * camMatrix.getInverseMatrixElement(22)) + (objMatrix.getElement(23) * camMatrix.getInverseMatrixElement(32)) + (objMatrix.getElement(24) * camMatrix.getInverseMatrixElement(42));
		wvpMatrixCopy[1][2] = (objMatrix.getElement(21) * camMatrix.getInverseMatrixElement(13)) + (objMatrix.getElement(22) * camMatrix.getInverseMatrixElement(23)) + (objMatrix.getElement(23) * camMatrix.getInverseMatrixElement(33)) + (objMatrix.getElement(24) * camMatrix.getInverseMatrixElement(43));
		wvpMatrixCopy[1][3] = (objMatrix.getElement(21) * camMatrix.getInverseMatrixElement(14)) + (objMatrix.getElement(22) * camMatrix.getInverseMatrixElement(24)) + (objMatrix.getElement(23) * camMatrix.getInverseMatrixElement(34)) + (objMatrix.getElement(24) * camMatrix.getInverseMatrixElement(44));

		wvpMatrixCopy[2][0] = (objMatrix.getElement(31) * camMatrix.getInverseMatrixElement(11)) + (objMatrix.getElement(32) * camMatrix.getInverseMatrixElement(21)) + (objMatrix.getElement(33) * camMatrix.getInverseMatrixElement(31)) + (objMatrix.getElement(34) * camMatrix.getInverseMatrixElement(41));
		wvpMatrixCopy[2][1] = (objMatrix.getElement(31) * camMatrix.getInverseMatrixElement(12)) + (objMatrix.getElement(32) * camMatrix.getInverseMatrixElement(22)) + (objMatrix.getElement(33) * camMatrix.getInverseMatrixElement(32)) + (objMatrix.getElement(34) * camMatrix.getInverseMatrixElement(42));
		wvpMatrixCopy[2][2] = (objMatrix.getElement(31) * camMatrix.getInverseMatrixElement(13)) + (objMatrix.getElement(32) * camMatrix.getInverseMatrixElement(23)) + (objMatrix.getElement(33) * camMatrix.getInverseMatrixElement(33)) + (objMatrix.getElement(34) * camMatrix.getInverseMatrixElement(43));
		wvpMatrixCopy[2][3] = (objMatrix.getElement(31) * camMatrix.getInverseMatrixElement(14)) + (objMatrix.getElement(32) * camMatrix.getInverseMatrixElement(24)) + (objMatrix.getElement(33) * camMatrix.getInverseMatrixElement(34)) + (objMatrix.getElement(34) * camMatrix.getInverseMatrixElement(44));

		wvpMatrixCopy[3][0] = (objMatrix.getElement(41) * camMatrix.getInverseMatrixElement(11)) + (objMatrix.getElement(42) * camMatrix.getInverseMatrixElement(21)) + (objMatrix.getElement(43) * camMatrix.getInverseMatrixElement(31)) + (objMatrix.getElement(44) * camMatrix.getInverseMatrixElement(41));
		wvpMatrixCopy[3][1] = (objMatrix.getElement(41) * camMatrix.getInverseMatrixElement(12)) + (objMatrix.getElement(42) * camMatrix.getInverseMatrixElement(22)) + (objMatrix.getElement(43) * camMatrix.getInverseMatrixElement(32)) + (objMatrix.getElement(44) * camMatrix.getInverseMatrixElement(42));
		wvpMatrixCopy[3][2] = (objMatrix.getElement(41) * camMatrix.getInverseMatrixElement(13)) + (objMatrix.getElement(42) * camMatrix.getInverseMatrixElement(23)) + (objMatrix.getElement(43) * camMatrix.getInverseMatrixElement(33)) + (objMatrix.getElement(44) * camMatrix.getInverseMatrixElement(43));
		wvpMatrixCopy[3][3] = (objMatrix.getElement(41) * camMatrix.getInverseMatrixElement(14)) + (objMatrix.getElement(42) * camMatrix.getInverseMatrixElement(24)) + (objMatrix.getElement(43) * camMatrix.getInverseMatrixElement(34)) + (objMatrix.getElement(44) * camMatrix.getInverseMatrixElement(44));
		
		/*
		wvpMatrixCopy[0][0] = camMatrix.getInverseMatrixElement(11);
		wvpMatrixCopy[0][1] = camMatrix.getInverseMatrixElement(12);
		wvpMatrixCopy[0][2] = camMatrix.getInverseMatrixElement(13);
		wvpMatrixCopy[0][3] = camMatrix.getInverseMatrixElement(14);

		wvpMatrixCopy[1][0] = camMatrix.getInverseMatrixElement(21);
		wvpMatrixCopy[1][1] = camMatrix.getInverseMatrixElement(22);
		wvpMatrixCopy[1][2] = camMatrix.getInverseMatrixElement(23);
		wvpMatrixCopy[1][3] = camMatrix.getInverseMatrixElement(24);

		wvpMatrixCopy[2][0] = camMatrix.getInverseMatrixElement(31);
		wvpMatrixCopy[2][1] = camMatrix.getInverseMatrixElement(32);
		wvpMatrixCopy[2][2] = camMatrix.getInverseMatrixElement(33);
		wvpMatrixCopy[2][3] = camMatrix.getInverseMatrixElement(34);

		wvpMatrixCopy[3][0] = camMatrix.getInverseMatrixElement(41);
		wvpMatrixCopy[3][1] = camMatrix.getInverseMatrixElement(42);
		wvpMatrixCopy[3][2] = camMatrix.getInverseMatrixElement(43);
		wvpMatrixCopy[3][3] = camMatrix.getInverseMatrixElement(44);
		*/
		/*
		//Multiply (objmatrix * InverseCamMatrix) * camTranslation Matrix
		wvpMatrixCopyTwo[0][0] = (wvpMatrixCopy[0][0] * camMatrix.getTranslationElement(11)) + (wvpMatrixCopy[0][1] * camMatrix.getTranslationElement(21)) + (wvpMatrixCopy[0][2] * camMatrix.getTranslationElement(31)) + (wvpMatrixCopy[0][3] * camMatrix.getTranslationElement(41));
		wvpMatrixCopyTwo[0][1] = (wvpMatrixCopy[0][0] * camMatrix.getTranslationElement(12)) + (wvpMatrixCopy[0][1] * camMatrix.getTranslationElement(22)) + (wvpMatrixCopy[0][2] * camMatrix.getTranslationElement(32)) + (wvpMatrixCopy[0][3] * camMatrix.getTranslationElement(42));
		wvpMatrixCopyTwo[0][2] = (wvpMatrixCopy[0][0] * camMatrix.getTranslationElement(13)) + (wvpMatrixCopy[0][1] * camMatrix.getTranslationElement(23)) + (wvpMatrixCopy[0][2] * camMatrix.getTranslationElement(33)) + (wvpMatrixCopy[0][3] * camMatrix.getTranslationElement(43));
		wvpMatrixCopyTwo[0][3] = (wvpMatrixCopy[0][0] * camMatrix.getTranslationElement(14)) + (wvpMatrixCopy[0][1] * camMatrix.getTranslationElement(24)) + (wvpMatrixCopy[0][2] * camMatrix.getTranslationElement(34)) + (wvpMatrixCopy[0][3] * camMatrix.getTranslationElement(44));

		wvpMatrixCopyTwo[1][0] = (wvpMatrixCopy[1][0] * camMatrix.getTranslationElement(11)) + (wvpMatrixCopy[1][1] * camMatrix.getTranslationElement(21)) + (wvpMatrixCopy[1][2] * camMatrix.getTranslationElement(31)) + (wvpMatrixCopy[1][3] * camMatrix.getTranslationElement(41));
		wvpMatrixCopyTwo[1][1] = (wvpMatrixCopy[1][0] * camMatrix.getTranslationElement(12)) + (wvpMatrixCopy[1][1] * camMatrix.getTranslationElement(22)) + (wvpMatrixCopy[1][2] * camMatrix.getTranslationElement(32)) + (wvpMatrixCopy[1][3] * camMatrix.getTranslationElement(42));
		wvpMatrixCopyTwo[1][2] = (wvpMatrixCopy[1][0] * camMatrix.getTranslationElement(13)) + (wvpMatrixCopy[1][1] * camMatrix.getTranslationElement(23)) + (wvpMatrixCopy[1][2] * camMatrix.getTranslationElement(33)) + (wvpMatrixCopy[1][3] * camMatrix.getTranslationElement(43));
		wvpMatrixCopyTwo[1][3] = (wvpMatrixCopy[1][0] * camMatrix.getTranslationElement(14)) + (wvpMatrixCopy[1][1] * camMatrix.getTranslationElement(24)) + (wvpMatrixCopy[1][2] * camMatrix.getTranslationElement(34)) + (wvpMatrixCopy[1][3] * camMatrix.getTranslationElement(44));

		wvpMatrixCopyTwo[2][0] = (wvpMatrixCopy[2][0] * camMatrix.getTranslationElement(11)) + (wvpMatrixCopy[2][1] * camMatrix.getTranslationElement(21)) + (wvpMatrixCopy[2][2] * camMatrix.getTranslationElement(31)) + (wvpMatrixCopy[2][3] * camMatrix.getTranslationElement(41));
		wvpMatrixCopyTwo[2][1] = (wvpMatrixCopy[2][0] * camMatrix.getTranslationElement(12)) + (wvpMatrixCopy[2][1] * camMatrix.getTranslationElement(22)) + (wvpMatrixCopy[2][2] * camMatrix.getTranslationElement(32)) + (wvpMatrixCopy[2][3] * camMatrix.getTranslationElement(42));
		wvpMatrixCopyTwo[2][2] = (wvpMatrixCopy[2][0] * camMatrix.getTranslationElement(13)) + (wvpMatrixCopy[2][1] * camMatrix.getTranslationElement(23)) + (wvpMatrixCopy[2][2] * camMatrix.getTranslationElement(33)) + (wvpMatrixCopy[2][3] * camMatrix.getTranslationElement(43));
		wvpMatrixCopyTwo[2][3] = (wvpMatrixCopy[2][0] * camMatrix.getTranslationElement(14)) + (wvpMatrixCopy[2][1] * camMatrix.getTranslationElement(24)) + (wvpMatrixCopy[2][2] * camMatrix.getTranslationElement(34)) + (wvpMatrixCopy[2][3] * camMatrix.getTranslationElement(44));

		wvpMatrixCopyTwo[3][0] = (wvpMatrixCopy[3][0] * camMatrix.getTranslationElement(11)) + (wvpMatrixCopy[3][1] * camMatrix.getTranslationElement(21)) + (wvpMatrixCopy[3][2] * camMatrix.getTranslationElement(31)) + (wvpMatrixCopy[3][3] * camMatrix.getTranslationElement(41));
		wvpMatrixCopyTwo[3][1] = (wvpMatrixCopy[3][0] * camMatrix.getTranslationElement(12)) + (wvpMatrixCopy[3][1] * camMatrix.getTranslationElement(22)) + (wvpMatrixCopy[3][2] * camMatrix.getTranslationElement(32)) + (wvpMatrixCopy[3][3] * camMatrix.getTranslationElement(42));
		wvpMatrixCopyTwo[3][2] = (wvpMatrixCopy[3][0] * camMatrix.getTranslationElement(13)) + (wvpMatrixCopy[3][1] * camMatrix.getTranslationElement(23)) + (wvpMatrixCopy[3][2] * camMatrix.getTranslationElement(33)) + (wvpMatrixCopy[3][3] * camMatrix.getTranslationElement(43));
		wvpMatrixCopyTwo[3][3] = (wvpMatrixCopy[3][0] * camMatrix.getTranslationElement(14)) + (wvpMatrixCopy[3][1] * camMatrix.getTranslationElement(24)) + (wvpMatrixCopy[3][2] * camMatrix.getTranslationElement(34)) + (wvpMatrixCopy[3][3] * camMatrix.getTranslationElement(44));
		
		//Multiply (objMatrix * InverseCamMatrix * camTranslation) * camRotation
		wvpMatrixCopyThree[0][0] = (wvpMatrixCopyTwo[0][0] * camMatrix.getRotationElement(11)) + (wvpMatrixCopyTwo[0][1] * camMatrix.getRotationElement(21)) + (wvpMatrixCopyTwo[0][2] * camMatrix.getRotationElement(31)) + (wvpMatrixCopyTwo[0][3] * camMatrix.getRotationElement(41));
		wvpMatrixCopyThree[0][1] = (wvpMatrixCopyTwo[0][0] * camMatrix.getRotationElement(12)) + (wvpMatrixCopyTwo[0][1] * camMatrix.getRotationElement(22)) + (wvpMatrixCopyTwo[0][2] * camMatrix.getRotationElement(32)) + (wvpMatrixCopyTwo[0][3] * camMatrix.getRotationElement(42));
		wvpMatrixCopyThree[0][2] = (wvpMatrixCopyTwo[0][0] * camMatrix.getRotationElement(13)) + (wvpMatrixCopyTwo[0][1] * camMatrix.getRotationElement(23)) + (wvpMatrixCopyTwo[0][2] * camMatrix.getRotationElement(33)) + (wvpMatrixCopyTwo[0][3] * camMatrix.getRotationElement(43));
		wvpMatrixCopyThree[0][3] = (wvpMatrixCopyTwo[0][0] * camMatrix.getRotationElement(14)) + (wvpMatrixCopyTwo[0][1] * camMatrix.getRotationElement(24)) + (wvpMatrixCopyTwo[0][2] * camMatrix.getRotationElement(34)) + (wvpMatrixCopyTwo[0][3] * camMatrix.getRotationElement(44));

		wvpMatrixCopyThree[1][0] = (wvpMatrixCopyTwo[1][0] * camMatrix.getRotationElement(11)) + (wvpMatrixCopyTwo[1][1] * camMatrix.getRotationElement(21)) + (wvpMatrixCopyTwo[1][2] * camMatrix.getRotationElement(31)) + (wvpMatrixCopyTwo[1][3] * camMatrix.getRotationElement(41));
		wvpMatrixCopyThree[1][1] = (wvpMatrixCopyTwo[1][0] * camMatrix.getRotationElement(12)) + (wvpMatrixCopyTwo[1][1] * camMatrix.getRotationElement(22)) + (wvpMatrixCopyTwo[1][2] * camMatrix.getRotationElement(32)) + (wvpMatrixCopyTwo[1][3] * camMatrix.getRotationElement(42));
		wvpMatrixCopyThree[1][2] = (wvpMatrixCopyTwo[1][0] * camMatrix.getRotationElement(13)) + (wvpMatrixCopyTwo[1][1] * camMatrix.getRotationElement(23)) + (wvpMatrixCopyTwo[1][2] * camMatrix.getRotationElement(33)) + (wvpMatrixCopyTwo[1][3] * camMatrix.getRotationElement(43));
		wvpMatrixCopyThree[1][3] = (wvpMatrixCopyTwo[1][0] * camMatrix.getRotationElement(14)) + (wvpMatrixCopyTwo[1][1] * camMatrix.getRotationElement(24)) + (wvpMatrixCopyTwo[1][2] * camMatrix.getRotationElement(34)) + (wvpMatrixCopyTwo[1][3] * camMatrix.getRotationElement(44));

		wvpMatrixCopyThree[2][0] = (wvpMatrixCopyTwo[2][0] * camMatrix.getRotationElement(11)) + (wvpMatrixCopyTwo[2][1] * camMatrix.getRotationElement(21)) + (wvpMatrixCopyTwo[2][2] * camMatrix.getRotationElement(31)) + (wvpMatrixCopyTwo[2][3] * camMatrix.getRotationElement(41));
		wvpMatrixCopyThree[2][1] = (wvpMatrixCopyTwo[2][0] * camMatrix.getRotationElement(12)) + (wvpMatrixCopyTwo[2][1] * camMatrix.getRotationElement(22)) + (wvpMatrixCopyTwo[2][2] * camMatrix.getRotationElement(32)) + (wvpMatrixCopyTwo[2][3] * camMatrix.getRotationElement(42));
		wvpMatrixCopyThree[2][2] = (wvpMatrixCopyTwo[2][0] * camMatrix.getRotationElement(13)) + (wvpMatrixCopyTwo[2][1] * camMatrix.getRotationElement(23)) + (wvpMatrixCopyTwo[2][2] * camMatrix.getRotationElement(33)) + (wvpMatrixCopyTwo[2][3] * camMatrix.getRotationElement(43));
		wvpMatrixCopyThree[2][3] = (wvpMatrixCopyTwo[2][0] * camMatrix.getRotationElement(14)) + (wvpMatrixCopyTwo[2][1] * camMatrix.getRotationElement(24)) + (wvpMatrixCopyTwo[2][2] * camMatrix.getRotationElement(34)) + (wvpMatrixCopyTwo[2][3] * camMatrix.getRotationElement(44));

		wvpMatrixCopyThree[3][0] = (wvpMatrixCopyTwo[3][0] * camMatrix.getRotationElement(11)) + (wvpMatrixCopyTwo[3][1] * camMatrix.getRotationElement(21)) + (wvpMatrixCopyTwo[3][2] * camMatrix.getRotationElement(31)) + (wvpMatrixCopyTwo[3][3] * camMatrix.getRotationElement(41));
		wvpMatrixCopyThree[3][1] = (wvpMatrixCopyTwo[3][0] * camMatrix.getRotationElement(12)) + (wvpMatrixCopyTwo[3][1] * camMatrix.getRotationElement(22)) + (wvpMatrixCopyTwo[3][2] * camMatrix.getRotationElement(32)) + (wvpMatrixCopyTwo[3][3] * camMatrix.getRotationElement(42));
		wvpMatrixCopyThree[3][2] = (wvpMatrixCopyTwo[3][0] * camMatrix.getRotationElement(13)) + (wvpMatrixCopyTwo[3][1] * camMatrix.getRotationElement(23)) + (wvpMatrixCopyTwo[3][2] * camMatrix.getRotationElement(33)) + (wvpMatrixCopyTwo[3][3] * camMatrix.getRotationElement(43));
		wvpMatrixCopyThree[3][3] = (wvpMatrixCopyTwo[3][0] * camMatrix.getRotationElement(14)) + (wvpMatrixCopyTwo[3][1] * camMatrix.getRotationElement(24)) + (wvpMatrixCopyTwo[3][2] * camMatrix.getRotationElement(34)) + (wvpMatrixCopyTwo[3][3] * camMatrix.getRotationElement(44));
		*/

		//Multiply result(wvpMatrixCopy) * perspProj
		wvpMatrix[0][0] = (wvpMatrixCopy[0][0] * perspProj.getPerspectiveProjMatrixElement(11)) + (wvpMatrixCopy[0][1] * perspProj.getPerspectiveProjMatrixElement(21)) + (wvpMatrixCopy[0][2] * perspProj.getPerspectiveProjMatrixElement(31)) + (wvpMatrixCopy[0][3] * perspProj.getPerspectiveProjMatrixElement(41));
		wvpMatrix[0][1] = (wvpMatrixCopy[0][0] * perspProj.getPerspectiveProjMatrixElement(12)) + (wvpMatrixCopy[0][1] * perspProj.getPerspectiveProjMatrixElement(22)) + (wvpMatrixCopy[0][2] * perspProj.getPerspectiveProjMatrixElement(32)) + (wvpMatrixCopy[0][3] * perspProj.getPerspectiveProjMatrixElement(42));
		wvpMatrix[0][2] = (wvpMatrixCopy[0][0] * perspProj.getPerspectiveProjMatrixElement(13)) + (wvpMatrixCopy[0][1] * perspProj.getPerspectiveProjMatrixElement(23)) + (wvpMatrixCopy[0][2] * perspProj.getPerspectiveProjMatrixElement(33)) + (wvpMatrixCopy[0][3] * perspProj.getPerspectiveProjMatrixElement(43));
		wvpMatrix[0][3] = (wvpMatrixCopy[0][0] * perspProj.getPerspectiveProjMatrixElement(14)) + (wvpMatrixCopy[0][1] * perspProj.getPerspectiveProjMatrixElement(24)) + (wvpMatrixCopy[0][2] * perspProj.getPerspectiveProjMatrixElement(34)) + (wvpMatrixCopy[0][3] * perspProj.getPerspectiveProjMatrixElement(44));
		
		wvpMatrix[1][0] = (wvpMatrixCopy[1][0] * perspProj.getPerspectiveProjMatrixElement(11)) + (wvpMatrixCopy[1][1] * perspProj.getPerspectiveProjMatrixElement(21)) + (wvpMatrixCopy[1][2] * perspProj.getPerspectiveProjMatrixElement(31)) + (wvpMatrixCopy[1][3] * perspProj.getPerspectiveProjMatrixElement(41));
		wvpMatrix[1][1] = (wvpMatrixCopy[1][0] * perspProj.getPerspectiveProjMatrixElement(12)) + (wvpMatrixCopy[1][1] * perspProj.getPerspectiveProjMatrixElement(22)) + (wvpMatrixCopy[1][2] * perspProj.getPerspectiveProjMatrixElement(32)) + (wvpMatrixCopy[1][3] * perspProj.getPerspectiveProjMatrixElement(42));
		wvpMatrix[1][2] = (wvpMatrixCopy[1][0] * perspProj.getPerspectiveProjMatrixElement(13)) + (wvpMatrixCopy[1][1] * perspProj.getPerspectiveProjMatrixElement(23)) + (wvpMatrixCopy[1][2] * perspProj.getPerspectiveProjMatrixElement(33)) + (wvpMatrixCopy[1][3] * perspProj.getPerspectiveProjMatrixElement(43));
		wvpMatrix[1][3] = (wvpMatrixCopy[1][0] * perspProj.getPerspectiveProjMatrixElement(14)) + (wvpMatrixCopy[1][1] * perspProj.getPerspectiveProjMatrixElement(24)) + (wvpMatrixCopy[1][2] * perspProj.getPerspectiveProjMatrixElement(34)) + (wvpMatrixCopy[1][3] * perspProj.getPerspectiveProjMatrixElement(44));

		wvpMatrix[2][0] = (wvpMatrixCopy[2][0] * perspProj.getPerspectiveProjMatrixElement(11)) + (wvpMatrixCopy[2][1] * perspProj.getPerspectiveProjMatrixElement(21)) + (wvpMatrixCopy[2][2] * perspProj.getPerspectiveProjMatrixElement(31)) + (wvpMatrixCopy[2][3] * perspProj.getPerspectiveProjMatrixElement(41));
		wvpMatrix[2][1] = (wvpMatrixCopy[2][0] * perspProj.getPerspectiveProjMatrixElement(12)) + (wvpMatrixCopy[2][1] * perspProj.getPerspectiveProjMatrixElement(22)) + (wvpMatrixCopy[2][2] * perspProj.getPerspectiveProjMatrixElement(32)) + (wvpMatrixCopy[2][3] * perspProj.getPerspectiveProjMatrixElement(42));
		wvpMatrix[2][2] = (wvpMatrixCopy[2][0] * perspProj.getPerspectiveProjMatrixElement(13)) + (wvpMatrixCopy[2][1] * perspProj.getPerspectiveProjMatrixElement(23)) + (wvpMatrixCopy[2][2] * perspProj.getPerspectiveProjMatrixElement(33)) + (wvpMatrixCopy[2][3] * perspProj.getPerspectiveProjMatrixElement(43));
		wvpMatrix[2][3] = (wvpMatrixCopy[2][0] * perspProj.getPerspectiveProjMatrixElement(14)) + (wvpMatrixCopy[2][1] * perspProj.getPerspectiveProjMatrixElement(24)) + (wvpMatrixCopy[2][2] * perspProj.getPerspectiveProjMatrixElement(34)) + (wvpMatrixCopy[2][3] * perspProj.getPerspectiveProjMatrixElement(44));
		
		wvpMatrix[3][0] = (wvpMatrixCopy[3][0] * perspProj.getPerspectiveProjMatrixElement(11)) + (wvpMatrixCopy[3][1] * perspProj.getPerspectiveProjMatrixElement(21)) + (wvpMatrixCopy[3][2] * perspProj.getPerspectiveProjMatrixElement(31)) + (wvpMatrixCopy[3][3] * perspProj.getPerspectiveProjMatrixElement(41));
		wvpMatrix[3][1] = (wvpMatrixCopy[3][0] * perspProj.getPerspectiveProjMatrixElement(12)) + (wvpMatrixCopy[3][1] * perspProj.getPerspectiveProjMatrixElement(22)) + (wvpMatrixCopy[3][2] * perspProj.getPerspectiveProjMatrixElement(32)) + (wvpMatrixCopy[3][3] * perspProj.getPerspectiveProjMatrixElement(42));
		wvpMatrix[3][2] = (wvpMatrixCopy[3][0] * perspProj.getPerspectiveProjMatrixElement(13)) + (wvpMatrixCopy[3][1] * perspProj.getPerspectiveProjMatrixElement(23)) + (wvpMatrixCopy[3][2] * perspProj.getPerspectiveProjMatrixElement(33)) + (wvpMatrixCopy[3][3] * perspProj.getPerspectiveProjMatrixElement(43));
		wvpMatrix[3][3] = (wvpMatrixCopy[3][0] * perspProj.getPerspectiveProjMatrixElement(14)) + (wvpMatrixCopy[3][1] * perspProj.getPerspectiveProjMatrixElement(24)) + (wvpMatrixCopy[3][2] * perspProj.getPerspectiveProjMatrixElement(34)) + (wvpMatrixCopy[3][3] * perspProj.getPerspectiveProjMatrixElement(44));
	}

	void multiplyVertexByWVP(Object &object){
		for (unsigned int atx = 0; atx < object.objVerticies.size(); atx++){
			double x = object.objVerticies.at(atx).getX();
			double y = object.objVerticies.at(atx).getY();
			double z = object.objVerticies.at(atx).getZ();
			double w = object.objVerticies.at(atx).getW();

			object.objVerticies.at(atx).setXPrime((x * wvpMatrix[0][0]) + (y * wvpMatrix[1][0]) + (z * wvpMatrix[2][0]) + (w * wvpMatrix[3][0]));
			object.objVerticies.at(atx).setYPrime((x * wvpMatrix[0][1]) + (y * wvpMatrix[1][1]) + (z * wvpMatrix[2][1]) + (w * wvpMatrix[3][1]));
			object.objVerticies.at(atx).setZPrime((x * wvpMatrix[0][2]) + (y * wvpMatrix[1][2]) + (z * wvpMatrix[2][2]) + (w * wvpMatrix[3][2]));
			object.objVerticies.at(atx).setWPrime((x * wvpMatrix[0][3]) + (y * wvpMatrix[1][3]) + (z * wvpMatrix[2][3]) + (w * wvpMatrix[3][3]));
			//zperform the perspective, or homogeneous divide!
			object.objVerticies.at(atx).setXPrime(object.objVerticies.at(atx).getXPrime() / object.objVerticies.at(atx).getWPrime());
			object.objVerticies.at(atx).setYPrime(object.objVerticies.at(atx).getYPrime() / object.objVerticies.at(atx).getWPrime());
			object.objVerticies.at(atx).setZPrime(object.objVerticies.at(atx).getZPrime() / object.objVerticies.at(atx).getWPrime());
		}
	}

	void displayWVPToConsole()
	{
		cout << "WORLD VIEW PROJECTION MATRIX" << endl;
		for (int i = 0; i < 4; i++)
		{
			cout << "[";
			for (int j = 0; j < 4; j++)
			{
				cout << wvpMatrix[i][j] << ", ";
			}
			cout << "]" << endl;
		}
	}

	void displayMatrixCopyToConsole()
	{
		cout << "WORLD VIEW PROJECTION MATRIX" << endl;
		for (int i = 0; i < 4; i++)
		{
			cout << "[";
			for (int j = 0; j < 4; j++)
			{
				cout << wvpMatrixCopy[i][j] << ", ";
			}
			cout << "]" << endl;
		}
	}

	void displayMatrixCopyTwoToConsole()
	{
		cout << "WORLD VIEW PROJECTION MATRIX" << endl;
		for (int i = 0; i < 4; i++)
		{
			cout << "[";
			for (int j = 0; j < 4; j++)
			{
				cout << wvpMatrixCopyTwo[i][j] << ", ";
			}
			cout << "]" << endl;
		}
	}
};


#endif