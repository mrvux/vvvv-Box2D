#include "StdAfx.h"
#include "JointCustomData.h"

JointCustomData::JointCustomData(void)
{
	this->MarkedForDeletion = false;
	this->LifeTime = 0;
}

JointCustomData::~JointCustomData(void)
{
	//delete Custom;
}