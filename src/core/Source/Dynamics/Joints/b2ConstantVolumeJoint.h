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

#ifndef B2_CONSTANTVOLUME_JOINT_H
#define B2_CONSTANTVOLUME_JOINT_H

#include "b2Joint.h"
#include "b2DistanceJoint.h"
#include <vector>

struct b2ConstantVolumeJointDef : public b2JointDef
{
	b2ConstantVolumeJointDef()
	{
		type = e_distanceJoint;
	}

	std::vector<b2Body*> bodies;
	float frequencyHz;
	float dampingRatio;

};


class b2ConstantVolumeJoint : public b2Joint
{
public:

	b2Vec2 GetAnchor1() const;
	b2Vec2 GetAnchor2() const;

	b2Vec2 GetReactionForce(float32 inv_dt) const;
	float32 GetReactionTorque(float32 inv_dt) const;

	//--------------- Internals Below -------------------

	b2ConstantVolumeJoint(const b2ConstantVolumeJointDef* data);

	void InitVelocityConstraints(const b2TimeStep& step);
	void SolveVelocityConstraints(const b2TimeStep& step);
	bool SolvePositionConstraints(float32 baumgarte);

	void inflate(float factor) { targetVolume *= factor; }

private:
	b2World* world;
	std::vector<b2Body*> bodies;

	std::vector<float> targetLengths;
	float targetVolume;

	std::vector<b2Vec2*> normals;

	float m_impulse;

	b2DistanceJoint* distanceJoints;

};

#endif
