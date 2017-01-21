/*
* Copyright (c) 2006-2007 Erin Catto http://www.gphysics.com
*
* This software is provided 'as-is', without any express or implied
* warranty.  In no event will the authors be held liable for any damages
* arising from the use of this software.
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely, subject to the following restrictions:
* 1. The origin of this software must not be misrepresented; you must not
* claim that you wrote the original software. If you use this software
* in a product, an acknowledgment in the product documentation would be
* appreciated but is not required.
* 2. Altered source versions must be plainly marked as such, and must not be
* misrepresented as being the original software.
* 3. This notice may not be removed or altered from any source distribution.
*/

#include "b2ConstantVolumeJoint.h"
#include "../b2Body.h"
#include "../b2World.h"



b2ConstantVolumeJoint::b2ConstantVolumeJoint(const b2ConstantVolumeJointDef* def)
: b2Joint(def)
{
	this->world = def->bodies[0]->GetWorld();

	for (int i = 0; i < (int)def->bodies.size(); i++)
	{
		this->bodies.push_back(def->bodies[i]);

		this->normals.push_back(new b2Vec2());
	}


	this->m_body1 = bodies[0];
	this->m_body2 = bodies[1];
	this->m_collideConnected = false;
}

	private float getArea() {
		float area = 0.0f;
		// i'm glad i changed these all to member access
		area += bodies[bodies.length-1].getMemberWorldCenter().x * bodies[0].getMemberWorldCenter().y -
		bodies[0].getMemberWorldCenter().x * bodies[bodies.length-1].getMemberWorldCenter().y;
		for (int i=0; i<bodies.length-1; ++i){
			area += bodies[i].getMemberWorldCenter().x * bodies[i+1].getMemberWorldCenter().y -
			bodies[i+1].getMemberWorldCenter().x * bodies[i].getMemberWorldCenter().y;
		}
		area *= .5f;
		return area;
	}

void b2ConstantVolumeJoint::InitVelocityConstraints(const b2TimeStep& step)
{

}

void b2ConstantVolumeJoint::SolveVelocityConstraints(const b2TimeStep& step)
{

}

bool b2ConstantVolumeJoint::SolvePositionConstraints(float32 baumgarte)
{
	return false;
}

b2Vec2 b2ConstantVolumeJoint::GetAnchor1() const
{
	return b2Vec2(0.0f,0.0f);
}

b2Vec2 b2ConstantVolumeJoint::GetAnchor2() const
{
	return b2Vec2(0.0f,0.0f);
}

b2Vec2 b2ConstantVolumeJoint::GetReactionForce(float32 inv_dt) const
{
	return b2Vec2(0.0f,0.0f);
}

float32 b2ConstantVolumeJoint::GetReactionTorque(float32 inv_dt) const
{
	return 0.0f;
}
