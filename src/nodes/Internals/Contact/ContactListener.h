#pragma once

#include "Box2D.h"

#include <vector>

#include "../Data/ShapeCustomData.h"
#include "../Data/ContactResultData.h"


struct ContactReportId
{
	int Shape1;
	int Shape2;
	uint32 FeaturesId;
	double IsNew;
};

public class ContactListener : public b2ContactListener
{
	private:
		std::vector<ContactReportId*>* m_reports;
		std::vector<ContactResultData*>* m_results;
	public:
		ContactListener(std::vector<ContactResultData*>* results,std::vector<ContactReportId*>* reports) 
		{
			this->m_results = results;
			this->m_reports = reports;
		}

		void ResetResults()
		{
			//Clear reports (whatever happens)
			for (unsigned int i = 0; i < this->m_reports->size();i++)
			{
				ContactReportId* cp = this->m_reports->at(i);
				delete cp;
			}
			this->m_reports->clear();

			for (unsigned int i = 0; i < this->m_results->size();i++)
			{
				ContactResultData* cd = this->m_results->at(i);
				delete cd;
			}
			this->m_results->clear();
		}
				
		~ContactListener(void) 
		{

		}
							
		void Add(const b2ContactPoint* point)
		{	
			
			ContactReportId* rep = new ContactReportId();
			rep->FeaturesId = point->id.key;
			rep->IsNew = 1;
			ShapeCustomData* s1 = (ShapeCustomData*)point->shape1->GetUserData();
			rep->Shape1 = s1->Id;
			ShapeCustomData* s2 = (ShapeCustomData*)point->shape2->GetUserData();
			rep->Shape2 = s2->Id;
			m_reports->push_back(rep);
		}
			    
		void Persist(const b2ContactPoint* point)
		{		
			ContactReportId* rep = new ContactReportId();
			rep->FeaturesId = point->id.key;
			rep->IsNew = 0;
			ShapeCustomData* s1 = (ShapeCustomData*)point->shape1->GetUserData();
			rep->Shape1 = s1->Id;
			ShapeCustomData* s2 = (ShapeCustomData*)point->shape2->GetUserData();
			rep->Shape2 = s2->Id;
			m_reports->push_back(rep);

		}
	    
		void Remove(const b2ContactPoint* point)
		{

		}
	    
		void Result(const b2ContactResult* point)
		{
			
			ContactResultData* cpoint;
			cpoint = new ContactResultData();
			this->m_results->push_back(cpoint);

			cpoint->id = point->id;
			cpoint->shape1 = point->shape1;
			cpoint->shape2 = point->shape2;
			cpoint->position.x = point->position.x;
			cpoint->position.y = point->position.y;
			cpoint->normal.x = point->normal.x;
			cpoint->normal.y = point->normal.y;
			cpoint->normalImpulse = point->normalImpulse;
			cpoint->tangentImpulse = point->tangentImpulse;
		}
};

