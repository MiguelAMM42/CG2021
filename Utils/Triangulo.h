#pragma once
#include "Ponto3D.h"

class Triangulo{
public:
    Ponto3D *ponto1,*ponto2,*ponto3;
    float redValue, greenValue, blueValue;
    Triangulo();
    Triangulo(Ponto3D *ponto11,Ponto3D *ponto22,Ponto3D *ponto33);
	Triangulo(Ponto3D *ponto11,Ponto3D *ponto22,Ponto3D *ponto33,float redV,float greenV,float blueV);
	void printCoords();
};