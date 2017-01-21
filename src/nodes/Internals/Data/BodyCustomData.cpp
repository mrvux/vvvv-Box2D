
#include "BodyCustomData.h"

BodyCustomData::BodyCustomData(void)
{
	this->MarkedForDeletion = false;
	this->LifeTime = 0;
}

BodyCustomData::~BodyCustomData(void)
{
	//delete Custom;
}
