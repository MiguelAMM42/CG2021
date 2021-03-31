#pragma once
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <vector>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include "Transformations/Transform.h"
#include "Transformations/Translation.h"
#include "Transformations/Rotation.h"
#include "Transformations/Scale.h"
#include "Figure.h"

class Group {
public:
    std::vector<Transform*> transformations;
    std::unordered_map<std::string,Figure*> models;
    std::vector<Group*> nestedGroups;

    Group()=default;

    void addTransform(Transform * t);

    void addFile(std::string filename,float red,float green,float blue);

    void addGroup(Group * group);

    std::unordered_set<std::string> getModels();
};