
#include "ObjectManager.h"


void ObjectManager::ProcessBodies(b2World* world,std::vector<int>* deleted, bool force, double dt)
{
	deleted->clear();
	b2Body* node = world->GetBodyList();
	while (node)
	{
		b2Body* b = node;
		node = node->GetNext();
		
		bool destroyed = false;

		if (b != world->GetGroundBody()) 
		{
			BodyCustomData* bdata = (BodyCustomData*)b->GetUserData();
			bdata->LifeTime = bdata->LifeTime+ dt;


			
			if (force)
			{
				ProcessShapes(b,true,dt);
				delete bdata;
				world->DestroyBody(b);
				destroyed = true;
			}
			else
			{			
				//Mark the body as non created;
				bdata->Created = false;

				bool dodelete = false;
				if (bdata->HasTTL)
				{
					bdata->TTL -= dt;
					if (bdata->TTL <= 0.0)
					{
						dodelete = true;
					}
				}


				if (bdata->MarkedForDeletion || dodelete) 
				{
					ProcessShapes(b,true,dt);
					
					deleted->push_back(bdata->Id);
					delete bdata;
					world->DestroyBody(b);
					destroyed = true;
				} 
			}
		}

		if (!destroyed)
		{
			ProcessShapes(b,false,dt);
		}
	}
}

void ObjectManager::ProcessShapes(b2Body* b, bool force,double dt)
{
	b2Shape* snode = b->GetShapeList();

	bool change = false;
	while (snode)
	{
		b2Shape* s = snode;
		snode = snode->GetNext();

		if (s->GetType() == e_circleShape || s->GetType() == e_polygonShape) 
		{
			ShapeCustomData* sdata = (ShapeCustomData*)s->GetUserData();
			sdata->LifeTime = sdata->LifeTime+ dt;
			bool del = false;

			if (force)
			{
				delete sdata;
				b->DestroyShape(s);
				del = true;
			}
			else
			{
				if (sdata->MarkedForDeletion)
				{
					delete sdata;
					b->DestroyShape(s);
					change = true;
					del = true;
				}

				
				if (sdata->MarkedForUpdate)
				{
					b->DestroyShape(s);
					b2Shape* newshape = b->CreateShape(sdata->NewShape);
					newshape->SetUserData(sdata);
					sdata->MarkedForUpdate = false;
					delete sdata->NewShape;
					change = true;
					del = true;
				}
			}
		}
	}

	if (change && b->IsDynamic())
	{
		b->SetMassFromShapes();
	}
}

void ObjectManager::ProcessJoints(b2World* world, bool force,double dt)
{
	b2Joint* nodej = world->GetJointList();
	while (nodej)
	{
		b2Joint* j = nodej;
		nodej = nodej->GetNext();

		if (j->GetUserData() != nullptr)
		{
			JointCustomData* jdata = (JointCustomData*)j->GetUserData();
			jdata->LifeTime = jdata->LifeTime+ dt;
				
			if (jdata->MarkedForDeletion || force) 
			{
				delete jdata;
				world->DestroyJoint(j);	
			}
		}
	}
}
