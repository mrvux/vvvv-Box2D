#include "StdAfx.h"
#include "GetBodyDetailsNode.h"
#include "../../Internals/Data/BodyCustomData.h"

using namespace System::Collections::Generic;

namespace VVVV 
{
	namespace Nodes 
	{

		GetBodyDetailsNode::GetBodyDetailsNode(void)
		{
			this->m_shapes = gcnew ShapeDataType();
		}

		void GetBodyDetailsNode::SetPluginHost(IPluginHost^ Host) 
		{
			this->FHost = Host;

			this->FHost->CreateNodeInput("Bodies",TSliceMode::Dynamic,TPinVisibility::True,this->vInBodies);
			this->vInBodies->SetSubType(ArrayUtils::DoubleGuidArray(BodyDataType::GUID, GroundDataType::GUID), BodyDataType::FriendlyName);
			//this->vInBodies->SetSubType(ArrayUtils::SingleGuidArray(BodyDataType::GUID), BodyDataType::FriendlyName);//   

			this->FHost->CreateValueInput("Filter Persisted", 1, nullptr, TSliceMode::Single, TPinVisibility::True, this->vInFilterPersisted);
			this->vInFilterPersisted->SetSubType(0, 1, 1, 0, false,true, false);

			this->FHost->CreateValueOutput("Position",2,nullptr,TSliceMode::Dynamic,TPinVisibility::True,this->vOutPosition);
			this->vOutPosition->SetSubType2D(Double::MinValue,Double::MaxValue,0.01,0.0,0.0,false,false,false);

			this->FHost->CreateValueOutput("Rotation",1,nullptr,TSliceMode::Dynamic,TPinVisibility::True,this->vOutRotation);
			this->vOutRotation->SetSubType(Double::MinValue,Double::MaxValue,0.01,0.0,false,false,false);

			this->FHost->CreateValueOutput("Velocity",2,nullptr,TSliceMode::Dynamic,TPinVisibility::True,this->vOutVelocity);
			this->vOutVelocity->SetSubType2D(Double::MinValue,Double::MaxValue,0.01,0.0,0.0,false,false,false);

			this->FHost->CreateValueOutput("Is Dynamic",1,nullptr,TSliceMode::Dynamic,TPinVisibility::True,this->vOutIsDynamic);
			this->vOutIsDynamic->SetSubType(Double::MinValue,Double::MaxValue,1,0.0,false,true,false);

			this->FHost->CreateValueOutput("Is Sleeping",1,nullptr,TSliceMode::Dynamic,TPinVisibility::True,this->vOutIsSleeping);
			this->vOutIsSleeping->SetSubType(Double::MinValue,Double::MaxValue,1,0.0,false,true,false);

			this->FHost->CreateValueOutput("Is Frozen",1,nullptr,TSliceMode::Dynamic,TPinVisibility::True,this->vOutIsFrozen);
			this->vOutIsFrozen->SetSubType(Double::MinValue,Double::MaxValue,1,0.0,false,true,false);

			this->FHost->CreateValueOutput("Mass",1,nullptr,TSliceMode::Dynamic,TPinVisibility::True,this->vOutMass);
			this->vOutMass->SetSubType(Double::MinValue,Double::MaxValue,0.01,0.0,false,false,false);

			this->FHost->CreateValueOutput("Inertia",1,nullptr,TSliceMode::Dynamic,TPinVisibility::True,this->vOutInertia);
			this->vOutInertia->SetSubType(Double::MinValue,Double::MaxValue,0.01,0.0,false,false,false);

			this->FHost->CreateStringOutput("Custom",TSliceMode::Dynamic,TPinVisibility::True,this->vOutCustom);
			this->vOutCustom->SetSubType("",false);

			this->FHost->CreateNodeOutput("Shapes",TSliceMode::Dynamic,TPinVisibility::True,this->vOutShapes);
			this->vOutShapes->SetSubType(ArrayUtils::SingleGuidArray(ShapeDataType::GUID),ShapeDataType::FriendlyName);
			this->vOutShapes->SetInterface(this->m_shapes);

			this->FHost->CreateStringOutput("Shape Types",TSliceMode::Dynamic,TPinVisibility::True,this->vOutShapeType);
			this->vOutShapeType->SetSubType("",false);

			this->FHost->CreateValueOutput("Shape Count",1,nullptr,TSliceMode::Dynamic,TPinVisibility::True,this->vOutShapeCount);
			this->vOutShapeCount->SetSubType(0,Double::MaxValue,1,0.0,false,false,true);


			this->FHost->CreateValueOutput("Body Id",1,nullptr,TSliceMode::Dynamic,TPinVisibility::True,this->vOutId);
			this->vOutId->SetSubType(Double::MinValue,Double::MaxValue,1,0.0,false,false,true);

			this->FHost->CreateValueOutput("LifeTime",1,nullptr,TSliceMode::Dynamic,TPinVisibility::True,this->vOutLifeTime);
			this->vOutLifeTime->SetSubType(Double::MinValue,Double::MaxValue,1,0.0,false,false,false);

			this->FHost->CreateValueOutput("Has TTL",1,nullptr,TSliceMode::Dynamic,TPinVisibility::True,this->vOutHasTTL);
			this->vOutHasTTL->SetSubType(Double::MinValue,Double::MaxValue,1,0.0,false,true,false);

			this->FHost->CreateValueOutput("TTL",1,nullptr,TSliceMode::Dynamic,TPinVisibility::True,this->vOutTTL);
			this->vOutTTL->SetSubType(Double::MinValue,Double::MaxValue,1,0.0,false,false,false);

			//this->FHost->CreateValueOutput("Velocity",2,nullptr,TSliceMode::Dynamic,TPinVisibility::True,this->vOutVelocity);
			//this->vOutVelocity->SetSubType2D(Double::MinValue,Double::MaxValue,0.01,0.0,0.0,false,false,false);
		}


		void GetBodyDetailsNode::Configurate(IPluginConfig^ Input)
		{

		}

		
		void GetBodyDetailsNode::Evaluate(int SpreadMax)
		{
			FHost->Log(VVVV::PluginInterfaces::V1::TLogType::Debug, this->vInBodies->IsConnected.ToString());
			if (this->vInBodies->IsConnected) 
			{
				this->m_shapes->Reset();

				int cnt = this->vInBodies->SliceCount;
				double dblfilter;
				this->vInFilterPersisted->GetValue(0, dblfilter);
				if (dblfilter > 0.5)
				{
					cnt = 0;
					for (int i = 0; i < this->vInBodies->SliceCount; i++) 
					{	
						b2Body* body;

						if (this->isbody) 
						{
							int realslicebody;
							this->vInBodies->GetUpsreamSlice(i % this->vInBodies->SliceCount,realslicebody);
							body = this->m_bodies->GetSlice(realslicebody);
							BodyCustomData* bdata = (BodyCustomData*)body->GetUserData();
							if (!bdata->Persisted)
							{
								cnt++;
							} 
						} 
						else
						{
							body = this->mGround->GetGround();
							cnt++;
						}
					}
				}

				this->vOutPosition->SliceCount = cnt;
				this->vOutVelocity->SliceCount = cnt;
				this->vOutRotation->SliceCount = cnt;
				this->vOutIsDynamic->SliceCount = cnt;
				this->vOutId->SliceCount = cnt;
				this->vOutCustom->SliceCount = cnt;
				this->vOutMass->SliceCount = cnt;
				this->vOutInertia->SliceCount = cnt;
				this->vOutShapeCount->SliceCount = cnt;
				this->vOutIsSleeping->SliceCount = cnt;
				this->vOutIsFrozen->SliceCount = cnt;
				this->vOutLifeTime->SliceCount = cnt;
				this->vOutHasTTL->SliceCount = cnt;
				this->vOutTTL->SliceCount = cnt;
				List<String^>^ types = gcnew List<String^>();

				int cntper = 0;
				for (int i = 0; i < this->vInBodies->SliceCount; i++) 
				{
					b2Body* body;

					if (this->isbody) 
					{
						int realslicebody;
						this->vInBodies->GetUpsreamSlice(i % this->vInBodies->SliceCount,realslicebody);
						body = this->m_bodies->GetSlice(realslicebody);
					} 
					else
					{
						body = this->mGround->GetGround();
					}

					b2Vec2 pos = body->GetPosition();
					b2Vec2 vel = body->GetLinearVelocity();
					this->vOutPosition->SetValue2D(cntper, pos.x,pos.y);
					this->vOutVelocity->SetValue2D(cntper, vel.x,vel.y);
					this->vOutRotation->SetValue(cntper,body->GetAngle() / (Math::PI * 2.0));
					this->vOutIsDynamic->SetValue(cntper,Convert::ToInt32(body->IsDynamic()));
					this->vOutMass->SetValue(cntper,body->GetMass());
					this->vOutInertia->SetValue(cntper,body->GetInertia());
					this->vOutIsFrozen->SetValue(cntper,Convert::ToDouble(body->IsFrozen()));
					this->vOutIsSleeping->SetValue(cntper,Convert::ToDouble(body->IsSleeping()));

					if (this->isbody) 
					{
						BodyCustomData* bdata = (BodyCustomData*)body->GetUserData();
						this->vOutId->SetValue(cntper, bdata->Id);
						String^ str = gcnew String(bdata->Custom);
						this->vOutCustom->SetString(cntper,str);
						this->vOutLifeTime->SetValue(cntper,bdata->LifeTime);
						this->vOutHasTTL->SetValue(cntper,Convert::ToDouble(bdata->HasTTL));
						this->vOutTTL->SetValue(cntper,bdata->TTL);
					}
					else
					{
						this->vOutId->SetValue(cntper, -1);
						this->vOutCustom->SetString(cntper,"");
						this->vOutLifeTime->SetValue(cntper,0);
						this->vOutHasTTL->SetValue(cntper,0);
						this->vOutTTL->SetValue(cntper,-1);
					}

					int shapecount = 0;
					for (b2Shape* s = body->GetShapeList(); s; s = s->GetNext())
					{
						if (s->GetType() == e_circleShape || s->GetType() == e_polygonShape || s->GetType() == e_edgeShape) 
						{
							this->m_shapes->Add(s);
							shapecount++;
						}

						String^ str;
						switch (s->GetType())
						{
						case e_circleShape:
							str = "Circle";
							break;
						case e_polygonShape:
							str = "Polygon";
							break;
						case e_edgeShape:
							str = "Edge";
							break;
						default:
							str = "Unknown";
							break;
						}
						types->Add(str);
						
					}
					this->vOutShapeCount->SetValue(cntper,shapecount);
					cntper++;
				}

				this->vOutShapes->SliceCount = this->m_shapes->Count();
				this->vOutShapeType->SliceCount = types->Count;
				for (int i = 0; i < types->Count; i++)
				{
					this->vOutShapeType->SetString(i, types[i]);
				}
			}
		}


		void GetBodyDetailsNode::ConnectPin(IPluginIO^ Pin)
		{
			if (Pin == this->vInBodies)
        	{
				INodeIOBase^ usI;
				try 
				{
					this->vInBodies->GetUpstreamInterface(usI);
					this->m_bodies = (BodyDataType^)usI;
					this->isbody = true;
				} 
				catch (Exception^ ex)
				{
					this->vInBodies->GetUpstreamInterface(usI);
					this->mGround = (GroundDataType^)usI;
					this->isbody = false;
				}
        	}
		}

		void GetBodyDetailsNode::DisconnectPin(IPluginIO^ Pin)
		{
			if (Pin == this->vInBodies)
        	{
        		if (this->isbody) 
				{
        			this->m_bodies = nullptr;
				} 
				else 
				{
					this->mGround = nullptr;
				}
        	}
		}
	}
}
