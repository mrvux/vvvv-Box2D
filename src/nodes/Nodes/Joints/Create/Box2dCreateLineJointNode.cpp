
#include "Box2dCreateLineJointNode.h"

namespace VVVV 
{
	namespace Nodes 
	{
		Box2dCreateLineJointNode::Box2dCreateLineJointNode(void)
		{
		}

		void Box2dCreateLineJointNode::OnPluginHostSet() 
		{
			this->FHost->CreateValueInput("Position",2,nullptr,v4::TSliceMode::Dynamic,v4::TPinVisibility::True,this->vInPosition);
			this->vInPosition->SetSubType2D(System::Double::MinValue,System::Double::MaxValue,0.01,0.0,0.0,false,false,false);

			this->FHost->CreateValueInput("Local Axis",2,nullptr,v4::TSliceMode::Dynamic,v4::TPinVisibility::True,this->vInLocalAxis);
			this->vInLocalAxis->SetSubType2D(System::Double::MinValue,System::Double::MaxValue,0.01,0.0,0.0,false,false,false);

			this->FHost->CreateValueInput("Enable Limit",1,nullptr,v4::TSliceMode::Dynamic,v4::TPinVisibility::True,this->vInEnableLimit);
			this->vInEnableLimit->SetSubType(System::Double::MinValue,System::Double::MaxValue,1.0,0.0,false,true,false);

			this->FHost->CreateValueInput("Lower Translation",1,nullptr,v4::TSliceMode::Dynamic,v4::TPinVisibility::True,this->vInLowTranslation);
			this->vInLowTranslation->SetSubType(System::Double::MinValue,System::Double::MaxValue,0.01,1.0,false,false,false);

			this->FHost->CreateValueInput("Upper Translation",1,nullptr,v4::TSliceMode::Dynamic,v4::TPinVisibility::True,this->vInUpTranslation);
			this->vInUpTranslation->SetSubType(System::Double::MinValue,System::Double::MaxValue,0.01,1.0,false,false,false);

			this->FHost->CreateValueInput("Max Motor Force",1,nullptr,v4::TSliceMode::Dynamic,v4::TPinVisibility::True,this->vInMaxMotorForce);
			this->vInMaxMotorForce->SetSubType(System::Double::MinValue,System::Double::MaxValue,0.01,0.0,false,false,false);

			this->FHost->CreateValueInput("Motor Speed",1,nullptr,v4::TSliceMode::Dynamic,v4::TPinVisibility::True,this->vInMotorSpeed);
			this->vInMotorSpeed->SetSubType(System::Double::MinValue,System::Double::MaxValue,0.01,0.0,false,false,false);

			this->FHost->CreateValueInput("Enable Motor",1,nullptr,v4::TSliceMode::Dynamic,v4::TPinVisibility::True,this->vInEnableMotor);
			this->vInEnableMotor->SetSubType(System::Double::MinValue,System::Double::MaxValue,1.0,0.0,false,true,false);
		}

		void Box2dCreateLineJointNode::Evaluate(int SpreadMax)
		{
			this->vInBody1->PinIsChanged;
			this->vInBody2->PinIsChanged;

			bool bcnn = true;
			bcnn = (this->vInBody1->IsConnected) || (this->vInBody1->SliceCount > 0);

			std::vector<int> ids;

			if (this->vInBody2->IsConnected && bcnn && this->vInBody2->SliceCount > 0 && this->vInWorld->IsConnected) 
			{
				if (this->mWorld->GetIsValid()) 
				{
					for (int i = 0; i < SpreadMax; i++) 
					{
						double dblcreate;
						this->vInDoCreate->GetValue(i,dblcreate);
						if (dblcreate >= 0.5) 
						{
							double px,py,cc,ax,ay;
							double enlimit,enmotor,motorf,motspeed,lt,ut;
							int realslice1,realslice2;
							System::String^ cust;

							b2Body* body1;

							if (this->isbody) 
							{
								this->vInBody1->GetUpsreamSlice(i % this->vInBody1->SliceCount,realslice1);
								body1 = this->m_body1->GetSlice(realslice1);
							} 
							else
							{
								body1 = this->m_ground1->GetGround();
							}

							this->vInBody2->GetUpsreamSlice(i % this->vInBody2->SliceCount,realslice2);
							b2Body* body2 = this->m_body2->GetSlice(realslice2);
						
							this->vInPosition->GetValue2D(i,px,py);
							this->vInLocalAxis->GetValue2D(i,ax,ay);
							this->vInCollideConnected->GetValue(i, cc);
							this->vInEnableLimit->GetValue(i, enlimit);
							this->vInEnableMotor->GetValue(i, enmotor);
							this->vInMaxMotorForce->GetValue(i, motorf);
							this->vInMotorSpeed->GetValue(i, motspeed);
							this->vInUpTranslation->GetValue(i, ut);
							this->vInLowTranslation->GetValue(i, lt);
							this->vInCustom->GetString(i,cust);

							b2LineJointDef jointDef;
							jointDef.Initialize(body1, body2, b2Vec2(px,py),b2Vec2(ax,ay));
							jointDef.collideConnected= cc >= 0.5;
							jointDef.enableLimit = enlimit >= 0.5;
							jointDef.maxMotorForce = System::Convert::ToSingle(motorf);
							jointDef.motorSpeed = System::Convert::ToSingle(motspeed);
							jointDef.enableMotor = enmotor >= 0.5;
							jointDef.lowerTranslation = System::Convert::ToSingle(lt);
							jointDef.upperTranslation = System::Convert::ToSingle(ut);
							

							JointCustomData* jdata = new JointCustomData();
							jdata->Id = this->mWorld->GetNewJointId();
							jdata->Custom = (char*)(void*)System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(cust);

							b2Joint* j = this->mWorld->GetWorld()->CreateJoint(&jointDef);
							j->SetUserData(jdata);

							ids.push_back(jdata->Id);
						}
					}
				}
			}

		}
	}
}


