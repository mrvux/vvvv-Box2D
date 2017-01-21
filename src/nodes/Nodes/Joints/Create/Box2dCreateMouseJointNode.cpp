
#include "Box2dCreateMouseJointNode.h"

namespace VVVV 
{
	namespace Nodes 
	{
		Box2dCreateMouseJointNode::Box2dCreateMouseJointNode(void)
		{
		}

		void Box2dCreateMouseJointNode::OnPluginHostSet() 
		{
			this->FHost->CreateValueInput("Target", 2, nullptr , v4::TSliceMode::Dynamic, v4::TPinVisibility::True, this->vInTarget);
			this->vInTarget->SetSubType2D(System::Double::MinValue, System::Double::MaxValue, 0.01, 0,0, false,false, false);

			this->FHost->CreateValueInput("Frequency",1,nullptr,v4::TSliceMode::Dynamic,v4::TPinVisibility::True,this->vInFrequency);
			this->vInFrequency->SetSubType(System::Double::MinValue,System::Double::MaxValue,0.01,0.0,false,false,false);

			this->FHost->CreateValueInput("Damping Ratio",1,nullptr,v4::TSliceMode::Dynamic,v4::TPinVisibility::True,this->vInDampingRatio);
			this->vInDampingRatio->SetSubType(0,1,0.01,0.0,false,false,false);

			this->FHost->CreateValueInput("Prevent Double", 1, nullptr, v4::TSliceMode::Dynamic, v4::TPinVisibility::True, this->vInPreventDouble);
			this->vInPreventDouble->SetSubType(0, 1, 1, 0, false,true, false);

		}

		void Box2dCreateMouseJointNode::Evaluate(int SpreadMax)
		{
			this->vInBody2->PinIsChanged;

			if (this->vInBody2->IsConnected && this->vInBody2->SliceCount > 0 && this->vInWorld->IsConnected) 
			{
				if (this->mWorld->GetIsValid()) 
				{
					for (int i = 0; i < SpreadMax; i++) 
					{
						double dblcreate;
						this->vInDoCreate->GetValue(i,dblcreate);
						if (dblcreate >= 0.5) 
						{
							double freq,dr,cc,tx,ty,dblprevent;
							int realslice1,realslice2;
							System::String^ cust;

							this->vInPreventDouble->GetValue(i, dblprevent);
							this->vInBody2->GetUpsreamSlice(i % this->vInBody2->SliceCount,realslice2);
							b2Body* body2 = this->m_body2->GetSlice(realslice2);

							//b2Joint* j = ;

							if (body2->GetJointList() && dblprevent > 0.5)
							{

							}
							else
							{
								this->vInFrequency->GetValue(i, freq);
								this->vInDampingRatio->GetValue(i,dr);
								this->vInCustom->GetString(i,cust);

								this->vInTarget->GetValue2D(i,tx,ty);

								b2MouseJointDef md;
								md.body1 = this->mWorld->GetWorld()->GetGroundBody();
								md.body2 = body2;
								md.target = b2Vec2(tx,ty); //body2->GetPosition();
								//md.
								md.frequencyHz = freq;
								md.dampingRatio = dr;
								md.maxForce = 1000.0f * body2->GetMass();
								
								JointCustomData* jdata = new JointCustomData();
								jdata->Id = this->mWorld->GetNewJointId();
								jdata->Custom = (char*)(void*)System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(cust);

								b2Joint* j = this->mWorld->GetWorld()->CreateJoint(&md);
								j->SetUserData(jdata);
								//b2MouseJoint* bm = (b2MouseJoint*)j;
								//bm->SetTarget(b2Vec2(Convert::ToSingle(tx),Convert::ToSingle(ty)));
							}
						}
					}
				}
			}

		}
	}
}
