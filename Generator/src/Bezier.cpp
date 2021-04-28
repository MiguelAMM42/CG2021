#include "../include/Bezier.h"
#include "../../Utils/Point3D.h"
#include <vector>
#include <string>
#define _USE_MATH_DEFINES
#include <math.h>
#include <fstream>
#include <sstream>
#include "iostream"

void printMatrix(std::vector<std::vector<Point3D>> auxMatrix) {
	for (int i=0;i<auxMatrix.size();i++) {
		for (int j=0;j<auxMatrix[0].size();j++) {
			std::cout<<" " << auxMatrix[i][j].z;
		}
		std::cout<<"\n";
	}
	std::cout<<"rows: "<< auxMatrix.size() << " cols: " << auxMatrix[0].size()<<"\n";
}

Bezier::Bezier(std::string patch_fileG,int tesselation_levelG) {
	patch_file=patch_fileG;
	vertical_tesselation=tesselation_levelG;
	horizontal_tesselation=tesselation_levelG;
	vertical_tesselation_inc=1.0/vertical_tesselation;
	horizontal_tesselation_inc=1.0/horizontal_tesselation;
	Point3D p1 = Point3D(1,1,1);
	Point3D p_1 = Point3D(p1*-1);
	Point3D p3 = Point3D(p1*3);
	Point3D p_3 = Point3D(p1*-3);
	Point3D p_6 = Point3D(p1*-6);
	Point3D p0 = Point3D(p1*0);
	Point3D p13 = Point3D(p1*(1.0/3.0));
	Point3D p23 = Point3D(p1*(2.0/3.0));
	std::vector<Point3D> row0,row1,row2,row3;
	row0.push_back(p_1);row0.push_back(p3);row0.push_back(p_3);row0.push_back(p1);
	row1.push_back(p3);row1.push_back(p_6);row1.push_back(p3);row1.push_back(p0);
	row2.push_back(p_3);row2.push_back(p3);row2.push_back(p0);row2.push_back(p0);
	row3.push_back(p1);row3.push_back(p0);row3.push_back(p0);row3.push_back(p0);
	bezier_matrix.push_back(row0);
	bezier_matrix.push_back(row1);
	bezier_matrix.push_back(row2);
	bezier_matrix.push_back(row3);
}

std::vector<std::vector<Point3D>> Bezier::multiplyMatrix(std::vector<std::vector<Point3D>> m1,std::vector<std::vector<Point3D>> m2) {
	std::vector<std::vector<Point3D>> returnMatrix;
	for(int i=0;i<m1.size();i++) {    
		std::vector<Point3D> row;
		for(int j=0;j<m2[0].size();j++) {
			float x;
			float y;
			float z;   
			for(int k=0;k<m1[0].size();k++) {    
				x+=m1[i][k].x*m2[k][j].x; 
				y+=m1[i][k].y*m2[k][j].y;   
				z+=m1[i][k].z*m2[k][j].z;      
			}
			row.push_back(Point3D(x,y,z));    
		}
		returnMatrix.push_back(row);    
	}
	return returnMatrix;
}


void Bezier::read_patches() {
    int numPoints;
	float x,y,z;
	std::map<int,std::vector<int>> index_patches;
	std::vector<Point3D> patch_points;
    
    std::ifstream fp("../patches/" + patch_file);
	std::string line;
	std::getline(fp,line);
	std::istringstream iss(line);
	iss >> numPatches;

	std::string delimiter = ", ";
	for (int i=0;i<numPatches;i++) {
		std::getline(fp,line);

		size_t pos = 0;
		std::string token;

		for (int j=0;j<16;j++) {
			pos = line.find(delimiter);
		    token = line.substr(0, pos);
		    index_patches[i].push_back(std::stoi(token));
			line.erase(0, pos + delimiter.length());
		}
    }
	std::getline(fp,line);
	std::istringstream points(line);
	points >> numPoints;
	std::string comma;
	for (int i=0;i<numPoints;i++) {
		std::getline(fp,line);
		std::istringstream p(line);
		p >> x >> comma >> y >> comma >> z;
		Point3D point = Point3D(x,y,z);
		patch_points.push_back(point);
    }

	for (int i=0;i<numPatches;i++) {
		
		Patch patch = Patch();
		for (int j=0;j<4;j++) {
			std::vector<Point3D> row;
			for (int k=0;k<4;k++) {
				int index=index_patches[i][j*4+k];
				row.push_back(patch_points[index]);
			}
			patch.points.push_back(row);
		}
		patches.push_back(patch);
	}
}

Point3D Bezier::calculatePoint(std::vector<std::vector<Point3D>> preCalculatedMatrix,int vertical_level,int horizontal_level) {
	float u = vertical_level*vertical_tesselation_inc;
	float v = horizontal_level*horizontal_tesselation_inc;
	std::vector<std::vector<Point3D>> u_matrix,v_matrix;
	std::vector<Point3D> rowu;
	for (int i=3;i>=0;i--) {
		std::vector<Point3D> rowv;
		float uValue = std::pow(u,i);
		float vValue = std::pow(v,i);
		Point3D uPoint = Point3D(uValue,uValue,uValue);
		Point3D vPoint = Point3D(vValue,vValue,vValue);
		rowu.push_back(uPoint);
		rowv.push_back(vPoint);
		v_matrix.push_back(rowv);
	}
	u_matrix.push_back(rowu);
	auto auxMatrix = multiplyMatrix(u_matrix,preCalculatedMatrix);
	auto returnMatrix = multiplyMatrix(auxMatrix,v_matrix);
	return returnMatrix[0][0];
}

void Bezier::calculatePoints(std::vector<std::vector<Point3D>> preCalculatedMatrix,int& indexPoint,std::vector<Point3D>& vertixes,std::vector<Triangle>& triangs) {
	for (int i=0;i<=vertical_tesselation;i++){
		for (int j=0;j<=horizontal_tesselation;j++) {
			auto point = calculatePoint(preCalculatedMatrix,i,j);
			point.index=indexPoint;
			vertixes.push_back(point);
			if (i && j) {
			    triangs.push_back(Triangle(indexPoint-horizontal_tesselation-2,indexPoint-horizontal_tesselation-1,indexPoint));
			    triangs.push_back(Triangle(indexPoint-1,indexPoint-horizontal_tesselation-2,indexPoint));
            }
            indexPoint++;
        }
	}
}

std::shared_ptr<Model> Bezier::generate() {
	read_patches();

	int indexPoint=0;
	std::vector<Point3D> vertixes;
	std::vector<Triangle> triangs;
	std::cout<<"im done reading patches";
	for (int i=0;i<numPatches;i++) {
		Patch p = patches[i];

		auto auxMatrix = multiplyMatrix(bezier_matrix,p.points);
		auto preCalculatedMatrix = multiplyMatrix(auxMatrix,bezier_matrix);
		calculatePoints(preCalculatedMatrix,indexPoint,vertixes,triangs);
	}
	std::cout<<"im done makig points";
	return std::make_shared<Model>(vertixes,triangs);
}