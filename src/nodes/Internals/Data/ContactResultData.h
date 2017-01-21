#pragma once

class ContactResultData
{
public:
	ContactResultData(void);
	b2Shape* shape1;		///< the first shape
	b2Shape* shape2;		///< the second shape
	b2Vec2 position;		///< position in world coordinates
	b2Vec2 normal;			///< points from shape1 to shape2
	float32 normalImpulse;	///< the normal impulse applied to body2
	float32 tangentImpulse;	///< the tangent impulse applied to body2
	b2ContactID id;			///< the contact id identifies the features in contact
public:
	~ContactResultData(void);
};
