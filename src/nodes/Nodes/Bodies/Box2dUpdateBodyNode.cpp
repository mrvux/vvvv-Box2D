
#include "Box2dUpdateBodyNode.h"

#include "../../Internals/Data/BodyCustomData.h"

namespace VVVV 
{
	namespace Nodes 
	{
		Box2dUpdateBodyNode::Box2dUpdateBodyNode(void)
		{
		}

		void Box2dUpdateBodyNode::SetPluginHost(v4::IPluginHost^ Host) 
		{
			this->FHost = Host;

			this->FHost->CreateNodeInput("Bodies",v4::TSliceMode::Dynamic,v4::TPinVisibility::True,this->vInBodies);
			this->vInBodies->SetSubType(VVVV::Utils::ArrayUtils::SingleGuidArray(v4b2d::BodyDataType::GUID), v4b2d::BodyDataType::FriendlyName);

			this->FHost->CreateValueInput("Position",2,nullptr,v4::TSliceMode::Dynamic,v4::TPinVisibility::True,this->vInPosition);
			this->vInPosition->SetSubType2D(System::Double::MinValue,System::Double::MaxValue,0.01,0.0,0.0,false,false,false);

			this->FHost->CreateValueInput("Set Position",1,nullptr,v4::TSliceMode::Dynamic,v4::TPinVisibility::True,this->vInSetPosition);
			this->vInSetPosition->SetSubType(0,1,1.0,0.0,true,false,false);	

			this->FHost->CreateValueInput("Angle",1,nullptr,v4::TSliceMode::Dynamic,v4::TPinVisibility::True,this->vInAngle);
			this->vInAngle->SetSubType(System::Double::MinValue,System::Double::MaxValue,0.01,0.0,false,false,false);

			this->FHost->CreateValueInput("Set Angle",1,nullptr,v4::TSliceMode::Dynamic,v4::TPinVisibility::True,this->vInSetAngle);
			this->vInSetAngle->SetSubType(0,1,1.0,0.0,true,false,false);

			this->FHost->CreateValueInput("Velocity",2,nullptr,v4::TSliceMode::Dynamic,v4::TPinVisibility::True,this->vInVelocity);
			this->vInVelocity->SetSubType2D(System::Double::MinValue,System::Double::MaxValue,0.01,0.0,0.0,false,false,false);

			this->FHost->CreateValueInput("Set Velocity",1,nullptr,v4::TSliceMode::Dynamic,v4::TPinVisibility::True,this->vInSetVelocity);
			this->vInSetVelocity->SetSubType(0,1,1.0,0.0,true,false,false);	

			this->FHost->CreateValueInput("Angular Velocity",1,nullptr,v4::TSliceMode::Dynamic,v4::TPinVisibility::True,this->vInAngularVelocity);
			this->vInAngularVelocity->SetSubType(System::Double::MinValue,System::Double::MaxValue,0.01,0.0,false,false,false);

			this->FHost->CreateValueInput("Set Angular Velocity",1,nullptr,v4::TSliceMode::Dynamic,v4::TPinVisibility::True,this->vInSetAngularVelocity);
			this->vInSetAngularVelocity->SetSubType(0,1,1.0,0.0,true,false,false);

			this->FHost->CreateStringInput("Custom",v4::TSliceMode::Dynamic,v4::TPinVisibility::True,this->vInCustom);
			this->vInCustom->SetSubType("",false);

			this->FHost->CreateValueInput("Set Custom",1,nullptr,v4::TSliceMode::Dynamic,v4::TPinVisibility::True,this->vInSetCustom);
			this->vInSetCustom->SetSubType(0,1,1,0.0,true,false,false);

			this->FHost->CreateValueInput("Sleeping",1,nullptr,v4::TSliceMode::Dynamic,v4::TPinVisibility::True,this->vInSleeping);
			this->vInSleeping->SetSubType(0,1,1.0,0.0,false,true,false);

			this->FHost->CreateValueInput("Set Sleeping",1,nullptr,v4::TSliceMode::Dynamic,v4::TPinVisibility::True,this->vInSetSleeping);
			this->vInSetSleeping->SetSubType(0,1,1.0,0.0,true,false,false);

			//Time to live
			this->FHost->CreateValueInput("TTL",1,nullptr,v4::TSliceMode::Dynamic,v4::TPinVisibility::True,this->vInTTL);
			this->vInTTL->SetSubType(System::Double::MinValue,System::Double::MaxValue,0.01,1.0,false,false,false);

			this->FHost->CreateValueInput("Has TTL",1,nullptr,v4::TSliceMode::Dynamic,v4::TPinVisibility::True,this->vInHasTTL);
			this->vInHasTTL->SetSubType(0,1,1.0,0.0,false,true,false);

			this->FHost->CreateValueInput("Set TTL",1,nullptr,v4::TSliceMode::Dynamic,v4::TPinVisibility::True,this->vInSetTTL);
			this->vInSetTTL->SetSubType(0,1,1.0,0.0,true,false,false);
		}

		
		void Box2dUpdateBodyNode::Configurate(v4::IPluginConfig^ Input)
		{

		}

		
		void Box2dUpdateBodyNode::Evaluate(int SpreadMax)
		{
			this->vInBodies->PinIsChanged;
			if (this->vInBodies->IsConnected) 
			{
				double dblsp,dblsv,dblsav,dblsc,dblsa,dblsleep,dblsetsleep,dblttl,dblhasttl,dblsetttl;
				for (int i = 0; i < this->vInBodies->SliceCount; i++) 
				{
					int realslice;
					this->vInBodies->GetUpsreamSlice(i % this->vInBodies->SliceCount,realslice);
					b2Body* body = this->m_bodies->GetSlice(realslice);
					BodyCustomData* bdata = (BodyCustomData*)body->GetUserData();

					int id = bdata->Id;

					this->vInSetPosition->GetValue(i,dblsp);
					this->vInSetAngle->GetValue(i, dblsa);
					if (dblsp >= 0.5 || dblsa >= 0.5) 
					{
						double x,y,a;

						if (dblsp)
						{
							this->vInPosition->GetValue2D(i,x,y);
						}
						else
						{
							x = body->GetPosition().x;
							y = body->GetPosition().y;
						}

						if (dblsa)
						{
							this->vInAngle->GetValue(i, a);
							a = a * (float)System::Math::PI;
						}
						else 
						{
							a = body->GetAngle();
						}

						body->SetXForm(b2Vec2(System::Convert::ToSingle(x), System::Convert::ToSingle(y)), System::Convert::ToSingle(a));
					}

					this->vInSetVelocity->GetValue(i,dblsv);
					if (dblsv >= 0.5) 
					{
						double x,y;
						this->vInVelocity->GetValue2D(i,x,y);
						body->SetLinearVelocity(b2Vec2(System::Convert::ToSingle(x), System::Convert::ToSingle(y)));
					}

					this->vInSetAngularVelocity->GetValue(i,dblsav);
					if (dblsav >= 0.5) 
					{
						double a;
						this->vInAngularVelocity->GetValue(i,a);
						body->SetAngularVelocity(System::Convert::ToSingle(a));
					}

					this->vInSetCustom->GetValue(i,dblsc);
					if (dblsc >= 0.5)
					{
						System::String^ cust;
						this->vInCustom->GetString(i,cust);
						BodyCustomData* bdata = (BodyCustomData*)body->GetUserData();
						bdata->Custom = (char*)(void*)System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(cust);
					}

					this->vInSetSleeping->GetValue(i, dblsetsleep);
					if (dblsetsleep > 0.5)
					{
						this->vInSleeping->GetValue(i,dblsleep);
						if (dblsleep > 0.5 || body->IsSleeping())
						{
							body->WakeUp();
						}

						if (dblsleep < 0.5 || !body->IsSleeping())
						{
							body->PutToSleep();
						}
					}

					this->vInSetTTL->GetValue(i, dblsetttl);
					if (dblsetttl > 0.5)
					{
						this->vInTTL->GetValue(i,dblttl);
						this->vInHasTTL->GetValue(i,dblhasttl);

						BodyCustomData* bdata = (BodyCustomData*)body->GetUserData();
						bdata->HasTTL = dblhasttl > 0.5;
						bdata->TTL = dblttl;
					}


				}
			}
		}




		
		void Box2dUpdateBodyNode::ConnectPin(v4::IPluginIO^ Pin)
		{
			if (Pin == this->vInBodies) 
			{
				v4::INodeIOBase^ usI;
				this->vInBodies->GetUpstreamInterface(usI);
				this->m_bodies = (v4b2d::BodyDataType^)usI;
			}
		}


		void Box2dUpdateBodyNode::DisconnectPin(v4::IPluginIO^ Pin)
		{
			if (Pin == this->vInBodies)
        	{
        		this->m_bodies = nullptr;
        	}
		}
	}
}
