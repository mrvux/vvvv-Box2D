#include "StdAfx.h"
#include "Box2dContactDetailsNode.h"
#include "../Internals/Data/ShapeCustomData.h"
#include "../Internals/Data/BodyCustomData.h"

namespace VVVV 
{
	namespace Nodes 
	{
		Box2dContactDetailsNode::Box2dContactDetailsNode(void)
		{
			this->m_shape1 = gcnew ShapeDataType();
			this->m_shape2 = gcnew ShapeDataType();
			this->mBody1 = gcnew BodyDataType();
			this->mBody2 = gcnew BodyDataType();
		}

		void Box2dContactDetailsNode::SetPluginHost(IPluginHost^ Host) 
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

			this->FHost->CreateNodeOutput("Shape 1",TSliceMode::Dynamic,TPinVisibility::True,this->vOutShape1);
			this->vOutShape1->SetSubType(ArrayUtils::SingleGuidArray(ShapeDataType::GUID),ShapeDataType::FriendlyName);
			this->vOutShape1->SetInterface(this->m_shape1);
		
			this->FHost->CreateNodeOutput("Shape 2",TSliceMode::Dynamic,TPinVisibility::True,this->vOutShape2);
			this->vOutShape2->SetSubType(ArrayUtils::SingleGuidArray(ShapeDataType::GUID),ShapeDataType::FriendlyName);
			this->vOutShape2->SetInterface(this->m_shape2);

			this->FHost->CreateNodeOutput("Body 1",TSliceMode::Dynamic,TPinVisibility::True,this->vOutBody1);
			this->vOutBody1->SetSubType(ArrayUtils::SingleGuidArray(BodyDataType::GUID),BodyDataType::FriendlyName);
			this->vOutBody1->SetInterface(this->mBody1);

			this->FHost->CreateNodeOutput("Body 2",TSliceMode::Dynamic,TPinVisibility::True,this->vOutBody2);
			this->vOutBody2->SetSubType(ArrayUtils::SingleGuidArray(BodyDataType::GUID),BodyDataType::FriendlyName);
			this->vOutBody2->SetInterface(this->mBody2);

			this->FHost->CreateValueOutput("Body Id 1",1,nullptr,TSliceMode::Dynamic,TPinVisibility::True,this->vOutBodyId1);
			this->vOutBodyId1->SetSubType(Double::MinValue,Double::MaxValue,1,0.0,false,false,true);

			this->FHost->CreateValueOutput("Shapes 1",1,nullptr,TSliceMode::Dynamic,TPinVisibility::True,this->vOutShapes1);
			this->vOutShapes1->SetSubType(Double::MinValue,Double::MaxValue,1,0.0,false,false,true);

			this->FHost->CreateValueOutput("Body Id 2",1,nullptr,TSliceMode::Dynamic,TPinVisibility::True,this->vOutBodyId2);
			this->vOutBodyId2->SetSubType(Double::MinValue,Double::MaxValue,1,0.0,false,false,true);


			this->FHost->CreateValueOutput("Shapes 2",1,nullptr,TSliceMode::Dynamic,TPinVisibility::True,this->vOutShapes2);
			this->vOutShapes2->SetSubType(Double::MinValue,Double::MaxValue,1,0.0,false,false,true);

			this->FHost->CreateValueOutput("Is New",1,nullptr,TSliceMode::Dynamic,TPinVisibility::True,this->vOutNew);
			this->vOutNew->SetSubType(Double::MinValue,Double::MaxValue,0.01,0.0,true,false,false);

			this->FHost->CreateValueOutput("Contact Count",1,nullptr,TSliceMode::Single,TPinVisibility::True,this->vOutContactCount);
			this->vOutContactCount->SetSubType(Double::MinValue,Double::MaxValue,0.01,0.0,true,false,false);

		}


		void Box2dContactDetailsNode::Configurate(IPluginConfig^ Input)
		{

		}

		
		void Box2dContactDetailsNode::Evaluate(int SpreadMax)
		{
			if (this->vInWorld->IsConnected) 
			{		
				if (m_world->GetIsValid()) 
				{
					this->vOutContactCount->SetValue(0,this->m_world->GetWorld()->GetContactCount());

					this->vOutId->SliceCount = this->m_world->Results->size();
					this->vOutPosition->SliceCount = this->m_world->Results->size();
					this->vOutNormals->SliceCount = this->m_world->Results->size();
					this->vOutShapes1->SliceCount = this->m_world->Results->size();
					this->vOutShapes2->SliceCount = this->m_world->Results->size();
					this->vOutShape1->SliceCount = this->m_world->Results->size();
					this->vOutShape2->SliceCount = this->m_world->Results->size();
					this->vOutBody1->SliceCount = this->m_world->Results->size();
					this->vOutBody2->SliceCount = this->m_world->Results->size();
					this->vOutBodyId1->SliceCount = this->m_world->Results->size();
					this->vOutBodyId2->SliceCount = this->m_world->Results->size();
					this->vOutNew->SliceCount = this->m_world->Results->size();
					this->vOutNormalImpulse->SliceCount = this->m_world->Results->size();
					this->vOutTangentImpulse->SliceCount = this->m_world->Results->size();
					this->m_shape1->Reset();
					this->m_shape2->Reset();
					this->mBody1->Reset();
					this->mBody2->Reset();

					for (int i = 0; i < this->m_world->Results->size(); i++)
					{
						ContactResultData* result = this->m_world->Results->at(i);
						this->vOutId->SetValue(i, Convert::ToDouble(result->id.key));
						this->vOutPosition->SetValue2D(i,result->position.x,result->position.y);
						this->vOutNormals->SetValue2D(i,result->normal.x,result->normal.y);
						this->vOutNormalImpulse->SetValue(i, result->normalImpulse);
						this->vOutTangentImpulse->SetValue(i, result->tangentImpulse);
						

						ShapeCustomData* sdata1 = (ShapeCustomData*)result->shape1->GetUserData();
						this->vOutShapes1->SetValue(i,sdata1->Id);
						ShapeCustomData* sdata2 = (ShapeCustomData*)result->shape2->GetUserData();
						this->vOutShapes2->SetValue(i,sdata2->Id);

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
						this->vOutBodyId1->SetValue(i, bdata->Id);
						bdata = (BodyCustomData*)result->shape2->GetBody()->GetUserData();
						this->vOutBodyId2->SetValue(i, bdata->Id);

						this->m_shape1->Add(result->shape1);
						this->m_shape2->Add(result->shape2);
						this->mBody1->Add(result->shape1->GetBody());
						this->mBody2->Add(result->shape2->GetBody());

					}
					this->vOutShape1->MarkPinAsChanged();
					this->vOutShape2->MarkPinAsChanged();
					this->vOutBody1->MarkPinAsChanged();
					this->vOutBody2->MarkPinAsChanged();
				}
				else 
				{
					this->vOutPosition->SliceCount = 0;
					this->vOutNormals->SliceCount = 0;
					this->vOutShapes1->SliceCount = 0;
					this->vOutShapes2->SliceCount = 0;
					this->vOutNew->SliceCount = 0;
				}
			} 
			else 
			{
				this->vOutPosition->SliceCount = 0;
				this->vOutNormals->SliceCount = 0;
				this->vOutShapes1->SliceCount = 0;
				this->vOutShapes2->SliceCount = 0;
				this->vOutNew->SliceCount = 0;
			}
		}


		void Box2dContactDetailsNode::ConnectPin(IPluginIO^ Pin)
		{
			if (Pin == this->vInWorld) 
			{
				INodeIOBase^ usI;
				this->vInWorld->GetUpstreamInterface(usI);
				this->m_world = (WorldDataType^)usI;
			}
		}

		void Box2dContactDetailsNode::DisconnectPin(IPluginIO^ Pin)
		{
			if (Pin == this->vInWorld)
        	{
        		this->m_world = nullptr;
        	}
		}
	}
}
