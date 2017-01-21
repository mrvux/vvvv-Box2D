#pragma once

#include <vector>
#include "Box2D.h"

#include "../Data/ShapeCustomData.h"
#include "../Data/BodyCustomData.h"
#include "../Data/JointCustomData.h"

class ObjectManager
{
public:
	static void ProcessBodies(b2World* world,std::vector<int>* deleted,bool force,double dt);
	static void ProcessShapes(b2Body* body,bool force,double dt);
	static void ProcessJoints(b2World* world,bool force,double dt);

};
