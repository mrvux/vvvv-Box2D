#include "StdAfx.h"
#include "Box2dContactDetailsDualNode.h"
#include "../Internals/Data/ShapeCustomData.h"
#include "../Internals/Data/BodyCustomData.h"

namespace VVVV 
{
	namespace Nodes 
	{
		Box2dContactDetailsDualNode::Box2dContactDetailsDualNode(void)
		{
			this->m_shape = gcnew ShapeDataType();
			this->mBody = gcnew BodyDataType();
		}

		void Box2dContactDetailsDualNode::SetPluginHost(IPluginHost^ Host) 
		{
			this->FHost = Host;
	
			this->FHost->CreateNodeInput("World",TSliceMode::Dynamic,TPinVisibility::True,this->vInWorld);
			this->vInWorld->SetSubType(ArrayUtils::SingleGuidArray(WorldDataType::GUID),WorldDataType::FriendlyName);

			this->FHost->CreateValueOutput("Contact Id",1,nullptr,TSliceMode::Dynamic,TPinVisibility::True,this->vOutId);
			this->vOutId->SetSubType(Double::MinValue,Double::MaxValue,1,0.0,false,false,true);

			this->FHost->CreateValueOutput("Position",2,nullptr,TSliceMode::Dynamic,TPinVisibility::True,this->vOutPosition);
			this->vOutPosition->SetSubType2D(Double::MinValue,Double::MaxValue,0.01,0.0,0.0,false,false,false);

			this->FHost->CreateValueOutput("Normals",2,nullptr,TSliceMode::Dynamic,TPinVisibility::True,this->vOutNormals);
			this->vOutNormals->SetSubType2D(Double::MinValue,Double::MaxValue,0.01,0.0,0.0,false,false,false);

			this->FHost->CreateValueOutput("Normal Impulse", 1, nullptr, TSliceMode::Dynamic, TPinVisibility::True, this->vOutNormalImpulse);
			this->vOutNormalImpulse->SetSubType(Double::MinValue, Double::MaxValue, 0.01, 0, false,false, false);

			this->FHost->CreateValueOutput("Tangent Impulse", 1, nullptr, TSliceMode::Dynamic, TPinVisibility::True, this->vOutTangentImpulse);
			this->vOutTangentImpulse->SetSubType(Double::MinValue, Double::MaxValue, 0.01, 0, false,false, false);

			this->FHost->CreateNodeOutput("Shapes",TSliceMode::Dynamic,TPinVisibility::True,this->vOutShape);
			this->vOutShape->SetSubType(ArrayUtils::SingleGuidArray(ShapeDataType::GUID),ShapeDataType::FriendlyName);
			this->vOutShape->SetInterface(this->m_shape);
	
			this->FHost->CreateNodeOutput("Bodies",TSliceMode::Dynamic,TPinVisibility::True,this->vOutBody);
			this->vOutBody->SetSubType(ArrayUtils::SingleGuidArray(BodyDataType::GUID),BodyDataType::FriendlyName);
			this->vOutBody->SetInterface(this->mBody);

			this->FHost->CreateValueOutput("Bodies Id",1,nullptr,TSliceMode::Dynamic,TPinVisibility::True,this->vOutBodyId);
			this->vOutBodyId->SetSubType(Double::MinValue,Double::MaxValue,1,0.0,false,false,true);

			this->FHost->CreateValueOutput("Shapes Id",1,nullptr,TSliceMode::Dynamic,TPinVisibility::True,this->vOutShapes);
			this->vOutShapes->SetSubType(Double::MinValue,Double::MaxValue,1,0.0,false,false,true);

			this->FHost->CreateValueOutput("Is New",1,nullptr,TSliceMode::Dynamic,TPinVisibility::True,this->vOutNew);
			this->vOutNew->SetSubType(Double::MinValue,Double::MaxValue,0.01,0.0,true,false,false);

			this->FHost->CreateValueOutput("Contact Count",1,nullptr,TSliceMode::Single,TPinVisibility::True,this->vOutContactCount);
			this->vOutContactCount->SetSubType(Double::MinValue,Double::MaxValue,0.01,0.0,true,false,false);

		}


		void Box2dContactDetailsDualNode::Configurate(IPluginConfig^ Input)
		{

		}

		
		void Box2dContactDetailsDualNode::Evaluate(int SpreadMax)
		{
			if (this->vInWorld->IsConnected) 
			{		
				if (m_world->GetIsValid()) 
				{
					this->vOutContactCount->SetValue(0,this->m_world->GetWorld()->GetContactCount());

					this->vOutId->SliceCount = this->m_world->Results->size();
					this->vOutPosition->SliceCount = this->m_world->Results->size();
					this->vOutNormals->SliceCount = this->m_world->Results->size();
					this->vOutShapes->SliceCount = this->m_world->Results->size() * 2;
					this->vOutShape->SliceCount = this->m_world->Results->size() * 2;
					this->vOutBody->SliceCount = this->m_world->Results->size() * 2;
					this->vOutBodyId->SliceCount = this->m_world->Results->size() * 2;
					this->vOutNew->SliceCount = this->m_world->Results->size();
					this->vOutNormalImpulse->SliceCount = this->m_world->Results->size();
					this->vOutTangentImpulse->SliceCount = this->m_world->Results->size();
					this->m_shape->Reset();
					this->mBody->Reset();

					for (int i = 0; i < this->m_world->Results->size(); i++)
					{
						ContactResultData* result = this->m_world->Results->at(i);
						this->vOutId->SetValue(i, Convert::ToDouble(result->id.key));
						this->vOutPosition->SetValue2D(i,result->position.x,result->position.y);
						this->vOutNormals->SetValue2D(i,result->normal.x,result->normal.y);
						this->vOutNormalImpulse->SetValue(i, result->normalImpulse);
						this->vOutTangentImpulse->SetValue(i, result->tangentImpulse);
						

						ShapeCustomData* sdata1 = (ShapeCustomData*)result->shape1->GetUserData();
						this->vOutShapes->SetValue(i * 2,sdata1->Id);
						ShapeCustomData* sdata2 = (ShapeCustomData*)result->shape2->GetUserData();
						this->vOutShapes->SetValue(i * 2 + 1,sdata2->Id);

						bool found = false;
						for (int c = 0; c < this->m_world->Reports->size() && !found; c++)
						{
							ContactReportId* report = this->m_world->Reports->at(c);
							if (report->FeaturesId == result->id.key
								&& report->Shape1 == sdata1->Id
								&& report->Shape2 == sdata2->Id)
							{
								found = true;
								this->vOutNew->SetValue(i,report->IsNew);
							}
						}

						if (!found)
						{
							this->vOutNew->SetValue(i, 0);
						}

						BodyCustomData* bdata = (BodyCustomData*)result->shape1->GetBody()->GetUserData();
						this->vOutBodyId->SetValue(i * 2, bdata->Id);
						bdata = (BodyCustomData*)result->shape2->GetBody()->GetUserData();
						this->vOutBodyId->SetValue(i * 2  +1, bdata->Id);

						this->m_shape->Add(result->shape1);
						this->m_shape->Add(result->shape2);
						this->mBody->Add(result->shape1->GetBody());
						this->mBody->Add(result->shape2->GetBody());

					}
					this->vOutShape->MarkPinAsChanged();
					this->vOutBody->MarkPinAsChanged();
				}
				else 
				{
					this->vOutPosition->SliceCount = 0;
					this->vOutNormals->SliceCount = 0;
					this->vOutShapes->SliceCount = 0;
					this->vOutNew->SliceCount = 0;
					this->vOutBody->SliceCount = 0;
				}
			} 
			else 
			{
				this->vOutPosition->SliceCount = 0;
				this->vOutNormals->SliceCount = 0;
				this->vOutShapes->SliceCount = 0;
				this->vOutNew->SliceCount = 0;
				this->vOutBody->SliceCount = 0;
			}
		}


		void Box2dContactDetailsDualNode::ConnectPin(IPluginIO^ Pin)
		{
			if (Pin == this->vInWorld) 
			{
				INodeIOBase^ usI;
				this->vInWorld->GetUpstreamInterface(usI);
				this->m_world = (WorldDataType^)usI;
			}
		}

		void Box2dContactDetailsDualNode::DisconnectPin(IPluginIO^ Pin)
		{
			if (Pin == this->vInWorld)
        	{
        		this->m_world = nullptr;
        	}
		}
	}
}
