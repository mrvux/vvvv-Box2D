#pragma once

#include <string.h>

class BodyCustomData
{
private:

public:
	BodyCustomData(void);
	~BodyCustomData(void);
	int Id;
	bool MarkedForDeletion;
	double LifeTime; //In frames
	char* Custom;
	bool Created;
	bool Persisted;
	double TTL;
	bool HasTTL;
};
