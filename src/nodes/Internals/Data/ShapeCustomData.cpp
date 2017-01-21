#include "StdAfx.h"
#include "ShapeCustomData.h"

ShapeCustomData::ShapeCustomData(void)
{
	this->MarkedForDeletion= false;
	this->MarkedForUpdate = false;
	this->LifeTime = 0;
}

ShapeCustomData::~ShapeCustomData(void)
{
	//delete Custom;
}
