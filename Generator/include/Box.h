#pragma once
#include "Triangulo.h"
#include "Ponto3D.h"
#include "Model.h"
#include <vector>
#include <string>
#include <tuple>
#include <map>

class Box{
public:
    Box();
    Box(int widthG,int depthG,int heightG,int nDivisionsG);
    Model* generate();
private:
    int width,depth,height,nDivisions;
    std::vector<Triangulo*> faces;
    std::map<std::tuple<int,int,int>,Ponto3D*> points;
    void addYLayer(int iterations,bool top);
    void addXLayer(int iterations,bool top);
    void addZLayer(int iterations,bool top);
    void addSquare(bool top,Ponto3D * topRight,Ponto3D * topLeft,Ponto3D * bellowLeft,Ponto3D * bellowRight);
};