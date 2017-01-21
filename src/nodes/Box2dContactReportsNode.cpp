#include "Box2dContactReportsNode.h"
#include "Internals/Data/ShapeCustomData.h"
#include "Internals/Data/BodyCustomData.h"


namespace VVVV 
{
	namespace Nodes 
	{
		Box2dContactReportsNode::Box2dContactReportsNode(void)
		{
			this->m_shape1 = gcnew v4b2d::ShapeDataType();
			this->m_shape2 = gcnew v4b2d::ShapeDataType();
			this->mBody1 = gcnew v4b2d::BodyDataType();
			this->mBody2 = gcnew v4b2d::BodyDataType();
		}

		void Box2dContactReportsNode::SetPluginHost(v4::IPluginHost^ Host) 
		{
			this->FHost = Host;
	
			this->FHost->CreateNodeInput("World",v4::TSliceMode::Dynamic,v4::TPinVisibility::True,this->vInWorld);
			this->vInWorld->SetSubType(VVVV::Utils::ArrayUtils::SingleGuidArray(v4b2d::WorldDataType::GUID), v4b2d::WorldDataType::FriendlyName);

			this->FHost->CreateValueOutput("Contact Id",1,nullptr,v4::TSliceMode::Dynamic,v4::TPinVisibility::True,this->vOutId);
			this->vOutId->SetSubType(System::Double::MinValue, System::Double::MaxValue,1,0.0,false,false,true);

			this->FHost->CreateValueOutput("Position",2,nullptr,v4::TSliceMode::Dynamic,v4::TPinVisibility::True,this->vOutPosition);
			this->vOutPosition->SetSubType2D(System::Double::MinValue,System::Double::MaxValue,0.01,0.0,0.0,false,false,false);

			this->FHost->CreateNodeOutput("Shape 1",v4::TSliceMode::Dynamic,v4::TPinVisibility::True,this->vOutShape1);
			this->vOutShape1->SetSubType(VVVV::Utils::ArrayUtils::SingleGuidArray(v4b2d::ShapeDataType::GUID), v4b2d::ShapeDataType::FriendlyName);
			this->vOutShape1->SetInterface(this->m_shape1);
		
			this->FHost->CreateNodeOutput("Shape 2",v4::TSliceMode::Dynamic,v4::TPinVisibility::True,this->vOutShape2);
			this->vOutShape2->SetSubType(VVVV::Utils::ArrayUtils::SingleGuidArray(v4b2d::ShapeDataType::GUID), v4b2d::ShapeDataType::FriendlyName);
			this->vOutShape2->SetInterface(this->m_shape2);

			this->FHost->CreateNodeOutput("Body 1",v4::TSliceMode::Dynamic,v4::TPinVisibility::True,this->vOutBody1);
			this->vOutBody1->SetSubType(VVVV::Utils::ArrayUtils::SingleGuidArray(v4b2d::BodyDataType::GUID), v4b2d::BodyDataType::FriendlyName);
			this->vOutBody1->SetInterface(this->mBody1);

			this->FHost->CreateNodeOutput("Body 2",v4::TSliceMode::Dynamic,v4::TPinVisibility::True,this->vOutBody2);
			this->vOutBody2->SetSubType(VVVV::Utils::ArrayUtils::SingleGuidArray(v4b2d::BodyDataType::GUID), v4b2d::BodyDataType::FriendlyName);
			this->vOutBody2->SetInterface(this->mBody2);

			this->FHost->CreateValueOutput("Body Id 1",1,nullptr,v4::TSliceMode::Dynamic,v4::TPinVisibility::True,this->vOutBodyId1);
			this->vOutBodyId1->SetSubType(System::Double::MinValue,System::Double::MaxValue,1,0.0,false,false,true);

			this->FHost->CreateValueOutput("Shapes 1",1,nullptr,v4::TSliceMode::Dynamic,v4::TPinVisibility::True,this->vOutShapes1);
			this->vOutShapes1->SetSubType(System::Double::MinValue,System::Double::MaxValue,1,0.0,false,false,true);

			this->FHost->CreateValueOutput("Body Id 2",1,nullptr,v4::TSliceMode::Dynamic,v4::TPinVisibility::True,this->vOutBodyId2);
			this->vOutBodyId2->SetSubType(System::Double::MinValue,System::Double::MaxValue,1,0.0,false,false,true);


			this->FHost->CreateValueOutput("Shapes 2",1,nullptr,v4::TSliceMode::Dynamic,v4::TPinVisibility::True,this->vOutShapes2);
			this->vOutShapes2->SetSubType(System::Double::MinValue,System::Double::MaxValue,1,0.0,false,false,true);

			this->FHost->CreateValueOutput("Is New",1,nullptr,v4::TSliceMode::Dynamic,v4::TPinVisibility::True,this->vOutNew);
			this->vOutNew->SetSubType(System::Double::MinValue,System::Double::MaxValue,0.01,0.0,true,false,false);

			this->FHost->CreateValueOutput("Contact Count",1,nullptr,v4::TSliceMode::Single,v4::TPinVisibility::True,this->vOutContactCount);
			this->vOutContactCount->SetSubType(System::Double::MinValue,System::Double::MaxValue,0.01,0.0,true,false,false);

		}


		void Box2dContactReportsNode::Configurate(v4::IPluginConfig^ Input)
		{

		}

		
		void Box2dContactReportsNode::Evaluate(int SpreadMax)
		{
			if (this->vInWorld->IsConnected) 
			{		
				if (m_world->GetIsValid()) 
				{
					this->vOutContactCount->SetValue(0,this->m_world->GetWorld()->GetContactCount());

					this->vOutId->SliceCount = this->m_world->Reports->size();
					this->vOutPosition->SliceCount = this->m_world->Reports->size();
					this->vOutShapes1->SliceCount = this->m_world->Reports->size();
					this->vOutShapes2->SliceCount = this->m_world->Reports->size();
					this->vOutShape1->SliceCount = this->m_world->Reports->size();
					this->vOutShape2->SliceCount = this->m_world->Reports->size();
					this->vOutBody1->SliceCount = this->m_world->Reports->size();
					this->vOutBody2->SliceCount = this->m_world->Reports->size();
					this->vOutBodyId1->SliceCount = this->m_world->Reports->size();
					this->vOutBodyId2->SliceCount = this->m_world->Reports->size();
					this->vOutNew->SliceCount = this->m_world->Reports->size();
					this->m_shape1->Reset();
					this->m_shape2->Reset();
					this->mBody1->Reset();
					this->mBody2->Reset();

					for (unsigned int i = 0; i < this->m_world->Reports->size(); i++)
					{
						ContactReportId* result = this->m_world->Reports->at(i);
						
						
						/*this->vOutId->SetValue(i, Convert::ToDouble(result->id.key));
						this->vOutPosition->SetValue2D(i,result->position.x,result->position.y);*/
						
						//ShapeCustomData* sdata1 = (ShapeCustomData*)result->shape1->GetUserData();
						//this->vOutShapes1->SetValue(i,result->Shape1);
						//ShapeCustomData* sdata2 = (ShapeCustomData*)result->shape2->GetUserData();
						//this->vOutShapes2->SetValue(i,result->Shape2);

						/*bool found = false;
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
						}*/

						//BodyCustomData* bdata = (BodyCustomData*)result->shape1->GetBody()->GetUserData();
						this->vOutBodyId1->SetValue(i, result->Shape1);
						//bdata = (BodyCustomData*)result->shape2->GetBody()->GetUserData();
						this->vOutBodyId2->SetValue(i, result->Shape2);

						/*this->m_shape1->Add(result->shape1);
						this->m_shape2->Add(result->shape2);
						this->mBody1->Add(result->shape1->GetBody());
						this->mBody2->Add(result->shape2->GetBody());*/

					}
					this->vOutShape1->MarkPinAsChanged();
					this->vOutShape2->MarkPinAsChanged();
					this->vOutBody1->MarkPinAsChanged();
					this->vOutBody2->MarkPinAsChanged();
				}
				else 
				{
					this->vOutPosition->SliceCount = 0;
					this->vOutShapes1->SliceCount = 0;
					this->vOutShapes2->SliceCount = 0;
					this->vOutNew->SliceCount = 0;
				}
			} 
			else 
			{
				this->vOutPosition->SliceCount = 0;
				this->vOutShapes1->SliceCount = 0;
				this->vOutShapes2->SliceCount = 0;
				this->vOutNew->SliceCount = 0;
			}
		}


		void Box2dContactReportsNode::ConnectPin(v4::IPluginIO^ Pin)
		{
			if (Pin == this->vInWorld) 
			{
				v4::INodeIOBase^ usI;
				this->vInWorld->GetUpstreamInterface(usI);
				this->m_world = (v4b2d::WorldDataType^)usI;
			}
		}

		void Box2dContactReportsNode::DisconnectPin(v4::IPluginIO^ Pin)
		{
			if (Pin == this->vInWorld)
        	{
        		this->m_world = nullptr;
        	}
		}
	}
}
